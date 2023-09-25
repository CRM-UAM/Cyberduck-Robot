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
    Serial.print(sensorValues[i]);
    sensorCum += sensorValues[i] * pow(2, sensorCount-1-i);
    Serial.print("-");
    Serial.print(pow(2,i));
    Serial.println("*");
  }

  Serial.println();
  Serial.println(sensorCum);
  
  switch(sensorCum){
    case 0b00000: 
      // Leftwards of the track
      if(previous_error < 0) error = -3;
      // Rightwards of the track
      else error = 3;
     break;
    
    case 0b01000:
      error = 1;
      //Serial.print("hola");
      break;
    
    case 0b00010:
      error = 1;
      break;

    case 0b10000:
      error = 2;
      break;
      
    case 0b00001:
      error = 2;
      break;

    // e.g.: 01011, which is an unlikely case, but we ought to consider it too 
    // In either case we want to continue moving straight
    case 0b00100:
    case 0b11111:
    default:
      error = 0;
      //Serial.print("adios");
      break;
  }

  previous_error = error;

  //Serial.println(error);
  if(error > 0){
     turnLeft(stdSpeed, stdSpeed);
  }else{
     turnRight(stdSpeed, stdSpeed);
  }
 
  forwardMovement(stdSpeed, stdSpeed);
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
