#!/usr/bin/env bash

set -eEuo pipefail

APPENGINE_TOKEN=''
ASTARTE_BASE_URL=''
REALM=''
DEVICE_ID=''
INTERFACE='org.astarte-platform.cpp.examples.AggregateLongPath'
INT_PATH="/request/test_object_path"
DATA='{"string_endpoint":"test","longinteger_endpoint":1000000,"boolean_endpoint":true}'

curl -v -X POST -H "Accept: application/json" -H "Content-Type: application/json" \
    -H "User-Agent: astarte-go" \
    -H "Authorization: Bearer $APPENGINE_TOKEN" \
    --data "{\"data\":$DATA}" \
    "https://$ASTARTE_BASE_URL/appengine/v1/$REALM/devices/$DEVICE_ID/interfaces/$INTERFACE/$INT_PATH"
