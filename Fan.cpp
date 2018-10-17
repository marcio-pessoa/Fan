/* Fan.cpp - Fan Speed Control and Sensor Library
 * 
 */

#include "Arduino.h"
#include "Fan.h"

/* Fan
 * 
 * Description
 *   Fan speed control.
 * 
 *   Fan (int write_pin, int read_pin)
 * 
 * Parameters
 *   write_pin: Arduino LED connected to PWM motor controller
 *   read_pin: Arduino pin connected to motor sensor pin 
 * 
 * Returns
 *   void
 */
Fan::Fan(byte write_pin, byte read_pin) {
  pinMode(write_pin, OUTPUT);
  _read_pin = read_pin;
  _write_pin = write_pin;
  _percent_speed = 0;
  _rpm = 0;
  _timer = millis();
  _resolution = 400;
  _delta_t = 0;
  _counter_rpm = 0;
  _is_sensor_present = false;
  if (read_pin != -1) {
    _is_sensor_present = true;
    pinMode(read_pin, INPUT_PULLUP);
    // attachInterrupt(_read_pin, counter, RISING);
  }
}

/* counter
 * 
 * Description
 *   Just a counter
 * 
 * Parameters
 *   none
 * 
 * Returns
 *   void
 */
void Fan::counter() {
  _counter_rpm++;
  if (_counter_rpm >= _resolution) {
    _delta_t = millis() - _timer;
    _rpm = (float)_counter_rpm / 2 / _delta_t * 1000 * 60;
    _timer = millis();
    _counter_rpm = 0;
  }
  if (!_is_sensor_present) {
    _counter_rpm = 0;
  }
}

/* writeSpeed
 * 
 * Description
 *   Change fan speed value.
 * 
 *   fan.speedWrite(int percent_speed)
 * 
 * Parameters
 *   percent_speed: Fan percent speed
 * 
 * Returns
 *   false: invalid input
 *   true: no errors
 */
int Fan::writeSpeed(int percent_speed) {
  _percent_speed = percent_speed;
  // Check input limits
  if (_percent_speed < 0 or _percent_speed > 100)
    return true;
  // Percent to Arduino PWM limits
  int pwm8bits = percent_to_pwm(_percent_speed);
  // Write to PWM
  analogWrite(_write_pin, pwm8bits);
  return false;
}

/* readSpeed
 * 
 * Description
 *   .
 * 
 *   a_led.blink()
 * 
 * Parameters
 *   none
 * 
 * Returns
 *   false: if the last state was not modified
 *   true: if the last state was modified
 */
int Fan::readSpeed() {
  return _percent_speed;
}

/* incrementSpeed
 * 
 * Description
 *   Increment fan speed.
 * 
 *   fan.speedIncrement()
 * 
 * Parameters
 *   none
 * 
 * Returns
 *   false: if no errors
 *   true: if speed is on limits
 */
int Fan::incrementSpeed() {
  _percent_speed++;
  if (_percent_speed > 100) {
    _percent_speed = 100;
    return true;
  }
  // Percent to Arduino PWM limits
  int pwm8bits = percent_to_pwm(_percent_speed);
  // Write to PWM
  analogWrite(_write_pin, pwm8bits);
  return false;
}

/* decrementSpeed
 * 
 * Description
 *   Decrement fan speed.
 * 
 *   fan.speedIncrement()
 * 
 * Parameters
 *   none
 * 
 * Returns
 *   false: if no errors
 *   true: if speed is on limits
 */
int Fan::decrementSpeed() {
  _percent_speed--;
  if (_percent_speed < 0) {
    _percent_speed = 0;
    return true;
  }
  // Percent to Arduino PWM limits
  int pwm8bits = percent_to_pwm(_percent_speed);
  // Write to PWM
  analogWrite(_write_pin, pwm8bits);
  return false;
}

/* readRPM
 * 
 * Description
 *   Get fan speed in RPM.
 * 
 *   fan.speedRPM()
 * 
 * Parameters
 *   none
 * 
 * Returns
 *   unsigned long: Fan speed in RPM
 *   unsigned long: -1 is errors found
 */
unsigned long Fan::readRPM() {
  return _rpm;
}

/* setResolutionRPM
 * 
 * Description
 *   Set RPM resolution to monitor fan speed in RPM.
 * 
 *   fan.setResolutionRPM(400)
 * 
 * Parameters
 *   resolution: An number used to compare with fan ticks provided by 
 *               rotation moviment detected from interruption system
 * 
 * Returns
 *   void
 */
void Fan::setResolutionRPM(int resolution) {
  _resolution = resolution;
}

/* percent_to_pwm
 * 
 * Description
 *   Convert percent values to PWM bit value
 * 
 *   private.percent_to_pwm(int percent, int bits)
 * 
 * Parameters
 *   percent: a percent value (0 ~ 100)
 *   bits: an value used to convesion (default: 8)
 * 
 * Returns
 *   int: value converted
 */
int percent_to_pwm(int percent, int bits) {
  return (((pow(2, bits) - 1) * percent) / 100);
}
