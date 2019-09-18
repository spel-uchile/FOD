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

char buf[100];
char params[100];
int cmd = 0;

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
    if (SerialUSB.available()) {
	int i = 0;
    	while (SerialUSB.available()) {
    	    buf[i] = SerialUSB.read();
	    i++;
	}
	buf[i] = 0;
	sscanf(buf, "%d %[^\n]s", &cmd, &params);
    	executeCommands(cmd, params);
    }
    delay(10);
}

/**
 * I2C comunication handler between the FOD
 * and the OBC. This is used when the master
 * sends a command or instruction to the FOD.
 * @param Number of bytes read from the OBC.
 */
void receiveHandler(int numBytes) {
    int i = 0;
    while (Wire.available()) {
	buf[i] = Wire.read();
	i++;
    }
    buf[i] = 0;
    sscanf(buf, "%d %[^\n]s", &cmd, &params);
    executeCommands(cmd, params); 
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
    SerialUSB.println("Low power mode selected");
    return radio.lowPowerMode();
}

/**
 * Enables the normal mode of operation.
 */
void normalMode() {
    SerialUSB.println("Normal mode selected");
    radio.normalMode();
}

/**
 * This function displays all the FOD's commands.
 * These commands are the same for both the user
 * (via serial) and the OBC (via I2C).
 */
void help() {
    SerialUSB.println(F("Available commands:"));
    SerialUSB.print(HELP);
    SerialUSB.println(F(": HELP"));
    SerialUSB.print(DEPLOY_FEMTOSATS);
    SerialUSB.println(F(": DEPLOY_FEMTOSATS"));
    SerialUSB.print(FOD_GET_STATUS);
    SerialUSB.println(F(": FOD_GET_STATUS"));
    SerialUSB.print(SEND_FEMTOSAT_DATA);
    SerialUSB.println(F(": SEND_FEMTOSAT_DATA"));
    SerialUSB.print(SET_ON_TIME);
    SerialUSB.println(F(": SET_ON_TIME"));
    SerialUSB.print(SEND_BEACON);
    SerialUSB.println(F(": SEND_BEACON"));
    SerialUSB.print(UPDATE_DATA);
    SerialUSB.println(F(": UPDATE_DATA"));
    SerialUSB.print(ENABLE_LOW_POWER_MODE);
    SerialUSB.println(F(": ENABLE_LOW_POWER_MODE"));
    SerialUSB.print(DISABLE_LOW_POWER_MODE);
    SerialUSB.println(F(": DISABLE_LOW_POWER_MODE"));
    SerialUSB.print(GET_VERSION);
    SerialUSB.println(F(": GET_VERSION"));
}

void executeCommand(char cmd, char params[]) {
    SerialUSB.print("Received: ");
    SerialUSB.print(cmd);
    SerialUSB.print("   Command: ");
    if (cmd == SEND_FEMTOSAT_DATA) {
    	SerialUSB.println("SEND_FEMTOSAT_DATA");
    	radio.updateBeacon(&fod.gpsData);
    	radio.send_data();
    }
    else if (cmd == FOD_GET_STATUS) {
	SerialUSB.println("FOD_GET_STATUS");
        SerialUSB.print("Released: ");
        SerialUSB.println(fod.status());    
    }
    else if (cmd == SET_ON_TIME) {
        SerialUSB.print("SET_ON_TIME");
	SerialUSB.print("   Parameters: ");
	SerialUSB.println(params);
        fod.setOnTime(params);
        SerialUSB.print("New on time: ");
        SerialUSB.print(fod.on_time);
        SerialUSB.println(" ms.");
    }
    else if (cmd == SEND_BEACON) {
	SerialUSB.println("SEND_BEACON");
    	radio.updateBeacon(&fod.gpsData);
    	radio.send_data();
    }
    else if (cmd == UPDATE_DATA) {
        SerialUSB.print("UPDATE_DATA");
	SerialUSB.print("   Parameters: ");
	SerialUSB.println(params);
        fod.updateData(data);
        SerialUSB.print("Data: ");
        SerialUSB.print("HH:MM:SS: ");
        SerialUSB.print(fod.gpsData.hour);
        SerialUSB.print(":");
        SerialUSB.print(fod.gpsData.minute);
        SerialUSB.print(":");
        SerialUSB.print(fod.gpsData.second);
        SerialUSB.print("    Sat: ");
        SerialUSB.print(fod.gpsData.satellites);
        SerialUSB.print("    Location: ");
        SerialUSB.print(fod.gpsData.latitude, 6);
        SerialUSB.print(",");
        SerialUSB.print(fod.gpsData.longitude, 6);
        SerialUSB.print("    Altitude (GPS): ");
        SerialUSB.println(fod.gpsData.altitude, 6);
    }
    else if (cmd == ENABLE_LOW_POWER_MODE) {
    	SerialUSB.println("ENABLE_LOW_POWER_MODE");
    	lowPowerMode();
    }
    else if (cmd == DISABLE_LOW_POWER_MODE) {
	SerialUSB.println("DISABLE_LOW_POWER_MODE");
    	normalMode();
    }
    else if (cmd == DEPLOY_FEMTOSATS) {
    	SerialUSB.println("DEPLOY_FEMTOSATS");
    	fod.deploy();
	SerialUSB.print("Deployment complete at ");
    	SerialUSB.print(fod.dt);
    	SerialUSB.println(" ms.");
    }
    else if (cmd == HELP) {
	SerialUSB.println("HELP");
    	help();
    }
    else if (cmd == GET_VERSION) {
        SerialUSB.println("GET_VERSION");
        SerialUSB.print("Current version is: ");
        SerialUSB.println(fod.version);
    }
    else {
        SerialUSB.print(cmd);
        SerialUSB.println(" is not available yet");
    }
    cmd = 0;
}
