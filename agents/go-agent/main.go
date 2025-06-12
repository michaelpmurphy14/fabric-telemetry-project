package main

import (
		"crypto/tls"
		"fmt"
		"os"
		"time"

		mqtt "github.com/eclipse/paho.mqtt.golang"
)

func main() {
		broker := os.Getenv("MQTT_BROKER") // e.g., "your-cluster.hivemq.cloud"
		username := os.Getenv("MQTT_USERNAME")
		password := os.Getenv("MQTT_PASSWORD")

		opts := mqtt.NewClientOptions()
		opts.AddBroker("tls://" + broker)
		opts.SetUsername(username)
		opts.SetPassword(password)
		opts.SetClientID("go-subscriber-" + fmt.Sprint(time.Now().Unix()))

		// ✅ Add TLS config
		tlsConfig := &tls.Config{
				InsecureSkipVerify: true, // OK for development — use CA certs in production
				ClientAuth:         tls.NoClientCert,
		}
		opts.SetTLSConfig(tlsConfig)

		client := mqtt.NewClient(opts)
		if token := client.Connect(); token.Wait() && token.Error() != nil {
				panic(token.Error())
		}

		client.Subscribe("factory/line1/esp32/status", 0, func(client mqtt.Client, msg mqtt.Message) {
				fmt.Printf("Received: %s from topic: %s\n", msg.Payload(), msg.Topic())
		})

		fmt.Println("Subscribed to factory telemetry...")
		for {
				time.Sleep(1 * time.Second)
		}
}
