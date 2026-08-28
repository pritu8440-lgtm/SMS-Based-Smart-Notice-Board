# SMS-Based Smart Notice Board

An Arduino-based Smart Notice Board that displays messages on a MAX7219 LED Matrix. The system uses an RTC module for real-time clock information and is designed to receive notices through GSM SMS.

## Project Overview

The Smart Notice Board automatically displays:

- Time from the RTC module
- Good Morning / Good Afternoon / Good Evening greetings
- College welcome message
- Messages received through SMS
- Test messages entered through the Serial Monitor during Wokwi simulation

The main objective is to provide a simple wireless notice display system where an authorized user can send a message and the message can be displayed on an LED matrix.

## Features

- 📱 SMS-based notice display using GSM
- 🕒 Real-time clock using DS1307 RTC
- 💡 4-module MAX7219 LED Matrix display
- 🔄 Scrolling text display
- 👋 Automatic time-based greetings
- 🏫 Default college welcome message
- 🧪 Serial Monitor input for Wokwi testing
- 🔌 Arduino UNO based system
- 🖥️ Wokwi simulation support

## Hardware Components

| Component | Quantity | Purpose |
|---|---:|---|
| Arduino UNO | 1 | Main controller |
| MAX7219 LED Matrix | 4 modules | Notice/message display |
| DS1307 RTC | 1 | Real-time clock |
| GSM/SIM900 Module | 1 | SMS communication |
| SIM Card | 1 | SMS reception |
| Jumper Wires | As required | Connections |
| Power Supply | As required | Circuit power |

## Pin Connections

### DS1307 RTC → Arduino UNO

| RTC Pin | Arduino UNO |
|---|---|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

### MAX7219 LED Matrix → Arduino UNO

| Matrix Pin | Arduino UNO |
|---|---|
| VCC | 5V |
| GND | GND |
| DIN | D11 |
| CS | D10 |
| CLK | D13 |

### GSM/SIM900 → Arduino UNO

| GSM Pin | Arduino UNO |
|---|---|
| TX | D7 |
| RX | D8 |
| GND | GND |

> Note: During Wokwi simulation, the Serial Monitor is used to simulate SMS input because the project can be tested without a physical GSM network.

## Working Principle

The system works in the following sequence:

1. Arduino UNO initializes the LED Matrix.
2. The DS1307 RTC provides the current time.
3. The system displays a time-based greeting.
4. The current time is displayed on the LED Matrix.
5. `WELCOME TO COLLEGE` is displayed as the default notice.
6. When an SMS is received through the GSM module, the message is extracted.
7. The received message is displayed as scrolling text on the LED Matrix.
8. After displaying the message, the normal time/greeting display resumes.

### System Flow

```text
             SMS
              |
              v
       GSM / SIM900 Module
              |
              v
          Arduino UNO
          /         \
         /           \
        v             v
      RTC         LED Matrix
       |               |
       v               v
   Time/Greeting    SMS Notice

   ## Wokwi Simulation

▶️ [Run the SMS-Based Smart Notice Board on Wokwi](https://wokwi.com/projects/473572606485640193)

The simulation includes:
- Arduino Uno
- DS1307 RTC
- MAX7219 LED Matrix
- SMS/notice display logic
- Real-time clock and greeting display

You can start the simulation and test the notice board directly in your browser.