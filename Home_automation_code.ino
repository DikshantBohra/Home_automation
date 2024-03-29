#include <Servo.h>

Servo Roomservo;
Servo parkservo;
Servo gateservo;

#define ledPin 11
//#define moisture A0
const int trig = 9;
const int echo = 10;
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 8; // Echo Pin of Ultrasonic Sensor
//const int pump=2;
const int ldrPin=A1;
char state ;
int moists;


void setup() {
//  DOOR
  pinMode(trig, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo, INPUT); // Sets the echoPin as an Input
  Roomservo.attach(3);
  Roomservo.write(120);
//  pinMode(dLock, OUTPUT);

////Irrigation  
//  pinMode(pump, OUTPUT);
//  pinMode(moisture,INPUT);
//  
//Parking
  pinMode(echoPin, INPUT);
  pinMode(pingPin, OUTPUT);
  parkservo.attach(12);
  parkservo.write(120);
  
//Light  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(ldrPin, INPUT);

//GLASS
//  pinMode(Smartglass,OUTPUT);

//GATE
  gateservo.attach(6);
  gateservo.write(120);
  
  Serial.begin(38400); // Default communication rate of the Bluetooth module
}

void loop() {
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
//    state = Serial.read(); // Reads the data from the serial port
 }

 if(Serial.available()>0){
  state=Serial.read();
 }
 
// led();
 mled(state);
// glass(state);
 door(state);
 parking(state);
 maingate(state);
 
}

//-----------------------------------------------------------------------------------------------------------------------
void led(){
  int ldr = analogRead(ldrPin);
  if (ldr <= 200) {

  digitalWrite(ledPin, HIGH);

  } else if (ldr >= 200) {

  digitalWrite(ledPin, LOW);
  
 } 
}
//----------------------------------------------------------------------------------------------------------------------

void mled(char st){
  if(st=='L'){
    digitalWrite(ledPin,HIGH );
  }
  else if(st=='J'){
    digitalWrite(ledPin,LOW );
  }
}
//--------------------------------------------------------------------------------------------------------------------------

//void glass(int gls){
//  if(gls=="GD"){
//    digitalWrite(Smartglass, HIGH);
//  }
//  else{
//    digitalWrite(Smartglass, LOW);
//  } 
//}

//---------------------------------------------------------------------------------------------------------------------------------

//void garden(){
// moists = analogRead(moisture); 
// Serial.println("Analog Value : ");
// Serial.println(moists);
//
// int limit=300;
// 
// if (moists<limit) {
// digitalWrite(pump, HIGH); 
// }
// else {
// digitalWrite(pump, LOW); 
// } 
//}
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
   
  if(data=='M'){
   parkservo.write(0);
   delay(5000);
   parkservo.write(120); 
  }
  else if(data=='N'){
    parkservo.write(0);
  }
  else if(data=='O'){
    parkservo.write(120);
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
  
  if(Door_c=='A' && distance<=100){
    Roomservo.write(0);
    delay(3000);
    Roomservo.write(120);
  }
  else if(Door_c=='B'){
    Roomservo.write(0);
  }
  else if(Door_c=='C'){
    Roomservo.write(120);
  }
  
}
void maingate(char gatest){
  if (gatest=='Y'){
    
    for(int i=120;i>=1;i-=10)
    {
    gateservo.write(i);
    delay(300);
   }
  }
  else if(gatest=='Z'){
    for(int i=10;i<=125;i+=10)
    {
    gateservo.write(i);
    delay(300);
   }
  }
}
