import paho.mqtt.publish as publish

MQTT_BROKER = "MQTT_BROKER"
MQTT_PORT = 8883
MQTT_USER = "MQTT_USERNAME"
MQTT_PASS = "MQTT_PASSWORD"
TOPIC = "factory/line1/esp32/status"

publish.single(
    TOPIC,
    payload="Simulated telemetry message from Replit",
    hostname=MQTT_BROKER,
    port=MQTT_PORT,
    auth={'username': MQTT_USER, 'password': MQTT_PASS},
    tls={}  # Enables TLS
)

print("Published test message!")

print("Message sent!")
