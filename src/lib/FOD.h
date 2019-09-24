/**
 * @class FOD
 * @brief Class that manages the femto-satellite deployment
 * @author: Matías Vidal
 */

/*Requiered Libraries*/
#include <Arduino.h>
#include "fod_pins.h"
#include "gps_data.h"

class FOD {
    /*Private Members*/

    // FOD Objects

    // Internal Variables
public:
    /*Public Members*/
    GpsData gpsData;

    bool deployed;
    unsigned long t0, dt;
    float version;
    char lat[12], lng[12], alt[12];
    int hour, min, sec, sats, on_time, attempts;

    /*Base contructor (null)*/
    FOD() {}

    // methods
    void init(void);
    void deploy(void);
    bool status(void);
    void updateData(char data[]);
    void setOnTime(char data[]);

// private:
    // methods
};
