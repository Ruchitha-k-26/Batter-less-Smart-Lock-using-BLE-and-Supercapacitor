# Batter-less-Smart-Lock-using-BLE-and-Supercapacitor
Bluetooth-enabled batteryless smart locker using CC2640R5, powered by a supercapacitor and solar panel.
## 🔋 Features
- Batteryless design using 1.5F supercapacitor and 9V solar panel
- BLE communication using CC2640R5 and MIT App Inventor app
- Checksum validation for secure unlock
- Automatic relock using timeout
- Low-power embedded system with GPIO control
- NVS and ADC integration for future expansion

## 🛠️ Hardware Used
- TI CC2640R5 LaunchPad
- Electromechanical solenoid lock (8–24V)
- LM317 voltage regulator with Schottky diode
- 9V solar panel
- 1.5F, 6.1V Supercapacitor

## 📲 Software Stack
- Code Composer Studio 12.1
- SimpleLink SDK for CC2640
- MIT App Inventor (BLE scanner app)
- Flash Programmer 2

## 🧪 How It Works
1. Supercapacitor is charged by solar panel using LM317
2. BLE command sent via mobile app to write data to Characteristic 5
3. Checksum is validated, and if correct:
    - Characteristic 4 sends notification
    - GPIO pin energizes lock for 0.5s
4. After a timeout (500ms), the lock resets automatically

## 🚀 Future Scope
- Biometric login in app
- Cloud sync and logging
- Tamper alerts
- Multi-user locker access
