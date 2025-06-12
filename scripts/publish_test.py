import paho.mqtt.publish as publish

print("Preparing to publish...")

MQTT_BROKER = "MQTT_BROKER"
MQTT_PORT = 8883
MQTT_USER = "MQTT_USERNAME"     # replace with HiveMQ username
MQTT_PASS = "MQTT_PASSWORD"     # replace with HiveMQ password
TOPIC = "factory/line1/esp32/status"

publish.single(
    topic=TOPIC,
    payload="Hello from Python via Replit",
    hostname=MQTT_BROKER,
    port=MQTT_PORT,
    auth={'username': MQTT_USER, 'password': MQTT_PASS},
    tls={}
)

print("Message sent.")
