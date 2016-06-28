#include <SoftwareSerial.h>
#include <NewPing.h>

#define TRIGGER_PIN1  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN1     11  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIGGER_PIN2  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN2     9  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIGGER_PIN3  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN3     3  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

SoftwareSerial espSerial(5, 6); // RX:5, TX:6
NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {

  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

void loop() { // run over and over
  if (sonar1.ping_cm() < 80 && sonar1.ping_cm() > 0) {
    Serial.println("Sonar 1");
    espSerial.println(0);
  }
  if (sonar2.ping_cm() < 80 && sonar2.ping_cm() > 0) {
    Serial.println("Sonar 2");
    espSerial.println(0);
  }
  if (sonar3.ping_cm() < 80 && sonar3.ping_cm() > 0) {
    Serial.println("Sonar 3");
    espSerial.println(0);
  }
  delay(100);
}
