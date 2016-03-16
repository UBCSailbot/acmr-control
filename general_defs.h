//
// Created by Bryan Luu on 2016-02-18.
//

#ifndef ACMR_CONTROL_SCHEMES_H
#define ACMR_CONTROL_SCHEMES_H

enum SteeringScheme { SailByCOG, SailByAWA, SailByHOG, SailByRudder};

enum ThrustScheme { ThrustPower, PropulsionPower, FixedSpeed };

#define PRINT_DELAY (0.5)
#define SCU_PUBLISH_PERIOD (0.1) // in secs
#define TCU_PUBLISH_PERIOD (0.1)
//#define DONTGO_ANGLE (170)
//#define FULL_DEFLECTION (15)

// ------- GLOBAL CONSTANTS -------
const int NO_GO_WIND_ANGLE = 24;               // degrees (half of the total no go zone)
const int DONT_GO_WING_ANGLE = 170;
const int UPWIND_HYSTERESIS_ANGLE = 5;
const int MAX_DEFLECTION = 15;


#endif //ACMR_CONTROL_SCHEMES_H
