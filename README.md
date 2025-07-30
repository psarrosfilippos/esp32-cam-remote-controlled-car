# ESP32-CAM Remote Controlled Car

This project implements a simple remote-controlled car using the ESP32-CAM module and the OV2640 camera. The car can be controlled via a web interface that streams live video and sends movement commands to the L298N motor driver.

## Features

- Live video streaming via the onboard OV2640 camera
- Web-based control interface (forward / backward / left / right / stop)
- Standalone operation (no external server needed)
- Compatible with mobile, tablet, and desktop devices

## Hardware Components

- ESP32-CAM (WROVER module)
- OV2640 Camera (built-in)
- L298N Motor Driver
- 4x DC Motors (for movement)
- Power supply (e.g., 18650 batteries)
- Jumper wires, wheels, and car frame

## Wiring

| ESP32 Pin | L298N IN |
|-----------|----------|
| IO2       | IN1      |
| IO14      | IN2      |
| IO15      | IN3      |
| IO13      | IN4      |
| GND       | GND      |
| 5V        | ENA/ENB  |

## Usage Instructions

1. Upload the code to the ESP32-CAM using the Arduino IDE.
2. Connect the wires according to the table above.
3. Open the Serial Monitor to see the device IP address.
4. Access that IP from a web browser.
5. Use the interface to control the car and view the live stream.

## Web Interface

The UI includes directional control buttons and displays the live camera feed. It is lightweight, responsive, and works on both desktop and mobile.

## Repository Contents

- `cam_car_esp32.ino` Main Arduino sketch for ESP32-CAM control
- `README.md`: Project overview and instructions
- `wiring diagrams and physical photo` for better understanding

## License

MIT License – Free to use, modify, and share for personal and educational purposes.

## Future improvements

- Obstacle avoidance using ultrasonic sensors  
- Bluetooth or mobile app-based control  
- Autonomous navigation and path planning  
- Battery voltage monitoring  


## Authors

Filippos Psarros

informatics and telecommunications Student

GitHub: psarrosfilipposploading

[README.md](https://github.com/user-attachments/files/21516742/README.md)
