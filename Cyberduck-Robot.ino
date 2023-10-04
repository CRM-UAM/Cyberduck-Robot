#include <Servo.h>

// L298n pins (For motors)
#define right_in1 2
#define right_in2 3
#define left_in3 4
#define left_in4 5
#define enableLeft 10 //ENB
#define enableRight 11 //ENA

// Motors
#define stdSpeed 75

// Servo
Servo neck;
#define servoNeck A0

// Sensors
const int sensorCount = 5;
int sensorPins[] = {8, 7, 6, 12, 13}; // From left to right (travelwise)
int sensorValues[sensorCount];

void setup(){
    pinMode(left_in3, OUTPUT);
    pinMode(left_in4, OUTPUT);
    pinMode(right_in1, OUTPUT);
    pinMode(right_in2, OUTPUT);
    pinMode(enableRight, OUTPUT);
    pinMode(enableLeft, OUTPUT);

    neck.attach(servoNeck);

    for(int i = 0; i < sensorCount; i++){
      pinMode(sensorPins[i], INPUT);
    }

    Serial.begin(9600);
}

void loop(){
  //control_robot();

  //test_motor();

  test_servo();
}

// @brief This method will update the array sensorValues with the current digital read of all the sensors in the array
void readSensors(){
  for(int i = 0; i < sensorCount; i++){
    sensorValues[i] = digitalRead(sensorPins[i]);
  }

}

void control_robot(){
  readSensors();

  //int left2 = sensorValues[4]; // 13
  int left1 = sensorValues[3]; // 12
  //int central = sensorValues[2]; // 6
  int right1 = sensorValues[1]; // 7
  //int right2 = sensorValues[0]; // 8

  Serial.print("LEFT: ");
  Serial.print(sensorValues[3]);
  Serial.print(" RIGHT: ");
  Serial.print(sensorValues[1]);
  Serial.println();

  if(left1 == 0 and right1 == 0){
    forwardMovement(stdSpeed, stdSpeed);
  }else if(left1 == 0 and right1 == 1){
    turnRight(stdSpeed);
  }else if(left1 == 1 and right1 ==0){
    turnLeft(stdSpeed);
  }else{
    forwardMovement(stdSpeed, stdSpeed);
  }
  

  
}



void forwardMovement(int speedLeft, int speedRight){
  // Left wheel
  digitalWrite(left_in3, LOW);
  digitalWrite(left_in4, HIGH);
  analogWrite(enableLeft, speedLeft);

  // Right wheel
  digitalWrite(right_in1, HIGH);
  digitalWrite(right_in2, LOW);
  analogWrite(enableRight, speedRight);  
}

void turnLeft(int speedRight){
  // Left wheel
  digitalWrite(left_in3, HIGH);
  digitalWrite(left_in4, LOW);
  analogWrite(enableLeft, speedRight/3);

  // Right wheel
  digitalWrite(right_in1, HIGH);
  digitalWrite(right_in2, LOW);
  analogWrite(enableRight, speedRight);  
}

void turnRight(int speedLeft){
  // Left wheel
  digitalWrite(left_in3, LOW);
  digitalWrite(left_in4, HIGH);
  analogWrite(enableLeft, speedLeft);

  // Right wheel
  digitalWrite(right_in1, LOW);
  digitalWrite(right_in2, HIGH);
  analogWrite(enableRight, speedLeft/3);
}

void test_motor(){
  digitalWrite(right_in1, LOW);
  digitalWrite(right_in2, LOW);
  digitalWrite(left_in3, LOW);
  digitalWrite(left_in4, LOW);
  delay(1000);
  
  forwardMovement(stdSpeed, stdSpeed);
  delay(3000);
  turnLeft(stdSpeed);
  delay(1500);
  turnRight(stdSpeed);
  delay(1500);
}

void test_sensorIR(){
  Serial.print("Sensors left to right: ");
  for(int i = 0; i < sensorCount; i++){
    Serial.print(digitalRead(sensorPins[i]));
  }
  Serial.println();
}

void test_servo(){
  neck.write(60);
  delay(500);
  neck.write(90);
  delay(500);
  neck.write(120);
  delay(500);  
  neck.write(90);
  delay(500);
}
