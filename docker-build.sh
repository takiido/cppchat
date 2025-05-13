#!/bin/bash
set -e

GREEN='\033[1;32m'
RESET='\033[0m'

draw_box() {
  local msg="$1"
  local width=50
  local padding=$(( (width - 2 - ${#msg}) / 2 ))
  local extra=$(( width - 2 - padding - ${#msg} ))

  local top="╔$(printf '═%.0s' $(seq 1 $((width - 2))))╗"
  local bottom="╚$(printf '═%.0s' $(seq 1 $((width - 2))))╝"
  local middle="║$(printf ' %.0s' $(seq 1 $padding))${msg}$(printf ' %.0s' $(seq 1 $extra))║"

  echo -e "\n${GREEN}${top}\n${middle}\n${bottom}${RESET}\n"
}

draw_box "Stopping and removing existing containers..."
docker compose --env-file .env.dev down

draw_box "Rebuilding images..."
docker compose --env-file .env.dev build

draw_box "Starting containers..."
docker compose --env-file .env.dev up
