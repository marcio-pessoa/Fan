#include <Fan.h>

Fan fan(5, 2);

int speed=0;

void setup() {
  Serial.begin(57600);
  attachInterrupt(0, counter, RISING);
}

void loop() {
  speed+=10;
  speed%=100;
  fan.writeSpeed(speed);
  Serial.print("Fan speed set to: ");
  Serial.print(fan.readSpeed());
  Serial.print("% (");
  Serial.print(fan.readSpeed());
  Serial.println(" RPM)");
  delay(500);
}

void counter() {
  fan.counter();
}

