byte motor_p1 = 10;
byte motor_p2 = 9;
byte pwmPin = 11;
int currentAngle;
int requiredAngle;
int errorAmount;
int remappedErrorAmount;
byte acceptableError = 4; //for smoothing

void setup()
{
  Serial.begin(9600);
  pinMode(motor_p1, OUTPUT);
  pinMode(motor_p2, OUTPUT);
  pinMode(pwmPin, OUTPUT);
}

void loop()
{
  readAndConditionAngle();
  readAndConditionRequiredAngle();
  if (currentAngle<requiredAngle){
    digitalWrite(motor_p1, HIGH);                      
    digitalWrite(motor_p2, LOW);
  } if (currentAngle>requiredAngle){
    digitalWrite(motor_p1, LOW);                      
    digitalWrite(motor_p2, HIGH);
  }
  calculateErrorAmount(currentAngle,requiredAngle);
  analogWrite(pwmPin,remappedErrorAmount);
  Serial.println(currentAngle);
 
}
 void calculateErrorAmount(int currentAngle,int requiredAngle){
  errorAmount=abs(currentAngle-requiredAngle);
  remappedErrorAmount=map(errorAmount, 300, 800, 200, 255);
  if (remappedErrorAmount<acceptableError){
    remappedErrorAmount=0;
  }}
 
 
void readAndConditionAngle() {
  currentAngle = analogRead(A4);
  if (currentAngle<200){
   currentAngle=200; 
  } if (currentAngle>800) {
   currentAngle=800; 
  
}}

void readAndConditionRequiredAngle() {
  requiredAngle = 400;
  if (requiredAngle<200){
   requiredAngle=200; 
  } if (requiredAngle>800) {
   requiredAngle=800; 
  
}}
