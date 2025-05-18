#!/bin/bash

mkdir -p /etc/basicstation
cp /usr/local/bin/station.conf /etc/basicstation/station.conf
echo "Generating station.conf..."
sed -i "s/\${GATEWAY_EUI}/${GATEWAY_EUI}/g" /etc/basicstation/station.conf

echo "Generating tc.uri..."
echo "wss://${TTN_REGION}.${GATEWAY_SERVER}:8887" > /etc/basicstation/tc.uri

echo "Generating tc.key..."
echo "Authorization: Bearer ${GATEWAY_API_KEY}" | perl -p -e 's/\r\n|\n|\r/\r\n/g' >> /etc/basicstation/tc.key

echo "Generating tc.trust..."
cp /etc/ssl/certs/ca-certificates.crt /etc/basicstation/tc.trust

echo "Starting BasicStation..."
cd /etc/basicstation
exec /usr/local/bin/station