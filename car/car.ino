int motor1_enablePin = 14; //pwm
int motor1_in1Pin = 16;
int motor1_in2Pin = 5;

int motor2_enablePin = 12; //pwm
int motor2_in1Pin = 4;
int motor2_in2Pin = 13;
 
void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //on initialise les pins du moteur 1
  pinMode(motor1_in1Pin, OUTPUT);
  pinMode(motor1_in2Pin, OUTPUT);
  pinMode(motor1_enablePin, OUTPUT);
 
  //on initialise les pins du moteur 2
  pinMode(motor2_in1Pin, OUTPUT);
  pinMode(motor2_in2Pin, OUTPUT);
  pinMode(motor2_enablePin, OUTPUT);
}
 
void loop(){
  SetMotor2(1023, true);
  SetMotor1(1023, false);
}
 
//Fonction qui set le moteur1
void SetMotor1(int speed, boolean reverse){
  analogWrite(motor1_enablePin, speed);
  digitalWrite(motor1_in1Pin, ! reverse);
  digitalWrite(motor1_in2Pin, reverse);
}
 
//Fonction qui set le moteur2
void SetMotor2(int speed, boolean reverse){
  analogWrite(motor2_enablePin, speed);
  digitalWrite(motor2_in1Pin, ! reverse);
  digitalWrite(motor2_in2Pin, reverse);
}
