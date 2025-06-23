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

## 🧠 Author

Michael P. Murphy  
🛡 Aerospace & Defense | ⚙️ Systems Engineering | 🔐 Secure DevOps  
📍 Dallas, TX  
🔗 [github.com/michaelpmurphy14](https://github.com/michaelpmurphy14)
