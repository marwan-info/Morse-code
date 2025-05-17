# 🔦 STM32 FreeRTOS Morse Code Transceiver (IR-Based)

A real-time embedded system project using **STM32**, **FreeRTOS**, and **Infrared (IR)** communication to transmit and receive Morse code using physical buttons and GPIO signaling.

---

## 📋 Project Overview

This project implements a **Morse code communication system** where:
- Users can send **dots** and **dashes** using buttons.
- Morse signals are transmitted via **IR** (Infrared LED).
- Signals are received and decoded by a **photodiode (IR receiver)**.
- Visual feedback is provided using **LEDs** for DOT and DASH recognition.

Implemented using:
- **STM32 microcontroller**
- **FreeRTOS** for task scheduling
- **HAL (Hardware Abstraction Layer)** for GPIO control

---

## ⚙️ Hardware Specifications

| Component           | Description                          |
|---------------------|--------------------------------------|
| MCU                 | STM32 (e.g., STM32F103C8T6 Blue Pill)|
| IR Transmitter      | Connected to `PB6`                   |
| IR Receiver         | Connected to `PB5`                   |
| DOT Button          | Connected to `PB14`                  |
| DASH Button         | Connected to `PB15`                  |
| DOT LED Indicator   | Connected to `PA8`                   |
| DASH LED Indicator  | Connected to `PB12`                  |

---

## 🔧 Hardware Specifications

### ✅ STM32 "Blue Pill" Development Board  
**MCU:** STM32F103C8T6 (ARM Cortex-M3, 72 MHz)

This project uses the STM32 Blue Pill board to implement Morse code transmission and reception via GPIO and IR, under FreeRTOS.

| **Feature**          | **Usage in Project**                                           |
|----------------------|----------------------------------------------------------------|
| **Core**             | (Single-core) ARM Cortex-M3 @ 72 MHz – runs FreeRTOS tasks     |
| **GPIO**             | Used for buttons (dot/dash), LEDs, IR TX/RX                    |
| **Timers (SysTick)** | Used by FreeRTOS for task timing and delays                    |
| **SRAM**             | 20 KB – stores stack and task data                             |
| **Flash**            | 64 KB – holds the compiled firmware                            |
| **IR Communication** | GPIO-based IR send/receive without UART                        |
| **FreeRTOS**         | Manages concurrent Sender and Receiver tasks                   |
| **CMSIS**            | Used as the RTOS abstraction and startup layer                 |
| **CubeMX (HAL)**     | Used to generate initialization and HAL drivers                |
| **Debugger**         | Compatible with ST-Link v2 over SWD                            |


## 🧠 Features

- **Debounced Button Input** for stable transmission
- **FreeRTOS Multi-Tasking**:
  - `SenderTask`: Reads button inputs and sends Morse via IR
  - `ReceiverTask`: Reads IR input and displays decoded signal via LEDs
- **IR Pulse Timing**:
  - DOT: 300 ms
  - DASH: 3000 ms
- **Debounce Delay**: 20 ms

---

## 🧪 Functional Behavior

### Sending Signals
- Press **DOT button (PB14)** → Transmit short IR pulse
- Press **DASH button (PB15)** → Transmit long IR pulse

### Receiving Signals
- IR receiver samples the signal.
- Distinguishes between DOT and DASH by timing the active-low pulse.
- Turns on:
  - **PA8 LED** for DOT
  - **PB12 LED** for DASH

---

## 🚦 GPIO Pin Mapping

| GPIO      | Function         | Mode       |
|-----------|------------------|------------|
| `PB14`    | DOT Button       | Input      |
| `PB15`    | DASH Button      | Input      |
| `PB6`     | IR Transmitter   | Output     |
| `PB5`     | IR Receiver      | Input      |
| `PA8`     | DOT LED          | Output     |
| `PB12`    | DASH LED         | Output     |

---

## 🧩 Software Stack

- **STM32CubeMX** (initial setup)
- **STM32 HAL** for GPIO abstraction
- **FreeRTOS** for task scheduling
- **C Language**
- **Keil MDK / STM32CubeIDE** (recommended IDEs)

---

## 🛠️ Building & Flashing

1. Generate code using **STM32CubeMX** with FreeRTOS middleware enabled.
2. Compile with **STM32CubeIDE** or **Keil**.
3. Flash to STM32 board using **ST-Link** or other supported programmer.
4. Open serial terminal (optional for debugging).
5. Press buttons and observe LED output corresponding to received IR signals.

---

## 🚀 Future Improvements

- Add **USART output** for debugging decoded Morse code.
- Implement **multi-character** Morse decoding with timing windows.
- Add **buzzer** for audio feedback.
- Extend project to support **wireless long-distance IR** transmission.

---

## 📖 Technical Concepts Used

### 🧵 RTOS (Real-Time Operating System)

**FreeRTOS** is an open-source RTOS used to manage multiple tasks concurrently. It offers:
- Preemptive or cooperative multitasking
- Task prioritization
- Deterministic timing (ideal for real-time control)

**In this project**, FreeRTOS runs two concurrent tasks:
- `SenderTask` handles button inputs and transmits IR signals.
- `ReceiverTask` samples the IR receiver and processes the signal.

### 🧠 CMSIS (Cortex Microcontroller Software Interface Standard)

**CMSIS** is a hardware abstraction layer for ARM Cortex-M cores provided by ARM. It includes:
- Core access headers and startup code
- Interfaces for RTOS integration
- Standard APIs for peripheral drivers

**In this project**, CMSIS ensures consistent access to hardware registers and RTOS threads using:
```c
#include "cmsis_os.h"
```


## 🧾 License

This project is licensed under the terms described in the LICENSE file provided by STMicroelectronics. If no license file is present, the software is provided **AS-IS**.

---

## 📚 References

- [FreeRTOS Official Documentation](https://www.freertos.org/)
- [STM32 HAL Library Guide](https://www.st.com/en/embedded-software/stm32cubefw.html)
- [Infrared Communication Basics](https://www.electronics-tutorials.ws/blog/infrared-ir-communication.html)
- [Morse Code Standard](https://en.wikipedia.org/wiki/Morse_code)
- [STM32CubeMX Documentation](https://www.st.com/en/development-tools/stm32cubemx.html)

---

> Developed as part of a real-time embedded systems learning project.
