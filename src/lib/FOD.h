/**
 * @class DPL
 * @brief Class that manages ballon's deployment
 * @author: Matías Vidal
 */

/*Requiered Libraries*/
#include <Arduino.h>
#include "fod_pines.h"

class FOD {
    /*Private Members*/

    // DLP Objects

    // Internal Variables
public:
    /*Public Members*/
    bool deployed;
    unsigned long t0, dt;

    /*Base contructor (null)*/
    DPL() {}

    // methods
    void init(void);
    void deploy(void);
    bool status(void);

// private:
    // methods
};
