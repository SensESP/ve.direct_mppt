#ifndef _ve_direct_helper_H_
#define _ve_direct_helper_H_

#include "sensors/sensor.h"
#include "vedirect_parser.h"

class VEDirectInput : public Sensor {
  public:
    VEDirectInput(Stream* rx_stream);
    virtual void enable() override final;
    VEDirectData vedirect_data_;
  private:
    Stream* rx_stream_;
    VEDirectParser vedirect_parser_;
};

VEDirectInput* setup_vedirect(Stream* rx_stream);

#endif
