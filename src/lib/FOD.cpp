#include "FOD.h"

bool deployed;
unsigned long t0, dt;

//-------------------------- Public Methods --------------------------
/**
 * Initialize all the pins of the deployment
 * system.
 */
void FOD::init(void) {
    pinMode(DPL_EN, OUTPUT);
    digitalWrite(DPL_EN, LOW);
    pinMode(DPL_STATUS, INPUT);
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
    while (!deployed && dt < 3000) {
	delay(100);
	deployed = status();
	dt = millis() - t0;
    }	    
    digitalWrite(DPL_EN, LOW);
}

/**
 * Returns the status of the deployment system
 * @return True if the femto-satellite was
 *  	   successfully deployed.
 */
bool FOD::status(void) {
    return digitalRead(DPL_STATUS);
}
