#include "DHTWrapper.h"

DHTWrapper::DHTWrapper(uint16_t pin){
    dht = DHT::getInstance(pin);
}

DHTWrapper::manual_reading(){
    //Read temperature twice to make sure reading is accurate
    dht -> get_temperature();
    wait(READ_INTERVAL);
    std::cout<<"Temperature: " << dht -> get_temperature() << (char)167 << "C" << " ";
    std::cout<<"(Real feel: "<< dht -> get_heatIndex() << (char)167 << "C)" << std::endl;
    std::cout<<"Humidity: " << dht -> get_humidity() << (char)37 << std::endl;
}


