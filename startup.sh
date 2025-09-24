#!/bin/bash

PROJECT_PATH=$1

if [ -z "$PROJECT_PATH" ]; then
  exit 1
fi
cd ../../
ls
risultato="$(find . -maxdepth 3 -type d -iname ${PROJECT_PATH} -print -quit)/client"
echo $risultato
FRONTEND_PATH=$(realpath "$risultato")

if [ ! -d "$FRONTEND_PATH" ]; then
  echo "Error: folder not found"
  exit 1
fi

echo "$FRONTEND_PATH" 

export FRONTEND_PATH
cd Apache/dev-server
ls
docker compose up -d
echo "Server started, frontend mounted from: $FRONTEND_PATH"
