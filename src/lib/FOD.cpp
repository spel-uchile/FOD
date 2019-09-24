#include "FOD.h"

bool deployed;
unsigned long t0, dt;
float version;
char lat[12], lng[12], alt[12];
int hour, min, sec, sats, on_time, attempts;

//-------------------------- Public Methods --------------------------
/**
 * Initialize all the pins of the deployment
 * system.
 */
void FOD::init(void) {
    pinMode(DPL_EN, OUTPUT);
    digitalWrite(DPL_EN, LOW);
    pinMode(DPL_STATUS, INPUT);
    on_time = 1500;
    version = 2.0;
    attempts = 0;
}

/**
 * Deploy the femto-satellites.
 * @return Time required to cut the wire to
 * 	   deploy the femto-satellite in
 * 	   milliseconds.
 */
void FOD::deploy(void) {
    t0 = millis();
    dt = 0;
    digitalWrite(DPL_EN, HIGH);
    deployed = status();
    while (!deployed && dt < on_time) {
	delay(10);
	deployed = status();
	dt = millis() - t0;
    }	    
    digitalWrite(DPL_EN, LOW);
    attempts++;
}

/**
 * Returns the status of the deployment system
 * @return True if the femto-satellite was
 *  	   successfully deployed.
 */
bool FOD::status(void) {
    return digitalRead(DPL_STATUS);
}

/**
 * Updates the time, location and number of GPS satellites
 * connected.
 * @param data: A char array with the new data.
 */
void FOD::updateData(char data[]) {
    sscanf(data, "%d %d %d %d %s %s %s",
           &hour, &min, &sec, &sats, &lat, &lng, &alt);
    gpsData.hour = hour;
    gpsData.minute = min;
    gpsData.second = sec;
    gpsData.satellites = sats;
    gpsData.latitude = ((String) lat).toFloat();
    gpsData.longitude = ((String) lng).toFloat();
    gpsData.altitude = ((String) alt).toFloat();
}

/**
 * Sets the duration of the deployment attempt in milliseconds.
 * @param data: A char array containing an integer number
 *              with the time in milliseconds.
 */
void FOD::setOnTime(char data[]) {
    sscanf(data, "%d", &on_time);
}
