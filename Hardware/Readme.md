# Hardware Setup

## Components Used

- **Microcontroller**: ESP32 DevKit or any ESP32 module
- **OLED Display**: SSD1306 (128x64 pixels)
- **Temperature and Humidity Sensor**: DHT22
- **Light Dependent Resistors (LDR)**: 2x
- **Push Buttons**: 4x (PB_CANCEL, PB_OK, PB_UP, PB_DOWN)
- **Buzzer**: For audio alerts
- **LED**: For visual alerts
- **Servo Motor**: SG90
- **WiFi Module**: ESP32 internal WiFi

## Hardware Connections

### ESP32 Pin Connections

- **OLED Display**
  - SDA to pin 21 (or SDA pin in ESP32)
  - SCL to pin 22 (or SCL pin in ESP32)
  - RST to pin -1 (or any digital pin, set in code)
  - VCC to 3.3V
  - GND to GND

- **DHT22**
  - Data pin to pin 17 (DHTPIN in code)
  - VCC to 3.3V
  - GND to GND

- **LDRs**
  - Left LDR to pin 32 (Left_LDR in code)
  - Right LDR to pin 33 (Right_LDR in code)
  - Both VCC to 3.3V
  - Both GND to GND

- **Push Buttons**
  - PB_CANCEL to pin 18
  - PB_OK to pin 34
  - PB_UP to pin 35
  - PB_DOWN to pin 12
  - All buttons GND to GND
  - All buttons with pull-up resistor to 3.3V

- **Buzzer**
  - Positive to pin 16 (BUZZER in code)
  - Negative to GND

- **LED**
  - Anode (longer pin) to pin 5 (LED_1 in code)
  - Cathode (shorter pin) to GND

- **Servo Motor**
  - Signal to pin 25 (servoPin in code)
  - VCC to 5V
  - GND to GND

### Additional Components

- External Power Supply: Required for the servo motor, usually 5V


- <span style="color:red">**Adjust Pin Definitions**: Ensure the pin definitions in the code match your actual wiring.</span>


## Usage

1. **Power Up**: Connect the ESP32 to power.
2. **Initialization**: The Medibox will initialize and connect to WiFi.
3. **Main Operation**:
   - The OLED will display the main screen.
   - Buttons are used to navigate through menus and set time/alarm settings.
   - Alerts for temperature, humidity, and alarms are displayed on the OLED, LED, and sound buzzer.
   - The servo motor adjusts based on ambient light levels.
