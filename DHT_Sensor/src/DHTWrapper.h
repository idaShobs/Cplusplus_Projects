#include <stdio.h>
#include <stdlib.h>
#include "DHT.h"

#ifndef DHTWrapper_H
#define DHTWrapper_H

#define READ_INTERVAL 3000

class DHTWrapper : private DHT {
    private:
        DHT* _dht;
    public:
        DHTWrapper(uint16_t pin);
        void manual_reading();
}