
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "WiFly.h"

#define SSID      "CC3000"
#define KEY       "12345678"
#define AUTH      WIFLY_AUTH_WPA2_PSK

// Pins' connection
// Arduino       WiFly
//  2    <---->    TX
//  3    <---->    RX
WiFly wifly(2, 3);

void setup() {

  Serial.begin(9600);
  Serial.println("--------- WIFLY TEST --------");
  
  // wait for initilization of wifly
  delay(3000);
  
  wifly.reset();
  
  Serial.println("Join " SSID );
  if (wifly.join(SSID, KEY, AUTH)) {
    Serial.println("OK");
  } else {
    Serial.println("Failed");
  }
  
  // get WiFly params
  wifly.sendCommand("get everthing\r");
  char c;
  while (wifly.receive((uint8_t *)&c, 1, 300) > 0) {
    Serial.print((char)c);
  }
  
  if (wifly.commandMode()) {
    Serial.println("Enter command mode. Send \"exit\"(with \\r) to exit command mode");
  }
}

void loop() {
  while (wifly.available()) {
    Serial.write(wifly.read());
  }
  
  while (Serial.available()) {
    wifly.write(Serial.read());
  }
}
