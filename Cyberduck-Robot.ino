// L298n pins (For motors)
#define right_in1 2
#define right_in2 3
#define left_in3 4
#define left_in4 5
#define enableLeft 10 //ENB
#define enableRight 11 //ENA

// Motors
#define stdSpeed 100

enum DirectionTaken{
  FWD,
  RIGHT,
  LEFT,
  NONE
};

// Sensors
const int sensorCount = 5;
int sensorPins[] = {8, 7, 6, 12, 13}; // From left to right (travelwise)
int sensorValues[sensorCount];
int previous_error = 0;
int error = 0;
DirectionTaken dir = NONE;

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
  int sensorCum =0;

  // Calculate the accomulative sensors value
  for(int i = 0; i < sensorCount; i++){
    sensorCum += sensorValues[i] * (1 << sensorCount-1-i); // pow is intended for floating point whereas with logical shifts we can work with integers without any interference
  }

  Serial.println();
  Serial.println(sensorCum, BIN);
  
    //delay(500);
  
  switch(sensorCum){
    case 0b10000:
    case 0b01000:
    case 0b11000:
    case 0b00111:
      turnRight(stdSpeed);
      Serial.print("Going right");
      dir = RIGHT;
      break;
      
    case 0b00001:
    case 0b00010:
    case 0b00011:
    case 0b11100:
      turnLeft(stdSpeed);
      Serial.print("Going left");
      dir = LEFT;
      break;

    // e.g.: 01011, which is an unlikely case, but we ought to consider it too 
    // In either case we want to continue moving straight
    case 0b01100:
    case 0b00110:
    case 0b00100:
    case 0b01110:
    case 0b11111:
      forwardMovement(stdSpeed, stdSpeed);
      Serial.print("Going forward");
      dir = FWD;
      break;

    default:
      // Leftwards of the track
      if(dir == LEFT) turnLeft(stdSpeed);
      else if(dir == RIGHT) turnRight(stdSpeed);
      // Rightwards of the track
      else forwardMovement(stdSpeed, stdSpeed);
      break;
  }
 
  forwardMovement(stdSpeed, stdSpeed);
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
  digitalWrite(left_in3, LOW);
  digitalWrite(left_in4, LOW);

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
  digitalWrite(right_in2, LOW);
}

void test_motor(){
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
