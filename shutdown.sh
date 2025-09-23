#!/bin/bash

if [ -f .frontend_path ]; then
  export FRONTEND_PATH=$(cat .frontend_path)
else
  echo "path file not found, please run startup.sh first"
fi

docker compose down

rm -f .frontend_path

echo "Server down"
