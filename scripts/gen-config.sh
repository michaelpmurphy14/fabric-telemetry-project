#!/usr/bin/env bash
# Usage: scripts/gen-config.sh <path/to/config.json>

CONFIG_JSON="$1"
HEADER="esp32-firmware/include/node_config.h"

# extract values via jq
DEVICE_ID=$(jq -r .device_id "$CONFIG_JSON")
MQTT_BASE_TOPIC=$(jq -r .mqtt_base_topic "$CONFIG_JSON")
TEMP_MIN=$(jq .temp_min "$CONFIG_JSON")
TEMP_MAX=$(jq .temp_max "$CONFIG_JSON")
ALERT_RSSI=$(jq .alert_rssi "$CONFIG_JSON")

# write the header
cat > "$HEADER" << EOF
#pragma once
// Auto-generated from $CONFIG_JSON
static const char* DEVICE_ID       = "$DEVICE_ID";
static const char* MQTT_BASE_TOPIC = "$MQTT_BASE_TOPIC";
static const float TEMP_MIN        = $TEMP_MIN;
static const float TEMP_MAX        = $TEMP_MAX;
static const int   ALERT_RSSI      = $ALERT_RSSI;
EOF
