#!/bin/bash
set -e

BUILD_DIR="build"
ENV_FILE=".env.dev"

# Load env vars from .env.dev if exists
if [ -f "$ENV_FILE" ]; then
  echo "📄 Loading environment from $ENV_FILE"
  export $(grep -v '^#' "$ENV_FILE" | xargs)
fi

build_target() {
  TARGET=$1
  echo "🛠️  Building $TARGET..."
  cmake --build "$BUILD_DIR" --target "$TARGET"
}

run_target() {
  BINARY_PATH="$1"
  echo "👉 BINARY PATH: $BINARY_PATH"
  echo "🚀 Running $BINARY_PATH"
  "$BINARY_PATH"
}

# Configure CMake if not done
if [ ! -f "$BUILD_DIR/CMakeCache.txt" ]; then
  echo "📦 Running initial CMake config..."
  cmake -B "$BUILD_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
fi

case "$1" in
  app)
    build_target cppchat_app
    run_target "$BUILD_DIR/app/cppchat_app"
    ;;
  server)
    build_target cppchat_server
    run_target "$BUILD_DIR/server/cppchat_server"
    ;;
  *)
    echo "❌ Invalid usage"
    echo "Usage: $0 [app|server]"
    exit 1
    ;;
esac