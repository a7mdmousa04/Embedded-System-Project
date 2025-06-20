# 🌊 Water Level Monitoring System for Lake Nasser

## 📖 Project Overview

This project presents an **IoT-based water level monitoring system** specifically designed for **Lake Nasser** in Egypt. The system consists of **two interconnected embedded circuits**:

1. **Sender Unit** – Installed on the **Aswan High Dam**  
2. **Receiver Unit** – Placed at the **shore of the lake**

Both circuits use **ESP8266 WiFi modules**, **ultrasonic sensors** for measuring water levels, and **buck converters** for power regulation. The receiver unit also uses a **GSM module** to send data to a remote website.
Real-time data is collected from both units and displayed **live on a dedicated website**.

---

## 📡 Sender Unit (Installed on the Dam)

- 📍 **Location**: Fixed on the Aswan High Dam, overlooking the center of Lake Nasser.
- ⚙️ **Components**:
  - ESP8266 WiFi Module  
  - Ultrasonic Sensor  
  - Buck Converter  
- 🧠 **Function**:
  - Measures the water level near the dam using an ultrasonic sensor.
  - Sends the measured data wirelessly to the receiver unit via WiFi.

---

## 🌐 Receiver Unit (At the Lake Shore)

- 📍 **Location**: Installed at the edge of Lake Nasser.
- ⚙️ **Components**:
  - ESP8266 WiFi Module  
  - Ultrasonic Sensor  
  - GSM Module  
  - Buck Converter  
- 🧠 **Function**:
  - Measures the local water level at the shore.
  - Receives remote measurements from the sender unit.
  - Uploads both water level readings to a **live website** via GSM for real-time monitoring.

---

## 🖥️ Live Data Display

- A custom website is used to display real-time water level readings.
- Users can monitor both dam and shore levels directly from the browser.

🔗 **Live Website**: [https://embeddedsysproject-production.up.railway.app/](https://embeddedsysproject-production.up.railway.app/)

---

## 🎯 Project Goals

- Enable real-time water level monitoring in Lake Nasser.
- Support early warning systems for water level changes.
- Facilitate remote environmental data collection for research or governmental use.

---

## 🚀 Future Improvements

- Integrate solar power for long-term field deployment.
- Add SD card storage for local data logging.
- Improve the web interface for enhanced data visualization and historical trends.

---
## 👥 Team Members

- Mohamed Hisham
- Ahmed mohamed AboElmakarem
- Ahmed mohamed Mousa
- Mohamed Nagi
- Mahmoud Ezat
- Mohamed Moawad
- Mohamed shiple
- Youssef Makarm
- Ahmed Shousha
- Marwa Ibrahim
- Hazem Ahmed
