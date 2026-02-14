#!/bin/bash
set -e

INTERNAL_PATH="./var/www/html"

echo "Starting C++ Reloader App for internal path: $INTERNAL_PATH"


/usr/local/bin/dev_reloader "$INTERNAL_PATH" & 

echo "Starting Apache HTTP Server in foreground..."

exec apache2ctl -D FOREGROUND