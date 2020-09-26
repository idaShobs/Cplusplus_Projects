/**
 * REQUIRES
 * One or more DHT11/DHT21/DHT22
 * GPIO pin number has to be passed as a command line argument
 */

#include <stdio.h>
#include <stdlib.h>
#include "DHTWrapper.h"

#ifdef PI_BUILD
#include <wiringPi.h>
#endif

int main(int argc, char* argv[])
{
#ifdef PI_BUILD
    if (wiringPiSetupGpio() == -1) {
        std::cout << "Pi GPIO Must be set up" << std::endl;
        return EXIT_FAILURE;
    }
#endif

    const auto pin = std::strtoul(argv[1], NULL, 0);
    if (!(argc > 1 && pin > 0)) {
        std::cout << "ERROR: GPIO Pin number must be passed as a command line argument"
                  << std::endl;
        return EXIT_FAILURE;
    }

    /* run test n times */
    int n = 1;
    do {
        DHTWrapper dht = DHTWrapper(static_cast<uint16_t>(pin));
        dht.manual_reading();
        ++n;
#ifdef PI_BUILD
        delay(1000); /* wait 1 second before next read */
#endif
    } while (n < 201);

    return EXIT_SUCCESS;
}
