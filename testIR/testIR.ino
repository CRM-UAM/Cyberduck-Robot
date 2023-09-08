#include <L298N.h>

// L298n pins (For motors)
#define left_in3 2
#define left_in4 3
#define right_in1 4
#define right_in2 5
#define enableLeft 9
#define enableRight 10

// Motors
L298N leftMotor(enableLeft, left_in3, left_in4);
L298N rightMotor(enableRight, right_in1, right_in2);

// Sensors
const int sensorCount = 3;
int sensorPins[sensorCount] = {6, 7, 8}; // From left to right (travelwise)
int sensorValues[sensorCount];

//PID
int error = 0;
int previous_error = 0;

// Proportional Control
const float kp = 0;

// Integral control
int integral = 0;
const float ki = 0;

// Derivative control
int derivative = 0;
const float kd = 0;



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
  
  Serial.print("Sensors left to right: ");
  for(int i = 0; i < sensorCount; i++){
    Serial.print(sensorValues[i]);
  }
  Serial.println();
  delay(1000);
  
  robot_movement();
}

// @brief This method will update the array sensorValues with the current digital read of all the sensors in the array
void readSensors(){
  for(int i = 0; i < sensorCount; i++){
    sensorValues[i] = digitalRead(sensorPins[i]);
  }
}

void robot_movement(){
  readSensors();
}
