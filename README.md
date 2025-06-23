![CI](https://github.com/michaelpmurphy14/fabric-telemetry-project/actions/workflows/secure-pipeline.yml/badge.svg)

# Fabric Telemetry Demo

A secure, extensible telemetry ingestion pipeline using:
- 📶 ESP32 hardware nodes
- 📡 MQTT messaging (via HiveMQ + AWS IoT Core)
- 🦀 Rust & 🐹 Go edge agents
- 📊 Grafana Cloud for dashboarding
- ☁️ AWS Timestream for time series storage
- 🔐 GitHub Actions with Syft + Grype for secure CI/CD

---

## ⚙️ Architecture Overview

[ESP32 CAM] → MQTT → [HiveMQ & AWS IoT Core] → Rust/Go Agents + AWS Timestream → Grafana Dashboards

---

## 🧪 Live Data Visualizations

This project supports real-time dashboards including:
- Temperature trends
- Device uptime
- Heap memory & RSSI
- Alert events for device health

---

## 🔐 Security Pipeline

CI/CD Workflow includes:
- [x] Syft Software Bill of Materials (SBOM)
- [x] Grype Vulnerability Scanning
- [x] GitHub Actions automation

---

## 🧭 Project Roadmap

- [x] ESP32 telemetry publisher
- [x] Dual MQTT subscriber (Rust & Go)
- [x] AWS-native telemetry pipeline
- [x] Grafana Cloud dashboards
- [ ] Multi-node template support
- [ ] CI/CD deploy + alerting
- [ ] OTA update pipeline for ESP32 firmware

---

## 📂 Repo Structure

esp32-firmware/ # Arduino code + certs
agents/ # Rust and Go MQTT clients
scripts/ # Python simulators
dashboards/ # Grafana JSON exports
.github/workflows/ # CI pipeline


---

## 🔐 Secure Setup Instructions

This project uses local-only credentials and certificates to protect sensitive information. Follow these steps to configure your environment without exposing secrets in the repository.

### 1. `secrets.h`

This file contains Wi-Fi credentials, MQTT usernames/passwords, and AWS endpoints.

- **Do not commit** this file to GitHub.  
- Use the provided `secrets_example.h` as a template.

#### To configure:

1. Duplicate the template:  
   ```bash
    esp32-firmware/include/secrets_example.h esp32-firmware/include/secrets.h
2. Open esp32-firmware/include/secrets.h and update the placeholders:
    ```bash
       #define WIFI_SSID       "your-network-ssid"
       #define WIFI_PASSWORD   "your-wifi-password"

       #define HIVEMQ_ENDPOINT "your-hivemq-endpoint"
       #define HIVEMQ_PORT     8883
       #define HIVEMQ_USER     "your-mqtt-username"
       #define HIVEMQ_PASS     "your-mqtt-password"

       #define AWS_IOT_ENDPOINT "your-aws-iot-endpoint"
       #define AWS_IOT_PORT     8883

### certificates.h

This file holds TLS certificates for HiveMQ and AWS.

- **Do not commit** this file to GitHub.
- Use certificates_example.h to see the expected structure.

#### To configure:

1. Duplicate the example:
   ```bash
   esp32-firmware/certs/certificates_example.h esp32-firmware/certs/certificates.h
2. Paste your real PEM-formatted certificates and keys into the placeholders:
   ```bash
   const char* AWS_CERT_CA = R"EOF(
    -----BEGIN CERTIFICATE-----
    ...your CA cert here...
    -----END CERTIFICATE-----
    )EOF";

    const char* AWS_CERT_CRT = R"EOF(
    -----BEGIN CERTIFICATE-----
    ...your client cert here...
    -----END CERTIFICATE-----
    )EOF";

    const char* AWS_CERT_PRIVATE = R"EOF(
    -----BEGIN RSA PRIVATE KEY-----
    ...your private key here...
    -----END RSA PRIVATE KEY-----
    )EOF";

### ✅ .gitignore Configuration
These entries ensure your sensitive files stay local:
   ```bash
   esp32-firmware/include/secrets.h
   esp32-firmware/include/secrets_station*.h
   esp32-firmware/certs/certificates.h
   ```


Anyone cloning the repo can copy the _example.h files, fill in their own values, and build without risk of leaking secrets.

---
## ↔️ Building for Multiple Nodes

This project uses per-node JSON configs and a small generator script to produce a single `node_config.h` at build time. You must tell the compiler which JSON to use via a build flag.

### 1. Manual Build with Arduino-CLI

```bash
# Generate node_config.h from your chosen JSON:
./scripts/gen-config.sh configs/nodeA.json

# Compile & upload, passing NODE_CONFIG to the preprocessor
arduino-cli compile \
  --fqbn esp32:esp32:esp32dev \
  -D NODE_CONFIG=\"configs/nodeA.json\" \
  esp32-firmware/src/fabric-esp32-test.ino

arduino-cli upload \
  --fqbn esp32:esp32:esp32dev \
  -D NODE_CONFIG=\"configs/nodeA.json\" \
  esp32-firmware/src/fabric-esp32-test.ino
```
Replace nodeA.json with whichever config (e.g. nodeB.json) you want to build.

### 2. One-step Shell Scripts
You can wrap both steps into a single script for convenience. For example, create:
```bash
# scripts/build-nodeA.sh
#!/usr/bin/env bash
./scripts/gen-config.sh configs/nodeA.json
arduino-cli compile --fqbn esp32:esp32:esp32dev \
  -D NODE_CONFIG=\"configs/nodeA.json\" \
  esp32-firmware/src/fabric-esp32-test.ino
```

Make it executable (chmod +x scripts/build-nodeA.sh), then simply run:
```bash
./scripts/build-nodeA.sh
```
### 3. PlatformIO / VSCode Integration
If you prefer PlatformIO in VSCode, add an environment per node to platformio.ini:
```bash
[env:nodeA]
platform = espressif32
board    = esp32dev
framework = arduino
build_flags =
  -DNODE_CONFIG=\"configs/nodeA.json\"
```

Then in the bottom status bar select env:nodeA and click Build or Upload—PlatformIO will automatically pass the right flag and generate node_config.h. Tip: Be sure to add esp32-firmware/include/node_config.h to your .gitignore, since it’s regenerated on each build.

---

## 🧠 Author

Michael P. Murphy  
🛡 Aerospace & Defense | ⚙️ Systems Engineering | 🔐 Secure DevOps  
📍 Dallas, TX  
🔗 [github.com/michaelpmurphy14](https://github.com/michaelpmurphy14)
