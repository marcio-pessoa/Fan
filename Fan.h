/* Fan.h - Fan Speed Control and Sensor Library
 * 
 * Author: Márcio Pessoa <marcio.pessoa@sciemon.com>
 * Contributors: none
 * 
 * Change log
 * 2016-09-08
 *         * Fan
 *         Fixed random behavior when reading fan rotation. Added INPUT_PULLUP
 *         in sensor pinMode.
 * 
 * 2015-06-05
 *         * Fan
 *         Set default _resolution variable to 400.
 * 
 * 2014-08-13
 *         Experimental version.
 */

#ifndef Fan_h
#define Fan_h

#include "Arduino.h"

class Fan
{
  public:
    Fan(byte write_pin, byte read_pin = -1);
    int writeSpeed(int percent_speed);
    int incrementSpeed();
    int decrementSpeed();
    int readSpeed();
    unsigned long readRPM();
    void setResolutionRPM(int resolution);
    void counter();
  private:
    byte _read_pin;
    byte _write_pin;
    int _percent_speed;
    unsigned long _rpm;
    int _resolution;
    int _delta_t;
    unsigned long _timer;
    unsigned int _counter_rpm;
    bool _is_sensor_present;
};

int percent_to_pwm(int percent, int bits=8);

#endif
