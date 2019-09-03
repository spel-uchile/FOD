/**
 * @brief Femto-satellite v2.0
 */
 
/*Author: Matias Vidal*/

#include "FOD.h"
#include "radio.h"
#include "cmds.h"
#include <Wire.h>

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define I2C_ADDRESS 7

/*Object Definitions*/
FOD fod;
Radio radio(RADIO_SLAVESELECTPIN, RADIO_INTERRUPT, RADIO_RST, CLIENT_ADDRESS, SERVER_ADDRESS);

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

char cmd = '*';
String serial_data;
char data[100];

void setup() {
    // Initialize serial communication
    SerialUSB.begin(115200);
    SerialUSB.setTimeout(200);
    while (!SerialUSB) {
        ; // wait for serial port to connect.
    }
    // Initialize femto-satellite's systems
    SerialUSB.println("Initializing FOD");
    fod.init();
    radio.init();
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveHandler);
    Wire.onRequest(requestHandler);
    SerialUSB.println("Done.");
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    gps.updateData();
    while (SerialUSB.available()) {
        cmd = SerialUSB.read() - 48;
        SerialUSB.print("Received: ");
        SerialUSB.println((char)(cmd+48));    
    	//base_cmd = radio.read_command(); 
    	if (cmd == SEND_FEMTOSAT_DATA) {
    	    radio.updateBeacon(&gps.gpsData);
    	    radio.send_data();
    	}
    	else if (cmd == SEND_PICTURE) {
    	    
    	}
    	else if (cmd == SEND_BEACON) {
    	    radio.updateBeacon(&gps.gpsData);
    	    radio.send_data();
    	}
    	else if (cmd == PING) {
    	    radio.ack_ping();
    	}
    	else if (cmd == ENABLE_LOW_POWER_MODE) {
    	    lowPowerMode();
    	}
    	else if (cmd == DISABLE_LOW_POWER_MODE) {
    	    normalMode();
    	}
    	else if (cmd == DEPLOY_FEMTOSATS) {
    	    fod.deploy();
	    SerialUSB.print("Deployment complete at ");
    	    SerialUSB.print(fod.dt);
    	    SerialUSB.println(" milliseconds.");
    	}
    	else if (cmd == HELP) {
    	    help();
    	}
	else {
            SerialUSB.print((char)(cmd+48));
            SerialUSB.println(" is not available yet");
        }
    }
    cmd = '*';
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
	if (cmd == DEPLOY_FEMTOSATS) {
            fod.deploy();
	    SerialUSB.print("Deployment complete at ");
    	    SerialUSB.print(fod.dt);
    	    SerialUSB.println(" milliseconds.");
        }
	else if (cmd == SEND_BEACON) {
            radio.updateBeacon(&gps.gpsData);
            radio.send_data();
    	}
	else if (cmd == PING) {
            radio.ack_ping();
    	}
	else if (cmd == ENABLE_LOW_POWER_MODE) {
            lowPowerMode();
    	}
	else if (cmd == DISABLE_LOW_POWER_MODE) {
            normalMode();
    	}
        SerialUSB.print(cmd);
        cmd = '*';
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

/**
 * Enables the low power mode of operation
 * of the FOD.
 * Currently it only puts the radio to sleep,
 * but it is expected to also put the
 * microcontroller into this state.
 * @return True if succesfull.
 */
bool lowPowerMode() {
    return radio.lowPowerMode();
}

/**
 * Enables the normal mode of operation.
 */
void normalMode() {
    radio.normalMode();
}

/**
 * This function displays all the FOD's commands.
 * These commands are the same for both the user
 * (via serial) and the OBC (via I2C).
 */
void help() {
    SerialUSB.println(F("Available commands:"));
    SerialUSB.print(SEND_FOD_STATUS);
    SerialUSB.println(F(": SEND_FOD_STATUS"));
    SerialUSB.print(DEPLOY_FEMTOSATS);
    SerialUSB.println(F(": DEPLOY_FEMTOSATS"));
    SerialUSB.print(SEND_FEMTOSAT_DATA);
    SerialUSB.println(F(": SEND_FEMTOSAT_DATA"));
    SerialUSB.print(SEND_PICTURE);
    SerialUSB.println(F(": SEND_PICTURE"));
    SerialUSB.print(SEND_BEACON);
    SerialUSB.println(F(": SEND_BEACON"));
    SerialUSB.print(PING);
    SerialUSB.println(F(": PING"));
    SerialUSB.print(ENABLE_LOW_POWER_MODE);
    SerialUSB.println(F(": ENABLE_LOW_POWER_MODE"));
    SerialUSB.print(DISABLE_LOW_POWER_MODE);
    SerialUSB.println(F(": DISABLE_LOW_POWER_MODE"));
    SerialUSB.print(HELP);
    SerialUSB.println(F(": HELP"));
}
