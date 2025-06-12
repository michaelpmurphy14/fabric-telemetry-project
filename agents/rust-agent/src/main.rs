use dotenv::dotenv;
use rumqttc::{Client, Event, MqttOptions, Packet, QoS};
use serde::Deserialize;
use serde_json;
use std::env;
use std::time::Duration;

#[derive(Deserialize, Debug)]
struct Telemetry {
    device_id: String,
    status: String,
    temp_c: f32,
    uptime_sec: u64,
    rssi: i32,
    heap_free: u32,
    alert: Option<String>, // ← new and optional
}

fn main() {
    dotenv().ok();

    let broker = env::var("MQTT_BROKER").expect("Missing MQTT_BROKER");
    let username = env::var("MQTT_USERNAME").expect("Missing MQTT_USERNAME");
    let password = env::var("MQTT_PASSWORD").expect("Missing MQTT_PASSWORD");

    let mut mqttoptions = MqttOptions::new("rust-agent", broker, 8883);
    mqttoptions.set_keep_alive(Duration::from_secs(5));
    mqttoptions.set_credentials(username, password);
    mqttoptions.set_transport(rumqttc::Transport::Tls(Default::default()));

    let (mut client, mut connection) = Client::new(mqttoptions, 10);

    // Subscribe to wildcard topic to simulate scalable factory line ingestion
    client
        .subscribe("factory/line1/+/+/status", QoS::AtMostOnce)
        .unwrap();

    println!("📡 Subscribed and listening on factory/line1/+/+/status...");

    let mut counter = 0;

    let mut counter = 0;

    for event in connection.iter() {
        match event {
            Ok(Event::Incoming(Packet::Publish(p))) => {
                let payload = String::from_utf8_lossy(&p.payload);

                match serde_json::from_str::<Telemetry>(&payload) {
                    Ok(data) => {
                        counter += 1;
                        println!("✅ [{}] Parsed Telemetry: {:?}", counter, data);

                        // NEW: log alerts if present
                        if let Some(alert_msg) = &data.alert {
                            println!(
                                "🚨 ALERT [{}]: {} from {}",
                                counter, alert_msg, data.device_id
                            );
                        }
                    }
                    Err(e) => {
                        println!("❌ Failed to parse JSON: {}", e);
                        println!("Raw payload: {}", payload);
                    }
                }
            }

            Ok(_) => {} // handle non-publish events quietly
            Err(e) => {
                println!("❗ MQTT error: {}", e);
                break;
            }
        }
    }
}
