
/*
REQUIRES
One or more DHT11/DHT21/DHT22
GPIO pin number has to be passed as a command line argument
*/

#include <stdio.h>
#include<stdlib.h>
#include "DHTWrapper.h"

int main(int argc, char *argv[])
{
	unsigned long pin = std::strtoul(argv[1], NULL, 0);
	if(argc > 1 && pin > 0){
		DHTWrapper dht = DHTWrapper((uint16_t)pin);
	}
	else{
		std::cout << "ERROR: GPIO Pin number must be passed as a command line argument"
	}
   return 0;
}


