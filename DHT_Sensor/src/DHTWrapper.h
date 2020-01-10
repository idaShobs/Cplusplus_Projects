#include "DHT.h"

#ifndef DHTWrapper_H
#define DHTWrapper_H
#endif

#define READ_INTERVAL 3000

class DHTWrapper : private DHT
{
  private:
    DHT *dht;

  public:
    DHTWrapper(uint16_t pin);
    void manual_reading();
};
