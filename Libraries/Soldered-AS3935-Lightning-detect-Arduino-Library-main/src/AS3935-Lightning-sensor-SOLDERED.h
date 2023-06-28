/**
 **************************************************
 *
 * @file        AS3935-Lightning-sensor-SOLDERED.h
 * @brief       Header file for AS3935 sensor library.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Goran Juric @ soldered.com
 ***************************************************/

#ifndef __AS3935__
#define __AS3935__

extern const int intPin;

#include "Arduino.h"
#include "libs/SparkFun_AS3935_Lightning_Detector_Arduino_Library-master/src/SparkFun_AS3935.h"

class AS3935 : public SparkFun_AS3935
{
  public:
    AS3935();
    AS3935(i2cAddress address);
    float measureTime();
    bool tuneAntenna();

  protected:
  private:
};

#endif
