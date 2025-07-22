# SPI-I2C-Arduino-MSP430
An SPI and I2C (multi slave, single master) communication based character driver for Arduino MSP430 

This repository demonstrates SPI communication between two MSP430 microcontrollers (G2553 as **Master**, G2231 as **Slave**) using a 3-wire SPI interface. UART is also used on the master side for user input via serial terminal.

---

## 🧰 Components Used

- **MSP430G2553** – SPI Master + UART Interface
- **MSP430G2231** – SPI Slave
- TI LaunchPad v1.5 boards
- Serial terminal (e.g., PuTTY, TeraTerm, minicom)

---

## 📁 File Descriptions

| File Name           | Description                                      |
|---------------------|--------------------------------------------------|
| `SPI_2553.c`        | Master code: Handles UART commands and sends data over SPI. |
| `SPI_2553 (1).c`    | Alternate version of master code (same functionality).       |
| `SPI_2231.c`        | Slave code: Receives SPI data and controls LED.  |
| `uart_simple_echo.c`| Minimal UART echo test for G2553.                |
| `UART-2553.c`       | UART-based command parser + LED control for G2553.           |

---

## ⚙️ How It Works

### SPI Master (G2553)

- Initializes UART (9600 baud) and SPI in master mode.
- Waits for user input over UART:
  - `"1"` → sends `0x31` (ASCII `'1'`) over SPI.
  - `"0"` → sends `0x30` (ASCII `'0'`) over SPI.
- Commands are echoed back to the terminal.
- Can also interpret `"ver"` and `"h"` for version/help.

### SPI Slave (G2231)

- Configured as SPI slave using USI module.
- Waits for SPI data from master:
  - `0x31` → turns **Red LED (P1.0)** **ON**.
  - `0x30` → turns LED **OFF**.
- Echoes back the received character.

---

## 🔌 Wiring (3-Wire SPI)

| Signal  | G2553 (Master) | G2231 (Slave) |
|---------|----------------|---------------|
| SCLK    | P1.5           | P1.5          |
| MOSI    | P1.7           | P1.7          |
| MISO    | P1.6           | P1.6          |
| GND     | GND            | GND           |
| VCC     | VCC            | VCC           |

> Note: Ensure both LaunchPads share a **common ground** and **3.3V supply**.

---

## 🧪 Testing Steps

1. Flash `SPI_2553.c` onto the G2553 board.
2. Flash `SPI_2231.c` onto the G2231 board.
3. Open a terminal (9600 baud, 8-N-1) to the G2553 UART.
4. Type `1` or `0` and observe the red LED on the slave.
5. Use `h` or `ver` for help and version info.

---

## ✅ Expected Behavior

| UART Input | Slave LED (P1.0) | SPI Response |
|------------|------------------|--------------|
| `1`        | ON               | Echo `'1'`   |
| `0`        | OFF              | Echo `'0'`   |
| `h`        | Help text        | —            |
| `ver`      | Version info     | —            |

---

## 🛠️ Tools Used

- **Code Composer Studio (CCS)**
- **TI LaunchPad**
- **MSP430G2xx devices**
- **MSP430 USB drivers**
- Serial terminal
