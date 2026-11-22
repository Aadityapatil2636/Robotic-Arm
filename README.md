# 🤖 ESP32-Based 4-Axis Robotic Arm

A Wi-Fi-controlled 4-axis robotic arm developed using an ESP32 and four servo motors. The project features a custom web-based interface that enables smooth, wireless and independent control of each robotic arm joint.

## ✨ Features

- **4-Axis Movement:** Independent control of Base, Shoulder, Elbow and Wrist.
- **Wireless Control:** ESP32 creates its own Wi-Fi access point.
- **Web-Based Interface:** Custom-designed interface accessible through a web browser.
- **Smooth Movement:** Gradual servo angle adjustments for controlled motion.
- **Individual Joint Control:** Dedicated arrow controls for each servo.
- **Real-Time Monitoring:** Live angle updates for all four servo motors.
- **Home Position:** Return all four servos to their default 90° position.
- **Responsive Design:** Control interface designed for mobile and desktop devices.

## 🛠️ Hardware Components

| Component | Quantity |
|---|---:|
| ESP32 Development Board | 1 |
| Servo Motors | 4 |
| Robotic Arm Structure | 1 |
| External Servo Power Supply | 1 |
| Jumper Wires | As required |

## 💻 Technologies Used

- Embedded C++
- Arduino IDE
- ESP32 Wi-Fi
- HTML
- CSS
- JavaScript
- ESP32Servo Library

## 🔌 Hardware Connections

| Robotic Arm Joint | ESP32 GPIO Pin |
|---|---|
| Base Servo | GPIO 13 |
| Shoulder Servo | GPIO 14 |
| Elbow Servo | GPIO 25 |
| Wrist Servo | GPIO 26 |

## 🚀 Getting Started

### 1. Requirements

- Arduino IDE
- ESP32 board package
- ESP32Servo library
- ESP32 development board
- Four compatible servo motors

### 2. Uploading the Code

1. Clone or download this repository.
2. Open `robotic_arm.ino` in Arduino IDE.
3. Install the required ESP32 board package and ESP32Servo library.
4. Select your ESP32 board and corresponding COM port.
5. Upload the program to the ESP32.

### 3. Connecting to the Robotic Arm

After uploading:

1. Power on the ESP32.
2. Connect your phone or laptop to the following Wi-Fi network:

   **Wi-Fi Name:** `RobotArm`

   **Password:** `12345678`

3. Open a web browser.
4. Enter the following IP address:

   `http://192.168.4.1`

5. The robotic arm control interface will appear.

## 🎮 Web-Based Controls

| Control | Function |
|---|---|
| Left Arrow | Move the selected joint in one direction |
| Right Arrow | Move the selected joint in the opposite direction |
| Hold Arrow | Continuously move the selected joint |
| Home Position | Return all servos to 90° |

## ⚙️ Movement Configuration

The firmware includes configurable movement parameters:

- Individual movement step sizes for each servo.
- Adjustable servo movement speed.
- Smooth transitions between current and target angles.
- Configurable movement interval.
- Software angle constraints.

## 📸 Project Gallery

Project photographs will be added here.

## 🎥 Demonstration

The demonstration video will be added here.

## 📂 Repository Structure


Robotic-Arm/
│
├── robotic_arm.ino
├── images/
│   ├── robotic-arm-front.jpg
│   └── robotic-arm-side.jpg
│
├── video/
│   └── robotic-arm-demo.mp4
│
├── README.md
└── LICENSE


## 👥 Contributors

- Aaditya Patil
- Rishi Neerati

## 🔮 Future Improvements

- Object gripping and pick-and-place operations.
- Preset movement sequences.
- Improved joint calibration.
- Automated robotic arm movements.

---

**Developed using ESP32, servo motors and web technologies.**
