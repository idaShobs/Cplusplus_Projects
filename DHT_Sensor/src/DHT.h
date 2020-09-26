/**
 * @brief     C++ library for DHT22 / DHT21 / DHT11 sensor
 * @author    Abidemi Shobayo-Eniola
 * @copyright Copyright (c) 2016-2020 Abidemi Shobayo-Eniola - All Rights
 *            Reserved
 * heat Index was computed using both Rothfusz and Steadman's equations
 * http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml

 * the constructor is set as private following the Singleton design pattern.
 * This project uses the BCM NATIVE HARDWARE NUMBERING FOR Setupmode.
 * In main, wiringPiSetupGpio() mode is used
 * The project should be run as root. For test purposes, Gpio17 was used
 */
#ifndef DHT_H
#define DHT_H

#include <iostream>

class DHT {
public:
    static DHT& getInstance(const uint16_t pin);

    float get_temperature();
    float get_humidity();
    float get_heatIndex();

private:
    DHT(const uint16_t pin);
    ~DHT() = default;

    bool pulse_setter();
    void read_sensor();
    const float compute_heat_index();
    const float convertCtoF(const float c);
    const float convertFtoC(const float f);

    float m_temperature = 0.0F;
    float m_humidity = 0.0F;
    uint8_t m_data[5] = {0};
    uint16_t m_pin;
    static constexpr uint16_t DHT_MAXCOUNT = 32000;
    static constexpr uint16_t DHT_PULSES = 41;
};

#endif
