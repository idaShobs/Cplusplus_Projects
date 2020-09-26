#include "DHTWrapper.h"

DHTWrapper::DHTWrapper(uint16_t pin)
{
    m_dht = &DHT::getInstance(pin);
}

void DHTWrapper::manual_reading()
{
    std::cout << "Humidity = " << m_dht->get_humidity() << "% "
              << "Temperature = " << m_dht->get_temperature() << "*C "
              << "(Real feel: " << m_dht->get_heatIndex() << "*C)" << std::endl;
}
