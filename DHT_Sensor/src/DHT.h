/** heat Index was computed
 * Using both Rothfusz and Steadman's equations
 * http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml

 * the constructor is set as private following the Singleton design pattern.
 * This project uses the BCM NATIVE HARDWARE NUMBERING FOR Setupmode.
 * In main, wiringPiSetupGpio() mode is used
 * The project should also be runned as root for it to work
 * for test purpose Gpio17 was used, so pin was declared as 17
 */

#ifndef DHT_H
#define DHT_H

// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <iostream>

// defines
#define DHT_MAXCOUNT 32000
#define DHT_PULSES 41

class DHT
{
  private:
    static DHT *_instance;
    float *temperature;
    float *humidity;
    static float t, h;
    float convertCtoF(float c);
    float convertFtoC(float f);
    bool pulse_setter();
    void read_sensor();
    static uint8_t data[5];
    uint8_t laststate;
    static uint16_t _pin;
    float start;

  protected:
    DHT()
    {
        start = 0.0;
        temperature = &start;
        humidity = &start;
    }

  public:
    static DHT *getInstance(uint16_t pin);
    float get_temperature();
    float get_humidity();
    float get_heatIndex();
};

#endif
