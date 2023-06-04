const int unitsPerG = 163; // depends on your ADC and the range (1.5g vs 6g)
int axisPin[] = {A0, A1, A2};
int selfTestPin = 8;
int gSelectPin = 9;
int sleepPin = 10;
int startVal[3] = {0};

char result[8]; // for format of the output
float xAcc, yAcc, zAcc;
float roll,pitch,rollF,pitchF=0;

void setup(){
  Serial.begin(9600);
  pinMode(selfTestPin, OUTPUT);
  pinMode(gSelectPin, OUTPUT);
  pinMode(sleepPin, OUTPUT);
  digitalWrite(sleepPin, HIGH);
  digitalWrite(selfTestPin, LOW);
  digitalWrite(gSelectPin, LOW);
  getStartVals(); 
}

void loop(){
  int angles[3] = {0};
  int raw[3] = {0};
  float gVal[3] = {0.0};

  getRawVals(raw);
  getGValues(gVal, raw); 

  xAcc = mapf(raw[0], 0, 1023, -1.0, 1.0); 
  yAcc = mapf(raw[1], 0, 1023, 0.0f, 5.0f); 
  zAcc = mapf(raw[2], 0, 1023, 0.0, 1.0); 

  Serial.print(xAcc); 
  Serial.print(" ");
  Serial.print(yAcc);
  Serial.print(" ");
  Serial.print(zAcc);
  Serial.print(" ");

  // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
   roll = atan(gVal[1] / sqrt(pow(gVal[0], 2) + pow(gVal[2], 2))) * 180 / PI;
   pitch = atan(-1 * gVal[0] / sqrt(pow(gVal[1], 2) + pow(gVal[2], 2))) * 180 / PI;


  // Low-pass filter
  rollF = 0.98 * roll + 0.02 * rollF;
  pitchF = 0.98 * pitch + 0.02 * pitchF;


  Serial.print(rollF);
  Serial.print(" ");
  Serial.println(pitchF);
  
  delay(200);
}

void getGValues(float gValueArray[], int rawArray[]){
  for(int i=0; i<3; i++){
    int diff = rawArray[i] - startVal[i];
    gValueArray[i] = diff*1.0/unitsPerG;
  }
}

void getRawVals(int rawVals[]){
  for(int i=0; i<3; i++){
    rawVals[i] = readMMA7361(axisPin[i]);
  }
}

int readMMA7361(int pin){
  long sum = 0;
  int result = 0;
  for(int i=0; i<50; i++){  // mean value of 50 measurements
    sum += analogRead(pin);
  }
  result = sum / 50;
  return result;
}

void getStartVals(){
  delay(2000); // to have enough time to position the MMA7361
  for(int i=0; i<3; i++){
    startVal[i] = readMMA7361(axisPin[i]);
  }
  startVal[2] -= unitsPerG; // Z-axis is at 90° in start position! 

}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
