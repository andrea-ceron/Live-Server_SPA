#!/bin/bash
FRONTEND_PATH=$1

if [ -z "$FRONTEND_PATH" ]; then
  echo "Uso: ./startup.sh /path/del/frontend"
  exit 1
fi

export FRONTEND_PATH=$(realpath "$FRONTEND_PATH")

docker-compose up -d
