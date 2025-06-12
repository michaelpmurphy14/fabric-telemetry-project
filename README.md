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

## 🧠 Author

Michael P. Murphy  
🛡 Aerospace & Defense | ⚙️ Systems Engineering | 🔐 Secure DevOps  
📍 Dallas, TX  
🔗 [github.com/michaelpmurphy14](https://github.com/michaelpmurphy14)
