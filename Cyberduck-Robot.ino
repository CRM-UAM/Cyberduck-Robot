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
int sensorPins[sensorCount] = {8, 7, 6, 12, 13}; // From left to right (travelwise)
int sensorValues[sensorCount];

//PID
int pid = 0;
int pid_left = 0;
int pid_right = 0;
int error = 0;
int previous_error = 0;

// Proportional Control
const int kp = 1;

// Integral control
int integral = 0;
const int ki = 1;

// Derivative control
int derivative = 0;
const int kd = 1;



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
  /*
  Serial.print("Sensors left to right: ");
  for(int i = 0; i < sensorCount; i++){
    Serial.print(sensorValues[i]);
  }
  Serial.println();
  delay(1000);
  */

  robot_movement();

  //test_motor();
}

// @brief This method will update the array sensorValues with the current digital read of all the sensors in the array
void readSensors(){
  for(int i = 0; i < sensorCount; i++){
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
}

void robot_movement(){
  readSensors(); // (0 0 1 1 0)
  int sensorCum =0;

  // Calculate the accomulative sensors value
  for(int i = 0; i < sensorCount; i++){
    sensorCum += sensorValues[i] * pow(2, sensorCount-1-i);
    
  }

  // Get the error for the current disposition of sensors
  
  previous_error = error;
  integral += previous_error;
  
  switch(sensorCum){
    case 0b11111: 
      // Leftwards of the track
      if(previous_error < 0) error = -3;
      // Rightwards of the track
      else error = 3;
     break;
    
    case 0b10111:
      error = -1;
      break;

    case 0b11101:
      error = 1;
      break;

    case 0b01111:
      error = -2;
      break;

     case 0b11110:
      error = 2;
      break;
          
    // In either case we want to continue moving straight
    case 0b11011:
    case 0b00000:
      error = 0;
      break;

     default:
      // Leftwards of the track
      if(previous_error < 0) error = -3;
      // Rightwards of the track
      else error = 3;
     break;
  }
  
  derivative = error-previous_error;

  if(error < 0){
    pid_left *= -1;
  }else if(error > 0){
    pid_right *= -1;
  }

  pid = pid_steering_value();
  pid_left = stdSpeed + pid; //quizás lo tengo que invertir
  pid_right = stdSpeed + pid;

  if(pid_left < 0) pid_left = 0;
  else if(pid_left > 255) pid_left = 255;

  if(pid_right < 0) pid_right = 0;
  else if(pid_right > 255) pid_right = 255; 

  forwardMovement(pid_left, pid_right);

  Serial.print(sensorCum);
  Serial.print("\n");
  Serial.print(error);
  Serial.print("\n");
  Serial.print(pid_left);
  Serial.print(" ");
  Serial.print(pid_right);
  Serial.print("\n\n");
  delay(1000);

  /*
  if(error < 0){
    turnRight(pid_left, pid_right);
  }else if(error > 0){
    turnLeft(pid_left, pid_right);
  }
  else{
    forwardMovement(pid_left, pid_right);
  }
  */
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

int pid_steering_value(){
  return error * kp + integral * ki + derivative * kd;
}
