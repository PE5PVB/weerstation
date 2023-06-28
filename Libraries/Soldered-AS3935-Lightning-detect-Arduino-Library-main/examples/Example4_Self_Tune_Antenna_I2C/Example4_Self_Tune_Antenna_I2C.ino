/**
 **************************************************

   @file        Example4_Self_Tune_Antenna_I2C.ino
   @brief       This example sketch will self adjust resonant frequency.
                Resonant frequency should be set to 500 kHz and MCU will
                self determine value of internal capacitor. Example uses
                MCUs internal timer to measure frequency and it is not 100%
                accurate, but it is accurate enough to be in OPTIMAL range of 3.5%
                as datasheet states. This tuning is not neccesary to do before every
                turn on, but only on first use or if starts to malfunction.


    product: www.solde.red/333097

   @authors     Elias Santistevan, SparkFun Electronics

    Modified by Soldered.com
 ***************************************************/

#include <Wire.h>
#include "AS3935-Lightning-sensor-SOLDERED.h"

// 0x03 is default, but the address can also be 0x02, or 0x01.
// Adjust the address jumpers on the underside of the product.
#define AS3935_ADDR 0x03
#define ANTFREQ 3
#define TIMEOUT_uS 1000

AS3935 lightning(AS3935_ADDR);

uint32_t start, finish;
byte divVal = 16;
const float ideal_time = 32;  //when using divider 16
//ideal time period is 32 microseconds

// Interrupt pin for lightning detection
const int intPin = 4;

void setup()
{

  Serial.begin(115200);
  Serial.println("AS3935 Franklin Lightning Detector");

  pinMode(intPin, INPUT);

  Wire.begin(); // Begin Wire before lightning sensor.
  if ( !lightning.begin() ) { // Initialize the sensor.
    Serial.println ("Lightning Detector did not start up, freezing!");
    while (1);
  }
  else
    Serial.println("Ready to tune antenna!");
  Serial.println("-----------------------");


  lightning.changeDivRatio(divVal); //Change frequency divider to 16

  // This will tell the IC to display the resonance frequncy as a digital
  // signal on the interrupt pin. There are two other internal oscillators
  // within the chip that can also be displayed on this line but is outside the
  // scope of this example, see page 35 of the datsheet for more information.
  Serial.println("\n----Displaying oscillator on INT pin.----\n");
  lightning.displayOscillator(true, ANTFREQ);
  delay(20);

  float curr = 0, temp = 0, best = 999;
  int bestIteration = 0;
  for (int i = 0; i < 16; i++) //Unfortunatelly this is the easiest way to find smallest difference in desired and measured time
  {
    lightning.tuneCap(8 * i); //Set current intex to capacitor value
    delay(2); //Time to stabilise frequency
    curr = 0;
    for (int j = 0; j < 10; j++) //Measuring time 10 times and averageing
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
    if (abs(32 - curr) < best) //Checking if current is better than previous best
    {
      best = abs(32 - curr);
      bestIteration = i;
    }

  }
  if (temp)
  {
    lightning.tuneCap(8 * bestIteration);

    Serial.println("Sensor is calibrated to best internal capacitor value!");
    Serial.print("Capacitor value after calibration: ");
    Serial.print(lightning.readTuneCap());
    Serial.println(" pF.");
    Serial.print("Frequency after calibration: ");
    Serial.print(1 / measureTime() * 16 * 1000);
    Serial.println(" kHz.");
    Serial.println(measureTime());
  }
}

void loop() {

}

float measureTime()
{
  int counter = 0;
  bool curr = 0, prev = 0;
  counter = 0;
  start = micros();
  //while(PIND & B00010000) //If Arduino board is used, this line can be uncommented for more precise measure
  while (digitalRead(intPin)) //If Arduino board is used, this line can be commented for more precise measure
  {
    if ( (micros() - start) > TIMEOUT_uS)
    {
      return 0;
    }
  }
  start = micros(); //Take timestamp of start
  while (counter < 50) //Measure time for 50 cycles
  {
    //curr = PIND & B00010000;  //If Arduino board is used, this line can be uncommented for more precise measure
    curr = digitalRead(intPin); //If Arduino board is used, this line can be commented for more precise measure
    if (curr == 1 && prev == 0) //If state is changed from 0 to 1
    {
      counter++;
    }
    prev = curr;
  }
  finish = micros() + 25;  //Compensation for commands timer didn't counted and add to timestamp on end of counting
  return (finish - start) / 50.0; //Divide with 50 to get time for one cycle
}
