#include <SoftwareSerial.h>
#include <NewPing.h>

#define TRIGGER_PIN1  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN1     11  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIGGER_PIN2  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN2     9  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIGGER_PIN3  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN3     5  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define DETECT_DISTANCE 80

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//Line detection
int lineSensorLeftPin = 7;  // This is our input pin
int lineSensorRightPin = 6;  // This is our input pin
int lineSensorLeft = HIGH;  // HIGH MEANS NO OBSTACLE
int lineSensorRight = HIGH;  // HIGH MEANS NO OBSTACLE

SoftwareSerial espSerial(2, 3); // RX, TX

void setup() {

  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(lineSensorLeftPin, INPUT);
  pinMode(lineSensorRightPin, INPUT);
  pinMode(13, OUTPUT);

  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/

}

void loop() { // run over and over

  //Obstacle
  if (sonar1.ping_cm() < DETECT_DISTANCE && sonar1.ping_cm() > 0) {
    //Serial.println("Sonar 1");
    espSerial.write(1);
    digitalWrite(13, HIGH);
  }
  if (sonar2.ping_cm() < DETECT_DISTANCE && sonar2.ping_cm() > 0) {
    //Serial.println("Sonar 2");
    espSerial.write(2);
    digitalWrite(13, HIGH);
  }
  if (sonar3.ping_cm() < DETECT_DISTANCE && sonar3.ping_cm() > 0) {
    //Serial.println("Sonar 3");
    espSerial.write(3);
    digitalWrite(13, HIGH);
  }

  //Line sensor
  lineSensorLeft = digitalRead(lineSensorLeftPin);
  lineSensorRight = digitalRead(lineSensorRightPin);
  if (lineSensorRight == LOW && lineSensorLeft == HIGH)
  {
    //Serial.println("Ligne à droite - tourner à droite (inversé donc à gauche)");
    espSerial.write(6); //6 = right, as it is on a numpad
  }
  else if (lineSensorRight == LOW && lineSensorLeft == LOW)
  {
    //Serial.println("Ligne au millieu - aller tout droit (inversé donc reculer)");
     espSerial.write(8); //8 = up, as it is on a numpad
  }
  else if (lineSensorRight == HIGH && lineSensorLeft == LOW)
  {
    //Serial.println("Ligne à gauche - tourner à gauche (inversé donc à droite)");
    espSerial.write(4); //4 = left, as it is on a numpad
  }
  else
  {
    //Serial.println("Pas de ligne - aller tout droit");
    espSerial.write(9); //9 = because can't write 0
  }

  delay(50);
  digitalWrite(13, LOW);
  delay(50);

}
