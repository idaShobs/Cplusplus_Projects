#include "DHT.h"
#include <math.h>

DHT::DHT(const uint16_t pin) : m_pin(pin)
{
}

DHT& DHT::getInstance(const uint16_t pin)
{
    static DHT instance(pin);
    return instance;
}

#ifdef PI_BUILD
#include <wiringPi.h>

bool DHT::pulse_setter()
{
    uint16_t pulseCounts[DHT_PULSES * 2] = {0};
    pinMode(m_pin, OUTPUT);

    int priority = piHiPri(2);

    digitalWrite(m_pin, HIGH);
    delay(500);

    digitalWrite(m_pin, LOW);
    delay(20);

    pinMode(m_pin, INPUT);
    pullUpDnControl(m_pin, PUD_UP);
    for (volatile int i = 0; i < 50; ++i) {
    }

    uint8_t count = 0;
    // wait for dht to pull pin low
    while (digitalRead(m_pin) == HIGH) {
        if (++count >= DHT_MAXCOUNT) {
            priority = piHiPri(0);
            return false;
        }
    }
    // Record pulse widths for the expected result bits.
    for (uint16_t i = 0; i < DHT_PULSES * 2; i += 2) {
        // count how long pin is low and store in pulseCounts
        while (digitalRead(m_pin) == LOW) {
            if (++pulseCounts[i] >= DHT_MAXCOUNT) {
                priority = piHiPri(0);
                return false;
            }
        }

        // count how long pin is high and store in pulse counts
        while (digitalRead(m_pin) == HIGH) {
            if (++pulseCounts[i + 1] >= DHT_MAXCOUNT) {
                priority = piHiPri(0);
                return false;
            }
        }
    }

    priority = piHiPri(0);

    // Compute the average low pulse width to use as a 50 microsecond reference
    // threshold.
    // Ignore the first two readings because they are a constant 80 microsecond
    // pulse.
    uint16_t threshold = 0;
    for (uint16_t i = 2; i < DHT_PULSES * 2; i += 2) {
        threshold += pulseCounts[i];
    }

    threshold /= DHT_PULSES - 1;

    // Interpret each high pulse as a 0 or 1 by comparing it to the 50us
    // reference.
    // If the count is less than 50us it must be a ~28us 0 pulse, and if it's
    // higher
    // then it must be a ~70us 1 pulse.
    for (uint16_t i = 3; i < DHT_PULSES * 2; i += 2) {
        uint8_t index = (i - 3) / 16;
        m_data[index] <<= 1;
        if (pulseCounts[i] >= threshold) {
            // One bit for long pulse.
            m_data[index] |= 1;
        }
        // Else zero bit for short pulse.
    }
    // Verify checksum of received data.
    if (!(m_data[4] == ((m_data[0] + m_data[1] + m_data[2] + m_data[3]) & 0xFF))) {
        return false;
    }

    return true;
}

void DHT::read_sensor()
{
    if (pulse_setter()) {
        m_humidity = static_cast<float>((m_data[0] * 256 + m_data[1]) / 10.0);

        const auto divisor = m_data[2] & 0x80 ? -10.0F : 10.0F; // -10 for negative temperature values
        m_temperature = static_cast<float>(((m_data[2] & 0x7F) * 256 + m_data[3]) / divisor);
    }
    else {
        std::cout << "bad data, last good read will be returned" << std::endl;
    }
}

#else // empty implementation for build on windows
bool DHT::pulse_setter() { return false; }
void DHT::read_sensor() {}
#endif

float DHT::get_heatIndex()
{
    const float hi = compute_heat_index();
    return hi;
}

const float DHT::compute_heat_index()
{
    float percentHumidity = m_humidity;
    float temperature = convertCtoF(m_temperature);
    float hi =
        0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (percentHumidity * 0.094));
    if (hi > 79) {
        hi = -42.379 + 2.04901523 * temperature + 10.14333127 * percentHumidity +
             -0.22475541 * temperature * percentHumidity + -0.00683783 * pow(temperature, 2) +
             -0.05481717 * pow(percentHumidity, 2) +
             0.00122874 * pow(temperature, 2) * percentHumidity +
             0.00085282 * temperature * pow(percentHumidity, 2) +
             -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

        if ((percentHumidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
            hi -= ((13.0 - percentHumidity) * 0.25) *
                  sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

        else if ((percentHumidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
            hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
    }

    return convertFtoC(hi);
}

const float DHT::convertCtoF(const float c)
{
    return c * 1.8 + 32;
}

const float DHT::convertFtoC(const float f)
{
    return (f - 32) * 0.55555;
}

float DHT::get_temperature()
{
    read_sensor();
    const float temperature = (m_temperature != 0.0F) ? m_temperature : 0.0F;
    return temperature;
}

float DHT::get_humidity()
{
    read_sensor();
    const float humidity = (m_humidity != 0.0F) ? m_humidity : 0.0F;
    return humidity;
}
