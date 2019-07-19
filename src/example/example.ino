/**
 * @brief Femto-satellite v2.0
 */
 
/*Author: Matias Vidal*/

#include "FOD.h"
#include "radio.h"
#include "gps.h"
#include "cmds.h"

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define I2C_ADDRESS 7

/*Object Definitions*/
FOD fod;
Radio radio(RADIO_SLAVESELECTPIN, RADIO_INTERRUPT, RADIO_RST, CLIENT_ADDRESS, SERVER_ADDRESS);
GPS gps;

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

uint8_t base_cmd;
char cmd;

void setup() {
    // Initialize serial communication
    SerialUSB.begin(115200);
    while (!SerialUSB) {
        ; // wait for serial port to connect.
    }
    // Initialize femto-satellite's systems
    SerialUSB.println("Initialize FOD");
    fod.init();
    gps.init();
    radio.init();
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveHandler);
    Wire.onRequest(requestHandler);
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    gps.updateData();
    base_cmd = radio.read_command(); 
    if (base_cmd == SEND_FEMTOSAT_DATA) {
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
        fod.deploy();
    }
    base_cmd = 0;
    delay(100);
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
            fod.deploy();
	    SerialUSB.print("Deployment complete at ");
    	    SerialUSB.print(fod.dt);
    	    SerialUSB.println(" milliseconds.");
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
    Wire.write(fod.status());
    SerialUSB.println("Sent status");
}
