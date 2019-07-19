/**
 * @class FOD
 * @brief Class that manages the femto-satellite deployment
 * @author: Matías Vidal
 */

/*Requiered Libraries*/
#include <Arduino.h>
#include "fod_pins.h"

class FOD {
    /*Private Members*/

    // FOD Objects

    // Internal Variables
public:
    /*Public Members*/
    bool deployed;
    unsigned long t0, dt;

    /*Base contructor (null)*/
    FOD() {}

    // methods
    void init(void);
    void deploy(void);
    bool status(void);

// private:
    // methods
};
