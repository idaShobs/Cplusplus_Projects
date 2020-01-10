#include "DHT.h"

//global reference definition for the static variables
uint8_t DHT::data[5];
float  DHT::t = 0.0;
float DHT::h = 0.0;
DHT * DHT::_instance = NULL;
uint16_t DHT::_pin;
#ifdef PI_BUILD

#include <wiringPi.h>

//pointer constructor. Should be used in any class that calls needs a dht object
//e.g in main -> DHT * dhtobject = DHT::getInstance();
DHT * DHT::getInstance(uint16_t pin) {
	if (_instance == NULL) {
		_instance = new DHT();
	}
	_pin = pin;
	return _instance;
}


bool DHT::pulse_setter() {
	int pulseCounts[DHT_PULSES * 2] = { 0 };
	pinMode(_pin, OUTPUT);

	int priority = piHiPri(2);

	digitalWrite(_pin, HIGH);
	delay(500);

	digitalWrite(_pin, LOW);
	delay(20);

	pinMode(_pin, INPUT);
	pullUpDnControl(_pin, PUD_UP);
	for (volatile int i = 0; i < 50; ++i) {}

	uint32_t count = 0;
	//wait for dht to pull pin low
	while (digitalRead(_pin) == HIGH) {
		if (++count >= DHT_MAXCOUNT) {
			priority = piHiPri(0);
			std::cout << "time out waiting for low" << std::endl;
			return false;
		}

	}
	// Record pulse widths for the expected result bits.
	for (int i = 0; i < DHT_PULSES * 2; i += 2) {
		//count how long pin is low and store in pulseCounts
		while (digitalRead(_pin) == LOW) {
			if (++pulseCounts[i] >= DHT_MAXCOUNT) {
				priority = piHiPri(0);
				std::cout << "time out waiting for high" << std::endl;
				return false;
			}
		}


		//count how long pin is high and store in pulse counts
		while (digitalRead(_pin) == HIGH) {
			if (++pulseCounts[i + 1] >= DHT_MAXCOUNT) {
				priority = piHiPri(0);
				std::cout << "time out waiting for low" << std::endl;
				return false;
			}
		}
	}

	priority = piHiPri(0);

	// Compute the average low pulse width to use as a 50 microsecond reference threshold.
	// Ignore the first two readings because they are a constant 80 microsecond pulse.
	uint32_t threshold = 0;
	for (int i = 2; i < DHT_PULSES * 2; i += 2) {
		threshold += pulseCounts[i];
	}

	threshold /= DHT_PULSES - 1;

	// Interpret each high pulse as a 0 or 1 by comparing it to the 50us reference.
	// If the count is less than 50us it must be a ~28us 0 pulse, and if it's higher
	// then it must be a ~70us 1 pulse.
	data[5] = { 0 };
	for (int i = 3; i < DHT_PULSES * 2; i += 2) {
		int index = (i - 3) / 16;
		data[index] <<= 1;
		if (pulseCounts[i] >= threshold) {
			// One bit for long pulse.
			data[index] |= 1;
		}
		// Else zero bit for short pulse.
	}
	// Verify checksum of received data.
	if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
		return true;
	}
	else {
		std::cout << "Check sum failure" << std::endl;
		return false;
	}
}


void DHT::read_sensor() {
	float div;
	if (pulse_setter() == true) {
		temperature = &t;
		humidity = &h;
		h = ((float)(data[0] * 256 + data[1])) / 10.0;
		if (data[2] & 0x80) {//for minus temperature values
			div = -10.0;
		}
		else div = 10.0;

		t = ((float)((data[2] & 0x7F) * 256 + data[3])) / div;
	}
	else if (!pulse_setter()) {
		std::cout << "bad data, last good read will be returned" << std::endl; 
	}
}


float DHT::get_heatIndex() {
	float temperature;
	float percentHumidity;
	float hi;
	temperature = *temperature;
	percentHumidity = *humidity;
	temperature = convertCtoF(temperature);
	hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (percentHumidity * 0.094));
	if (hi > 79) {
		hi = -42.379 +
			2.04901523 * temperature +
			10.14333127 * percentHumidity +
			-0.22475541 * temperature*percentHumidity +
			-0.00683783 * pow(temperature, 2) +
			-0.05481717 * pow(percentHumidity, 2) +
			0.00122874 * pow(temperature, 2) * percentHumidity +
			0.00085282 * temperature*pow(percentHumidity, 2) +
			-0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

		if ((percentHumidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
			hi -= ((13.0 - percentHumidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

		else if ((percentHumidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
			hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
	}

	return convertFtoC(hi);
}


float DHT::convertCtoF(float c) {
	return c * 1.8 + 32;
}

float DHT::convertFtoC(float f) {
	return (f - 32) * 0.55555;
}

float DHT::get_temperature() {
	if(temperature == NULL)
		read_sensor();
	if (temperature != NULL)
		return *temperature;
	else
		return 0.0;
}

float DHT::get_humidity() {
	if(humidity == NULL)
		read_sensor();
	if (humidity != NULL)
		return *humidity;
	else
		return 0.0;
}
#else
//empty implementation for build on windows
float  DHT::convertCtoF(float c) { return -1; }
float DHT::convertFtoC(float f) { return -1; }
bool DHT::pulse_setter() { return false; }
DHT * DHT::getInstance(uint16_t pin) { return NULL; }
float DHT::get_temperature() { return -1; }
float DHT::get_humidity() { return -1; }
float DHT::get_heatIndex() { return -1; }

#endif
