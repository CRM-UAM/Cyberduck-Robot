// L298n pins (For motors)
#define right_in1 2
#define right_in2 3
#define left_in3 4
#define left_in4 5
#define enableLeft 10 //ENB
#define enableRight 11 //ENA

// Motors
#define stdSpeed 230

// Sensors
const int sensorCount = 5;
int sensorPins[] = {8, 7, 6, 12, 13}; // From left to right (travelwise)
int sensorValues[sensorCount];
int previous_error = 0;
int error = 0;
bool wentLeft = false;

void setup(){
    pinMode(left_in3, OUTPUT);
    pinMode(left_in4, OUTPUT);
    pinMode(right_in1, OUTPUT);
    pinMode(right_in2, OUTPUT);
    pinMode(enableRight, OUTPUT);
    pinMode(enableLeft, OUTPUT);

    for(int i = 0; i < sensorCount; i++){
      pinMode(sensorPins[i], INPUT);
    }

    Serial.begin(9600);
}

void loop(){
  control_robot();
  
}

// @brief This method will update the array sensorValues with the current digital read of all the sensors in the array
void readSensors(){
  for(int i = 0; i < sensorCount; i++){
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
}

void control_robot(){
  readSensors();

  // Go forward
  if(((sensorValues[4] == 0 && sensorValues[3] == 0) && (sensorValues[2] == 1 && sensorValues[1] == 0)) && sensorValues[0] == 0){
    forwardMovement(stdSpeed, stdSpeed);
    wentLeft = false;
  }else if(((sensorValues[4] == 0 && sensorValues[3] == 1) && (sensorValues[2] == 1 && sensorValues[1] == 1)) && sensorValues[0] == 0){
    forwardMovement(stdSpeed, stdSpeed);
    wentLeft = false;
  }else if(((sensorValues[4] == 1 && sensorValues[3] == 1) && (sensorValues[2] == 1 && sensorValues[1] == 1)) && sensorValues[0] == 1){
    forwardMovement(stdSpeed, stdSpeed);
    wentLeft = false;
  }
  
  // Go left fast
  else if(((sensorValues[4] == 1 && sensorValues[3] == 0) && (sensorValues[2] == 0 && sensorValues[1] == 0)) && sensorValues[0] == 0){      
    turnLeft(stdSpeed, stdSpeed);
    wentLeft = true;
  }else if(((sensorValues[4] == 1 && sensorValues[3] == 1) && (sensorValues[2] == 0 && sensorValues[1] == 0)) && sensorValues[0] == 0){
    turnLeft(stdSpeed, stdSpeed);
    wentLeft = true;
  }

  // Go left medium speed
  else if(((sensorValues[4] == 0 && sensorValues[3] == 1) && (sensorValues[2] == 1 && sensorValues[1] == 0)) && sensorValues[0] == 0){    
    turnLeft(stdSpeed/2, stdSpeed/2);
    wentLeft = true;
  }else if(((sensorValues[4] == 0 && sensorValues[3] == 1) && (sensorValues[2] == 0 && sensorValues[1] == 0)) && sensorValues[0] == 0){    
    turnLeft(stdSpeed/2, stdSpeed/2);  
    wentLeft = true;  
  }

  // Go right fast
  else if(((sensorValues[4] == 0 && sensorValues[3] == 0) && (sensorValues[2] == 0 && sensorValues[1] == 1)) && sensorValues[0] == 1){
    turnRight(stdSpeed, stdSpeed);
    wentLeft = false;
  }
  else if(((sensorValues[4] == 0 && sensorValues[3] == 0) && (sensorValues[2] == 0 && sensorValues[1] == 0)) && sensorValues[0] == 1){
    turnRight(stdSpeed, stdSpeed);
    wentLeft = false;
  }

  // Go right medium speed
  else if(((sensorValues[4] == 0 && sensorValues[3] == 0) && (sensorValues[2] == 1 && sensorValues[1] == 1)) && sensorValues[0] == 0){
    turnRight(stdSpeed/2, stdSpeed/2);
    wentLeft = false;
  }
  else if(((sensorValues[4] == 0 && sensorValues[3] == 0) && (sensorValues[2] == 0 && sensorValues[1] == 1)) && sensorValues[0] == 0){
    turnRight(stdSpeed/2, stdSpeed/2);
    wentLeft = false;
  }

  // Locate if fully out of line or in weird cases
  else{
    if(wentLeft == true){
      turnRight(stdSpeed, stdSpeed);
      wentLeft = false;
    }else{
      turnLeft(stdSpeed, stdSpeed);
      wentLeft = true;
    }
  }

}

void forwardMovement(int speedLeft, int speedRight){
  // Left wheel
  digitalWrite(left_in3, LOW);
  digitalWrite(left_in4, HIGH);
  analogWrite(enableLeft, speedLeft);

  // Right wheel
  digitalWrite(right_in1, LOW);
  digitalWrite(right_in2, HIGH);
  analogWrite(enableRight, speedRight);  
}

void turnLeft(int speedLeft, int speedRight){
  // Left wheel
  digitalWrite(left_in3, HIGH);
  digitalWrite(left_in4, LOW);
  analogWrite(enableLeft, speedLeft);

  // Right wheel
  digitalWrite(right_in1, LOW);
  digitalWrite(right_in2, HIGH);
  analogWrite(enableRight, speedRight);  
}

void turnRight(int speedLeft, int speedRight){
  // Left wheel
  digitalWrite(left_in3, LOW);
  digitalWrite(left_in4, HIGH);
  analogWrite(enableLeft, speedLeft);

  // Right wheel
  digitalWrite(right_in1, HIGH);
  digitalWrite(right_in2, LOW);
  analogWrite(enableRight, speedRight);  
}

void test_motor(){
  forwardMovement(stdSpeed, stdSpeed);
  delay(3000);
  turnLeft(stdSpeed, stdSpeed);
  delay(1500);
  turnRight(stdSpeed, stdSpeed);
  delay(1500);
}

void test_sensorIR(){
  Serial.print("Sensors left to right: ");
  for(int i = 0; i < sensorCount; i++){
    Serial.print(digitalRead(sensorPins[i]));
  }
  Serial.println();
}
