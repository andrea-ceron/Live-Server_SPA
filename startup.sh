#!/bin/bash

FRONTEND_PATH=$1

if [ -z "$FRONTEND_PATH" ]; then
  exit 1
fi

FRONTEND_PATH=$(realpath "$FRONTEND_PATH")

if [ ! -d "$FRONTEND_PATH" ]; then
  echo "Error: folder not found"
  exit 1
fi

echo "$FRONTEND_PATH" > .frontend_path

export FRONTEND_PATH

docker compose up -d

echo "Server started, frontend mounted from: $FRONTEND_PATH"
