# DHT Sensor Library

A Raspberry Pi C++ based library for the series of low cost Digital humidity temperature sensors(DHT11/21/22)

## Usage

Root mode should be enabled to get readings from sensor. The GPIO pin must be given as a run parameter

Wrapper and Test driver already provided to run basic reading tests for convenience. To run:-

```
mkdir build
cd build
g++ -DPI_BUILD -o dht ../src/*.cpp -lwiringPi
./dht <GPIO_PIN_NUMBER>
```

Alternatively, to compile the library and run with your created main program(don't forget to setup the Pi in your program code):

```
mkdir build
cd build
g++ -DPI_BUILD -c ../src/DHT.cpp --lwiringPi
ar rvs DHT.a DHT.o
g++ <your_main_program.cpp> DHT.a
```

## Contributing

Pull requests are always welcome. For major changes, please open an issue first to discuss proposed changes.

## Licence

[MIT](https://choosealicense.com/licenses/mit/)
