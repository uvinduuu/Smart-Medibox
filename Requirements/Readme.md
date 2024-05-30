# Embedded Systems and Applications Programming Assignments

## Assignment 1: Medibox Simulation

### Project Overview
This project simulates a Medibox using an ESP32 microcontroller. The Medibox assists users in managing their medication schedules effectively. It includes features like setting alarms, fetching time from an NTP server, monitoring temperature and humidity, and providing warnings.

### Requirements
1. **Fetching Current Time and Displaying on OLED**
   - Fetches time from NTP server.
   - Displays current time on an OLED screen.

2. **Menu**
   - Allows user to set time zone, set 3 alarms, and disable all alarms.

3. **Alarms**
   - Rings alarms at set times with proper indications.
   - Stops alarms using a push button.

4. **Monitoring**
   - Monitors temperature and humidity.
   - Provides warnings if temperature or humidity exceed healthy limits.


## Assignment 2: Medibox Enhancement

### Project Overview
This assignment enhances the Medibox by adding features like light intensity monitoring, dynamic regulation of light with a shaded sliding window using a servo motor, and user-adjustable parameters via a Node-RED dashboard.

### Requirements
1. **Light Intensity Monitoring**
   - Uses two LDRs to monitor light intensity.
   - Sends data to Node-RED via MQTT.

2. **Shaded Sliding Window**
   - Uses a servo motor to adjust the window.
   - Dynamically regulates light based on a given equation.

3. **Node-RED Dashboard**
   - Visualizes light intensity with a plot and gauge.
   - Sets up sliders and dropdowns for user-adjustable parameters.
