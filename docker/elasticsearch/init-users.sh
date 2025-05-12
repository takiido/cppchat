#!/bin/bash
set -e

# Start Elasticsearch
/usr/local/bin/docker-entrypoint.sh &

# Wait for Elasticsearch REST API to be up
echo "⏳ Waiting for Elasticsearch API..."
until curl -s http://localhost:9200 >/dev/null; do
  sleep 2
done

# Wait for .security-7 index to be GREEN
echo "⏳ Waiting for .security-7 index to be ready..."
until curl -s -u "elastic:${ELASTIC_PASSWORD}" http://localhost:9200/_cluster/health/.security-7?wait_for_status=green | grep -q '"status":"green"'; do
  echo "⏳ .security-7 not ready yet..."
  sleep 2
done

# Now set the kibana_system password
echo "🔐 Setting kibana_system password..."
RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" -u "elastic:${ELASTIC_PASSWORD}" -X POST http://localhost:9200/_security/user/kibana_system/_password \
  -H "Content-Type: application/json" -d '{
    "password": "'"${KIBANA_SYS_PSSWD}"'"
}')

if [ "$RESPONSE" -eq 200 ]; then
  echo "✅ kibana_system password successfully set."
else
  echo "❌ Failed to set kibana_system password. HTTP code: $RESPONSE"
  exit 1
fi

echo "🔍 Checking if user 'kibana_admin' exists..."
USER_CHECK=$(curl -s -o /dev/null -w "%{http_code}" -u "elastic:${ELASTIC_PASSWORD}" \
  http://localhost:9200/_security/user/kibana_admin)

if [ "$USER_CHECK" -eq 200 ]; then
  echo "ℹ️ User 'kibana_admin' already exists. Skipping creation."
else
  echo "➕ Creating user 'kibana_admin'..."
  curl -s -u "elastic:${ELASTIC_PASSWORD}" -X POST http://localhost:9200/_security/user/kibana_admin \
    -H "Content-Type: application/json" -d '{
      "password": "'"${KIBANA_ADMIN_PSSWD}"'",
      "roles": ["kibana_admin"],
      "full_name": "Kibana Admin",
      "email": "admin@example.com"
    }'
  echo "\n✅ User 'kibana_admin' created."
fi

wait
