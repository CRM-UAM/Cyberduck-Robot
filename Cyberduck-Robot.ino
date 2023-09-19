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
const int sensorCount = 3;
int sensorPins[sensorCount] = {6, 7, 8}; // From left to right (travelwise)
int sensorValues[sensorCount];

//PID
int pid = 0;
int pid_left = 0;
int pid_right = 0;
int error = 0;
int previous_error = 0;

// Proportional Control
const int kp = 0;

// Integral control
int integral = 0;
const int ki = 0;

// Derivative control
int derivative = 0;
const int kd = 0;



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
  
  /*Serial.print("Sensors left to right: ");
  for(int i = 0; i < sensorCount; i++){
    Serial.print(sensorValues[i]);
  }
  Serial.println();
  delay(1000);*/

  forwardMovement(255, 255);
}

// @brief This method will update the array sensorValues with the current digital read of all the sensors in the array
void readSensors(){
  for(int i = 0; i < sensorCount; i++){
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
}

void robot_movement(){
  readSensors();
  int sensorCum =0;

  // Calculate the accomulative sensors value
  for(int i = 0; i < sensorCount; i++){
    sensorCum += sensorValues[i] * pow(2, i);
  }

  
  // Get the error for the current disposition of sensors
  
  previous_error = error;
  integral += previous_error;
  derivative = error-previous_error;
  switch(sensorCum){
    case 0b000: 
      // Leftwards of the track
      if(previous_error < 0) error = -2;
      // Rightwards of the track
      else error = 2;
     break;
    
    case 0b100:
      error = -1;
      break;
    
    case 0b001:
      error = 1;
      break;
      
    // In either case we want to continue moving straight
    case 0b010:
    case 0b111:
      error = 0;
      break;
  }

  pid = pid_steering_value();
  pid_left = stdSpeed - pid;
  pid_right = stdSpeed + pid;

  if(pid_left < 0) pid_left = 0;
  else if(pid_left > 255) pid_left = 255;

  if(pid_right < 0) pid_right = 0;
  else if(pid_right > 255) pid_right = 255; 

  forwardMovement(pid_left, pid_right);
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

int pid_steering_value(){
  return error * kp + integral * ki + derivative * kd;
}
