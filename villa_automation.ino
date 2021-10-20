#define ledPin 5
#define moisture A0
const int trig = 9;
const int echo = 10;
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const int dMotorL=3;
const int dMotorR=4;
const int pMotorL=8;
const int pMotorR=12;
const int dLock=13;
const int pump=2;
const int ldrPin=A1;
const int Smartglass=11;
char state ;
int moists;


void setup() {
//  DOOR
  pinMode(trig, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo, INPUT); // Sets the echoPin as an Input
  pinMode(dMotorL, OUTPUT);
  pinMode(dMotorR, OUTPUT);
  pinMode(dLock, OUTPUT);

//Irrigation  
  pinMode(pump, OUTPUT);
  pinMode(moisture,INPUT);
  
//Parking
  pinMode(echoPin, INPUT);
  pinMode(pingPin, OUTPUT);
  pinMode(pMotorL, OUTPUT);
  pinMode(pMotorR, OUTPUT);
  
//Light  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(ldrPin, INPUT);

//GLASS
  pinMode(Smartglass,OUTPUT);
  
  Serial.begin(38400); // Default communication rate of the Bluetooth module
}

void loop() {
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
 }

 led();
 mled(state);
 glass(state);
 door(state);
 parking(state);
 
}

//-----------------------------------------------------------------------------------------------------------------------
void led(){
  int ldr = analogRead(ldrPin);
  if (ldr <= 200) {

  digitalWrite(ledPin, HIGH);

  } else {

  digitalWrite(ledPin, LOW);
  
 } 
}
//----------------------------------------------------------------------------------------------------------------------

void mled(char st){
  if(st=="LDH"){
    digitalWrite(ledPin,HIGH );
  }
  else{
    digitalWrite(ledPin,LOW );
  }
}
//--------------------------------------------------------------------------------------------------------------------------

void glass(int gls){
  if(gls=="GD"){
    digitalWrite(Smartglass, HIGH);
  }
  else{
    digitalWrite(Smartglass, LOW);
  } 
}

//---------------------------------------------------------------------------------------------------------------------------------

void garden(){
 moists = analogRead(moisture); 
 Serial.println("Analog Value : ");
 Serial.println(moists);

 int limit=300;
 
 if (moists<limit) {
 digitalWrite(pump, HIGH); 
 }
 else {
 digitalWrite(pump, LOW); 
 } 
}
//-----------------------------------------------------------------------------------------------------------------------------


void parking(char data) {
   long duration, cm;
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
   Serial.print(cm);
   Serial.print("cm");
   
   if(cm<=300 ){
    if(Serial.available() > 0){
    digitalWrite(pMotorL,HIGH);
    delay(5000);
    digitalWrite(pMotorL,LOW);
    delay(30000);
    digitalWrite(pMotorR,HIGH);
    delay(5000);
    digitalWrite(pMotorL,LOW);
   }
  }
  if(data=="PD_O"){
   digitalWrite(pMotorL,HIGH);
   delay(5000); 
  }
  else if(data=="PD_C"){
    digitalWrite(pMotorR,HIGH);
    delay(5000);
  }
  else{
    digitalWrite(pMotorR,LOW);
    digitalWrite(pMotorR,LOW);
  }
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void door(char Door_c){

  // Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(echo, HIGH);
  
  // Calculating the distance in CM
  int distance= duration*0.034/2;
  
  Serial.print("Distance: ");
  Serial.println(distance);
  
  if(Door_c=="L_O" && distance<=100){
    digitalWrite(dMotorL,HIGH);
    delay(2000);
    digitalWrite(dMotorL,LOW);
    delay(20000);
    digitalWrite(dMotorR,HIGH);
    delay(2000);
    digitalWrite(dMotorL,LOW);
  }
  else if(Door_c=="L_C"){
    digitalWrite(dMotorL,LOW);
    digitalWrite(dMotorR,LOW);
    digitalWrite(dLock,HIGH);
  }
  
}
