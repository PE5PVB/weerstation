/**
 **************************************************
 *
 * @file        AS3935-Lightning-sensor-SOLDERED.cpp
 * @brief       Intentionaly blank.
 *
 *
 * @copyright GNU General Public License v3.0
 * @authors     Goran Juric @ soldered.com
 ***************************************************/

#include "AS3935-Lightning-sensor-SOLDERED.h"

AS3935::AS3935()
{
}

AS3935::AS3935(i2cAddress address) : SparkFun_AS3935(address)
{
}

bool AS3935::tuneAntenna()
{
    const float ideal_time = 32; // when using divider 16
    byte divVal = 16;

    pinMode(intPin, INPUT);

    changeDivRatio(divVal);

    displayOscillator(true, 3);

    float curr = 0, temp = 0, best = 999;
    int bestIteration = 0;
    for (int i = 0; i < 16;
         i++) // Unfortunatelly this is the easiest way to find smallest difference in desired and measured time
    {
        tuneCap(8 * i); // Set current intex to capacitor value
        delay(2);       // Time to stabilise frequency
        curr = 0;
        for (int j = 0; j < 10; j++) // Measuring time 10 times and averageing
        {
            temp = measureTime();
            if (temp == 0)
            {
                Serial.println("Error! Interrupt pin is not connected!");
                break;
            }
            curr += measureTime();
        }
        if (temp == 0)
        {
            break;
        }
        curr /= 10;
        if (abs(32 - curr) < best) // Checking if current is better than previous best
        {
            best = abs(32 - curr);
            bestIteration = i;
        }
    }
    if (temp)
    {
        tuneCap(8 * bestIteration);

        Serial.println("Sensor is calibrated to best internal capacitor value!");
        Serial.print("Capacitor value after calibration: ");
        Serial.print(readTuneCap());
        Serial.println(" pF.");
        Serial.print("Frequency after calibration: ");
        Serial.print(1 / measureTime() * 16 * 1000);
        Serial.println(" kHz.");
        Serial.println(measureTime());
    }

    displayOscillator(false, 3);

    return calibrateOsc();
}

float AS3935::measureTime()
{
    uint32_t start, finish;
    int counter = 0;
    bool curr = 0, prev = 0;
    counter = 0;
    start = micros();
    // while(PIND & B00010000) //If Arduino board is used, this line can be uncommented for more precise measure
    while (digitalRead(intPin)) // If Arduino board is used, this line can be commented for more precise measure
    {
        if ((micros() - start) > 1000)
        {
            return 0;
        }
    }
    start = micros();    // Take timestamp of start
    while (counter < 50) // Measure time for 50 cycles
    {
        // curr = PIND & B00010000;  //If Arduino board is used, this line can be uncommented for more precise measure
        curr = digitalRead(intPin); // If Arduino board is used, this line can be commented for more precise measure
        if (curr == 1 && prev == 0) // If state is changed from 0 to 1
        {
            counter++;
        }
        prev = curr;
    }
    finish = micros() + 25; // Compensation for commands timer didn't counted and add to timestamp on end of counting
    return (finish - start) / 50.0; // Divide with 50 to get time for one cycle
}