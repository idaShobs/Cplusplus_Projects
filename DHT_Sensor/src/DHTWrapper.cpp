#include "DHTWrapper.h"

DHTWrapper::DHTWrapper(uint16_t pin){
    _dht = DHT::getInstance(pin);
}

DHTWrapper::manual_reading(){
    //Read temperature twice to make sure reading is accurate
    _dht -> get_temperature();
    wait(READ_INTERVAL);
    std::cout<<"Temperature: " << _dht -> get_temperature() << (char)167 << "C" << " ";
    std::cout<<"(Real feel: "<< _dht -> get_heatIndex() << (char)167 << "C)" << std::endl;
    std::cout<<"Humidity: " << _dht -> get_humidity() << (char)37 << std::endl;
}


