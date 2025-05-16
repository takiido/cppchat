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