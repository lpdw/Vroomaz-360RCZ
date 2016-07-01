#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#include "pindef-nodemcu1.0.h"

#define STATIC_IP false
// Update these with values suitable for your network.
IPAddress ip(192,168,1,239);  //Node static IP 
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
//172.24.1.122

SoftwareSerial arduinoSerial(D3, D4);  // RX, TX
WiFiServer server(80);

int motor_speed;
int speed_default = 700;
int speed_turn = 1023;
int speed_slower = 550;
int speed_turn_slower = 1023; 

// last command, used to stop correctly
String last_command = "start";

int motor1_enablePin = D5; //pwm
int motor1_in1Pin = D0;
int motor1_in2Pin = D1;

int motor2_enablePin = D6; //pwm
int motor2_in1Pin = D2;
int motor2_in2Pin = D7;

int driveMode = 0; //0:man 1:auto 2:line

//const char* ssid = "LPDW-IOT";
//const char* password = "LPDWIOTROUTER2015";
const char* ssid = "raspberrypi-vroomaz";
const char* password = "raspberrypi-vroomaz";

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
  arduinoSerial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  if (STATIC_IP == true) WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print(".");
    delay(250);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");;

  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());

  delay(3000);

}

void loop() {

  // Check if a client is connected
  WiFiClient client = server.available();
  if (!client) {
    if (driveMode == 1) {    
      start(speed_default);
      if (arduinoSerial.available()) {
        

        int arduinoSerialValue = arduinoSerial.read();

        Serial.println(arduinoSerialValue);

        if (arduinoSerialValue == 1 || arduinoSerialValue == 2 || arduinoSerialValue == 3) {
          stopp(speed_default);
          delay(50);
          left(speed_turn);
          delay(250);
        }
      }
    } else if (driveMode == 2) {
      
      start(speed_slower);
      if (arduinoSerial.available()) {

        int arduinoSerialValue = arduinoSerial.read();

        Serial.println(arduinoSerialValue);

        if (arduinoSerialValue == 8) { //8 = up, as it is on a numpad
          stopp(speed_slower);
          delay(50);
          back(speed_slower);
          delay(100);
        } else if (arduinoSerialValue == 6) { //6 = right, as it is on a numpad
          stopp(speed_slower);
          delay(50);
          left(speed_slower);
          delay(100);
        } else if (arduinoSerialValue == 4) { //4 = left, as it is on a numpad
          stopp(speed_slower);
          delay(50);
          right(speed_slower);
          delay(100);          
        } else if (arduinoSerialValue == 9) { //9 = nothing
          delay(50);
          back(speed_slower);
          delay(100);
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
    stopp(speed_default);
  }
  else if (req.indexOf("/mode/auto") != -1) {
    driveMode = 1;
  }
  else if (req.indexOf("/mode/line") != -1) {
    driveMode = 2;
  }

  //Start and stop command available for all
  if (req.indexOf("/start") != -1) {
    start(speed_default);
  }
  else if (req.indexOf("/stop") != -1) {
    stopp(speed_default);
  }

  if (driveMode == 0) {
    Serial.println("Man");
    if (req.indexOf("/back") != -1) {
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
  digitalWrite(motor1_in1Pin, false);
  digitalWrite(motor1_in2Pin, true);
  analogWrite(motor2_enablePin, motor_speed);
  digitalWrite(motor2_in1Pin, false);
  digitalWrite(motor2_in2Pin, true);
}

void stopp(int motor_speed) {
  if (driveMode == 0 || driveMode == 2) {
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
  digitalWrite(motor1_in1Pin, true);
  digitalWrite(motor1_in2Pin, false);
  analogWrite(motor2_enablePin, motor_speed);
  digitalWrite(motor2_in1Pin, true);
  digitalWrite(motor2_in2Pin, false);
}

void right(int motor_speed) {
  last_command = "right";
  analogWrite(motor1_enablePin, motor_speed);
  digitalWrite(motor1_in1Pin, false);
  digitalWrite(motor1_in2Pin, true);
  analogWrite(motor2_enablePin, motor_speed);
  digitalWrite(motor2_in1Pin, true);
  digitalWrite(motor2_in2Pin, false);
}
void left(int motor_speed) {
  last_command = "left";
  analogWrite(motor1_enablePin, motor_speed);
  digitalWrite(motor1_in1Pin, true);
  digitalWrite(motor1_in2Pin, false);
  analogWrite(motor2_enablePin, motor_speed);
  digitalWrite(motor2_in1Pin, false);
  digitalWrite(motor2_in2Pin, true);
}

