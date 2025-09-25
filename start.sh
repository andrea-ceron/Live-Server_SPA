#!/bin/bash
set -e

# Definiamo il percorso interno valido. 
# Questo è il percorso dove il volume è mappato (DocumentRoot di Apache).
INTERNAL_PATH="./var/www/html"

echo "Starting C++ Reloader App for internal path: $INTERNAL_PATH"

# 1. Esegui il C++ e PASSA il percorso interno come argomento.
# Questo garantisce che 'argc' sia 2 (programma + argomento).
/usr/local/bin/dev_reloader "$INTERNAL_PATH" & 

echo "Starting Apache HTTP Server in foreground..."

# 2. Avvia Apache.
exec apache2ctl -D FOREGROUND