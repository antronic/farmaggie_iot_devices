#include <SoftwareSerial.h>
SoftwareSerial BTSerial(12, 11);

void setup() {
  // put your setup code here, to run once:
  BTSerial.begin(9600);
  Serial.begin(9600);

  Serial.println("Start!");
}

void list_devices() {

}

boolean bluetooth_in = false;
String response = "";

boolean serial_in = false;
String request = "";

void loop() {
  // put your main code here, to run repeatedly:

  if(BTSerial.available() > 0) {
    bluetooth_in = true;

    char c = ' ';
    c = BTSerial.read();
    response += c;
    delay(1);
//    Serial.print(c);
  } else if (bluetooth_in) {
    bluetooth_in = false;
    Serial.println(response);
    response = "";
  }

  if(Serial.available() > 0) {
    serial_in = true;
    char c = ' ';
    c = Serial.read();

    // BTSerial.print(c);
    request += c;
  } else if (serial_in) {
    serial_in = false;
    if (request == "AT+DISI") {
      Serial.println("IN");
    } else {
      BTSerial.print(request);
    }
    request = "";
  }
}