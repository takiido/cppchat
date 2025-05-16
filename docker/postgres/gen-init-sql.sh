#!/bin/bash
set -e

if [ -f "./docker/postgres/init.sql" ]; then
  rm ./docker/postgres/init.sql
fi

export $(grep -v '^#' .env.dev | xargs)
envsubst < ./docker/postgres/init.template.sql > ./docker/postgres/init.sql