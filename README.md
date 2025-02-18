# 🛍️ DistributoreSwag  

Welcome to the **Distributore Swag** repository! This project manages a water dispenser controlled via an ESP32.

## 📈 Requirements
### Hardware
ESP32 and relative sensors: 
  - Buzzer
  - Display LCD 1602 with I2C module
  - RGB LED
  - Button
  - Ultrasonic HC-SR04 sensor
  - Microservo 9G SG90 5V
  - Breadboard, cables and resistors
### Software 
VS Code with Platform IO.
## 🌲 Project tree organization
```plaintext
.
├── README.md
├── lib
│   ├── AP
│   │   ├── AP.cpp
│   │   ├── AP.h
│   │   ├── captiveRequestHandler.cpp
│   │   └── captiveRequestHandler.h
│   ├── button
│   │   ├── button.cpp
│   │   └── button.h
│   ├── buzzer
│   │   ├── buzzer.cpp
│   │   ├── buzzer.h
│   │   └── pitches.h
│   ├── display
│   │   ├── display.cpp
│   │   └── display.h
│   ├── logic
│   │   ├── logic.cpp
│   │   └── logic.h
│   ├── rgb
│   │   ├── rgb.cpp
│   │   └── rgb.h
│   ├── servo
│   │   ├── servo.cpp
│   │   └── servo.h
│   ├── timerControl
│   │   ├── timerControl.cpp
│   │   └── timerControl.h
│   └── ultrasound
│       ├── ultrasound.cpp
│       └── ultrasound.h
├── platformio.ini
├── src
│   └── main.cpp
```

## Getting started
### How to build the project
You can either clone this repository via terminal
``` bash
git clone https://github.com/renna3/distributoreSwag.git
```
or download the project folder.  

Then, using VS Code with the PlatformIO extension, open the project (PlatformIO Home Page --> Open Project). 

Ultimately, using the terminal inside VS Code, proceed to build the project
``` sh
platformio run
```
alternatively, click on the ✔️ in the bottom bar.

### How to burn and run the project

First, you need to connect the ESP32 to the computer.  

Then, proceed by uploading the built project through the terminal inside VS Code (specifying the target port)
``` sh
pio run --target upload
```
alternatively, click on the → in the bottom bar.

To run the project, just click on the RST button on the ESP32.

## Project diagram and user guide
<img src="Project design.jpeg" alt="My Image" width="600">

### ⚙️ DistributoreSwag Workflow
#### 1. Ready State  
- The system is powered on and enters the **ready** state.  
- The ESP32 is set as an AP. Wi-fi is now ready.  
- The display will show: **"Ready to connect"**.  
- **STATUS_LED:** 🔵 (BLUE).  

#### 2. Connection and Quantity Selection  
- The user connects to the dispenser’s WiFi via their phone and scans the QR code to access the local dispenser page.  
- They select the amount of water to dispense (**0.2L, 0.33L, 0.5L, 1L**).  
- **STATUS_LED:** 🟣 (PURPLE) when a device connects.  
- **STATUS_LED:** 🟡 (YELLOW) when a selection is made.  

#### 3. Bottle Detection and Dispensing  
- The proximity sensor checks if the bottle is correctly positioned.  
- If the bottle is placed at the correct distance, dispensing begins and **STATUS_LED turns 🟢 (GREEN)**.  
- If the bottle is removed during dispensing:  
  - **STATUS_LED turns 🟡 (YELLOW)**.  
  - The system remembers the dispensing level and resumes when the bottle is repositioned.  

#### 4. Process Completion  
- **Scenario 1:**  
  - Water has been successfully dispensed.  
  - The display will show **"Water dispensing finished"**, and the buzzer will sound.  
  - The system returns to **step 1**.  
- **Scenario 2:**  
  - The user presses the **STOP** button.  
  - The system immediately halts, regardless of the current state.  
  - The display will show **"Emergency! Rebooting..."**, and **STATUS_LED turns 🔴 (RED) for a few seconds**.  
  - The system returns to **step 1**.  



## 🔧 Things to Improve  
- [ ] Add more beverages
- [ ] Add temperature options
- [ ] Show in the display the live percentage of water erogated 
- [ ] Notify users of leaks or maintenance needs, for example when dispenser need to be refilled

## 🎥 Presentation and YouTube video

## 🧑‍🧑‍🧒‍🧒 Team members
- [Giulia Garonzi](https://github.com/GiuliaGa03): designed step 3, built the dispenser, made pitch video and presentation
- [Giulio Gualtiero](https://github.com/GiulioGualtiero): designed step 1 and 2, built the dispenser, overviewed cable management and sensors interaction
- [Jago Revrenna](https://github.com/renna3): designed step 1 and 2, built the dispenser, managed code organization and work direction
- [Elena Rubbo](https://github.com/elerub22): designed step 4, built the dispenser, made pitch video and presentation

 

