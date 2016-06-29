#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#include "pindef-nodemcu1.0.h"

SoftwareSerial espSerial(SD3, SD2);  // RX, TX
WiFiServer server(80);

int motor_speed;
int speed_default = 700;
int speed_turn = 1023;

// last command, used to stop correctly
String last_command = "start";

int motor1_enablePin = D7; //pwm
int motor1_in1Pin = D5;
int motor1_in2Pin = D6;

int motor2_enablePin = D0; //pwm
int motor2_in1Pin = D2;
int motor2_in2Pin = D1;

int driveMode = 0; //0:man 1:auto

const char* ssid = "LPDW-IOT";
const char* password = "LPDWIOTROUTER2015";

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(motor1_in1Pin, OUTPUT);
  pinMode(motor1_in2Pin, OUTPUT);
  pinMode(motor1_enablePin, OUTPUT);

  pinMode(motor2_in1Pin, OUTPUT);
  pinMode(motor2_in2Pin, OUTPUT);
  pinMode(motor2_enablePin, OUTPUT);

  Serial.begin(9600);
  espSerial.begin(9600);

  while (!Serial) {
    ;
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());

  delay(3000);

}

void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    if (driveMode == 1) {
      start(speed_default);
      if (espSerial.available()) {
        Serial.write(espSerial.read());

        if (espSerial.read() < 1) {
          stopp(speed_default);
          left(speed_turn);
          delay(250);
        }
      }
    }
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if (req.indexOf("/mode/man") != -1) {
    driveMode = 0;
  }
  else if (req.indexOf("/mode/auto") != -1) {
    driveMode = 1;
  }

  if (driveMode == 0) {
    Serial.println("Man");
    if (req.indexOf("/start") != -1) {
      start(speed_default);
    }
    else if (req.indexOf("/stop") != -1) {
      stopp(speed_default);
    }
    else if (req.indexOf("/back") != -1) {
      back(speed_default);
    }
    else if (req.indexOf("/right") != -1) {
      right(speed_turn);
    }
    else if (req.indexOf("/left") != -1) {
      left(speed_turn);
    }
    else {
      Serial.println("invalid request");
      client.stop();
      return;
    }
  }

  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

}





void start(int motor_speed) {
  last_command = "start";
  analogWrite(motor1_enablePin, motor_speed);
  digitalWrite(motor1_in1Pin, true);
  digitalWrite(motor1_in2Pin, false);
  analogWrite(motor2_enablePin, motor_speed);
  digitalWrite(motor2_in1Pin, true);
  digitalWrite(motor2_in2Pin, false);
}

void stopp(int motor_speed) {
  if (last_command == "left")
  {
    right(speed_turn); //TODO : replace by motor_speed
    delay(100);
    left(0);
  }
  else if (last_command == "right")
  {
    left(speed_turn); //TODO : replace by motor_speed
    delay(100);
    right(0);
  }
  else if (last_command == "back")
  {
    start(motor_speed);
    delay(100);
    back(0);  
  }
  else
  {
    back(motor_speed);
    delay(100);
    start(0);
  }
}

void back(int motor_speed) {
  last_command = "back";
  analogWrite(motor1_enablePin, motor_speed);
  digitalWrite(motor1_in1Pin, false);
  digitalWrite(motor1_in2Pin, true);
  analogWrite(motor2_enablePin, motor_speed);
  digitalWrite(motor2_in1Pin, false);
  digitalWrite(motor2_in2Pin, true);
}

void right(int motor_speed) {
  last_command = "right";
  analogWrite(motor1_enablePin, motor_speed);
  digitalWrite(motor1_in1Pin, true);
  digitalWrite(motor1_in2Pin, false);
  analogWrite(motor2_enablePin, motor_speed);
  digitalWrite(motor2_in1Pin, false);
  digitalWrite(motor2_in2Pin, true);
}
void left(int motor_speed) {
  last_command = "left";
  analogWrite(motor1_enablePin, motor_speed);
  digitalWrite(motor1_in1Pin, false);
  digitalWrite(motor1_in2Pin, true);
  analogWrite(motor2_enablePin, motor_speed);
  digitalWrite(motor2_in1Pin, true);
  digitalWrite(motor2_in2Pin, false);
}

