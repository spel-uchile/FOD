/**
 * @brief Femto-satellite v2.0
 */
 
/*Author: Matias Vidal*/

#include "radio.h"
#include "gps.h"
#include "fod_pins.h"

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define DEPLOY_FEMTOSATS 7

/*Object Definitions*/
Radio radio(RADIO_SLAVESELECTPIN, RADIO_INTERRUPT, RADIO_RST, CLIENT_ADDRESS, SERVER_ADDRESS);
GPS gps;

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

uint8_t base_cmd;

void setup() {
    // Initialize serial communication
    SerialUSB.begin(115200);
    while (!SerialUSB) {
        ; // wait for serial port to connect.
    }
    // Initialize femto-satellite's systems
    SerialUSB.println("Initialize FOD");
    pinMode(DPL_STATUS, INPUT);
    pinMode(DPL_EN, OUTPUT);
    gps.init();
    radio.init();
    Wire.begin(7);
    Wire.onReceive(receiveHandler);
    Wire.onRequest(requestHandler);
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    gps.updateData();
    base_cmd = radio.read_command(); 
    if (base_cmd == SEND_DATA) {
      	radio.updateBeacon(&gps.gpsData);
      	radio.send_data();
    }
    else if (base_cmd == SEND_PICTURE) {
        
    }
    else if (base_cmd == SEND_BEACON) {
        radio.updateBeacon(&gps.gpsData);
        radio.send_data();
    }
    else if (base_cmd == PING) {
        radio.ack_ping();
    }
    else if (base_cmd == ENABLE_LOW_POWER_MODE) {
        
    }
    else if (base_cmd == DISABLE_LOW_POWER_MODE) {
        
    }
    else if (base_cmd == DEPLOY_FEMTOSATS) {
        deployFemtosats();
    }
    base_cmd = 0;
    delay(100);
}

void deployFemtosats() {
    deploy = true;
    int t0 = millis();
    int dt = 0;
    SerialUSB.println("Initiating the femto-satellite deployment system");
    digitalWrite(DPL_EN, HIGH);
    while (deploy && (dt < 5000)) {
        dt = millis() - t0;
        deploy = digitalRead(DPL_STATUS);
        delay(100); 
    }
    digitalWrite(DPL_EN, LOW);
    SerialUSB.print("Deployment complete at ");
    SerialUSB.print(dt);
    SerialUSB.println(" milliseconds");
}

/**
 * I2C comunication handler between the FOD
 * and the OBC. This is used when the master
 * sends a command or instruction to the FOD.
 * @param Number of bytes read from the OBC.
 */
void receiveHandler(int numBytes) {
    while (Wire.available()) {
        cmd = Wire.read();
	if (cmd == 'D') {
            FOD.deploy();
	    SerialUSB.print("Deployment complete at ");
    	    SerialUSB.print(FOD.dt);
    	    SerialUSB.println(" milliseconds");
        }
        SerialUSB.print(cmd);
        cmd = 0;
    } 
}

/**
 * I2C request handler. This function is used
 * when the master asks the FOD to send data
 * back to him.
 */
void requestHandler() {
    Wire.write(FOD.status());
    SerialUSB.println("Sent status");
}
