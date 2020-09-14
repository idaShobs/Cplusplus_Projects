#ifndef DHTWrapper_H
#define DHTWrapper_H
#include "DHT.h"

class DHTWrapper {
public:
    explicit DHTWrapper(const uint16_t pin);
    void manual_reading();

private:
    DHT* m_dht = nullptr;
};
#endif
