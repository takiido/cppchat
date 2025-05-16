#!/bin/bash
set -e

GREEN='\033[1;32m'
RED='\033[1;31m'
RESET='\033[0m'

draw_box() {
  local msg="$1"
  local color="$2"
  local width=50
  local padding=$(( (width - 2 - ${#msg}) / 2 ))
  local extra=$(( width - 2 - padding - ${#msg} ))

  local top="╔$(printf '═%.0s' $(seq 1 $((width - 2))))╗"
  local bottom="╚$(printf '═%.0s' $(seq 1 $((width - 2))))╝"
  local middle="║$(printf ' %.0s' $(seq 1 $padding))${msg}$(printf ' %.0s' $(seq 1 $extra))║"

  echo -e "\n${color}${top}\n${middle}\n${bottom}${RESET}\n"
}

draw_box "Generating init.sql from template..." "$GREEN"
chmod +x ./docker/postgres/gen-init-sql.sh
./docker/postgres/gen-init-sql.sh
export $(grep -v '^#' .env.dev | xargs)

check_var() {
  local name="$1"
  local value="${!name}"
  if [ -n "$value" ]; then
    echo -e "${GREEN}✔${RESET} $name\t\t${GREEN}Found${RESET}"
  else
    echo -e "${RED}✘${RESET} $name\t\t${RED}Missing${RESET}"
    missing_env=true
  fi
}

draw_box "Validating init.sql generation..." "$GREEN"

missing_sql=false
if [ -f "./docker/postgres/init.sql" ]; then
  echo -e "${GREEN}✔${RESET} init.sql\t\t\t${GREEN}Exists${RESET}"
else
  echo -e "${RED}✘${RESET} init.sql\t\t\t${RED}Missing${RESET}"
  missing_sql=true
fi

draw_box "Checking required environment variables..." "$GREEN"

required_vars=(
  CPPCHAT_DB_USER
  CPPCHAT_DB_PSSWD
  GRAFANA_DB_PSSWD
)

missing_env=false
for var in "${required_vars[@]}"; do
  check_var "$var"
done

if [ "$missing_sql" = true ] || [ "$missing_env" = true ]; then
  draw_box "Environment validation failed. Aborting build." "$RED"
  exit 1
else
  draw_box "All checks passed. Continuing..." "$GREEN"
fi

draw_box "Stopping and removing existing containers..." "$GREEN"
docker compose --env-file .env.dev down

draw_box "Rebuilding images..." "$GREEN"
docker compose --env-file .env.dev build > /dev/null

draw_box "Starting containers..." "$GREEN"
docker compose --env-file .env.dev up -d --wait --no-log-prefix --quiet-pull

services=$(docker compose --env-file .env.dev ps --services)
all_healthy=true

for service in $services; do
  status=$(docker inspect --format='{{json .State.Health.Status}}' cppchat-${service} 2>/dev/null | tr -d '"')

  if [[ "$status" == "healthy" || -z "$status" ]]; then
    continue
  else
    all_healthy=false
    echo -e "${RED}Service $service is not healthy (status: $status)${RESET}"
  fi
done

if [ "$all_healthy" = true ]; then
  draw_box "All containers started successfully!" "$GREEN"
else
  draw_box "Some containers may have issues. Check logs." "$RED"
fi

draw_box "Cleaning up..." $GREEN
rm ./docker/postgres/init.sql