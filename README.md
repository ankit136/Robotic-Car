# Robotic Car using ESP32, L298N Motor Driver, 5V DC Geared Motors, and Dabble App

## Overview
This project demonstrates how to build and control a robotic car using the ESP32 microcontroller, L298N motor driver, 5V DC geared motors, and the Dabble app. The car can be controlled wirelessly via Bluetooth using the Dabble app on a smartphone.

## Components Required
- **ESP32**: Microcontroller for controlling the robotic car.
- **L298N Motor Driver**: Dual H-Bridge motor driver for driving the motors.
- **5V DC Geared Motors**: Motors to drive the wheels.
- **Wheels**: Attached to the DC motors.
- **Chassis**: Frame to hold all components.
- **Battery**: Power source for the motors and ESP32.
- **Bluetooth Module**: Integrated with ESP32 for wireless communication.
- **Dabble App**: Smartphone app for controlling the car.

## Wiring Diagram
1. **ESP32 Connections**:
   - Connect the **IN1** pin of the L298N to **GPIO 25** of the ESP32.
   - Connect the **IN2** pin of the L298N to **GPIO 26** of the ESP32.
   - Connect the **IN3** pin of the L298N to **GPIO 27** of the ESP32.
   - Connect the **IN4** pin of the L298N to **GPIO 14** of the ESP32.
   - Connect the **EN1** pin of the L298N to **GPIO 32** of the ESP32.
   - Connect the **EN2** pin of the L298N to **GPIO 33** of the ESP32.

2. **Motor Connections**:
   - Connect the **output pins** of the L298N to the **motors**.
   - Connect the **motor power supply** to the **L298N**.

3. **Power Connections**:
   - Connect the **power supply** to the **ESP32**.
   - Ensure a common **ground** between the ESP32 and L298N.

## Installation
1. **Arduino IDE Setup**:
   - Install the Arduino IDE from [Arduino](https://www.arduino.cc/en/Main/Software).
   - Install the ESP32 board package. Go to **File > Preferences** and add this URL to **Additional Board Manager URLs**: `https://dl.espressif.com/dl/package_esp32_index.json`.
   - Go to **Tools > Board > Boards Manager** and install the **ESP32** package.

2. **Library Installation**:
   - Install the **Dabble** library. Go to **Sketch > Include Library > Manage Libraries** and search for **Dabble**. Install it.

3. **Dabble App Installation**:
   - Install the **Dabble app** on your smartphone from the [Google Play Store](https://play.google.com/store/apps/details?id=io.dabbleapp) or [App Store](https://apps.apple.com/us/app/dabble/id1437038606).

## Code
```cpp
#include <DabbleESP32.h>
#define enA 32
#define in1 25
#define in2 26
#define enB 33
#define in3 27
#define in4 14

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize motor control pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Initialize Dabble
  Dabble.begin("YourBluetoothName"); // Initialize with Bluetooth name
}

void loop() {
  // Refresh Dabble communication
  Dabble.processInput();
  
  // Get joystick values
  int x = GamePad.getXaxisData();
  int y = GamePad.getYaxisData();
  
  // Control motors based on joystick input
  if (y > 0) { // Forward
    analogWrite(enA, y);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, y);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else if (y < 0) { // Backward
    analogWrite(enA, -y);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enB, -y);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else if (x > 0) { // Right
    analogWrite(enA, x);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enB, x);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else if (x < 0) { // Left
    analogWrite(enA, -x);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enB, -x);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else { // Stop
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }
}
