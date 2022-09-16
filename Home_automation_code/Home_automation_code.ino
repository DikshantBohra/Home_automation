#include <Servo.h>
//#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <WiFiClient.h> 

Servo Roomservo;
Servo parkservo;
Servo gateservo;

const char *ssid =  "note";     
const char *pass =  "123456789";
WiFiClient client; 
unsigned long myChannelNumber = 1718386;
const char * myWriteAPIKey = "6JKI0MBZCUU31EOR";


#define ledPin 11
//#define moisture A0
const int trig = 9;
const int echo = 10;
const int trigPin = 7; 
const int echoPin = 8; 
//const int pump=2;
const int ldrPin=A1;
char state ;
int moists;


void setup() {
    // WIFI MODULE
       Serial.begin(115200); 
       delay(10);
       pinMode(trigPin, OUTPUT);
       pinMode(echoPin, INPUT);
       
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
       while (WiFi.status() != WL_CONNECTED) 
     {
            delay(550);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

      Serial.println(WiFi.localIP()); 
      ThingSpeak.begin(client); 


  
//  DOOR
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 
  Roomservo.attach(3);
  Roomservo.write(120);
//  pinMode(dLock, OUTPUT);

//Irrigation  
//  pinMode(pump, OUTPUT);
//  pinMode(moisture,INPUT);


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
  
  Serial.begin(38400); 
}

void loop() {

 if(Serial.available()>0){
    state=Serial.read();
   
  // led();
   mled(state);
  // glass(state);
   door(state);
   parking(state);
   maingate(state);
   
  }
  else
  {
    Lock();
    }
  }

  
//--------------------------------------------------------- Automatic Light --------------------------------------------------------------
  void led(){
    int ldr = analogRead(ldrPin);
    if (ldr <= 200) {
  
    digitalWrite(ledPin, HIGH);
  
    } else if (ldr >= 200) {
  
    digitalWrite(ledPin, LOW);
    
   } 
  }
//------------------------------------------------------- LED ---------------------------------------------------------------

  void mled(char st){
    if(st=='L'){
      digitalWrite(ledPin,HIGH );
    }
    else if(st=='J'){
      digitalWrite(ledPin,LOW );
    }
  }
//------------------------------------------------------- Glasses -------------------------------------------------------------------

//void glass(int gls){
//  if(gls=="GD"){
//    digitalWrite(Smartglass, HIGH);
//  }
//  else{
//    digitalWrite(Smartglass, LOW);
//  } 
//}

//----------------------------------------------------------  Garden -----------------------------------------------------------------------

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



//----------------------------------------------------------Paeking Gate-------------------------------------------------------------------


  void parking(char data)
  {
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

//--------------------------------------------------------------House Door--------------------------------------------------------------------------

  void door(char Door_c){
     int distance=ultrasonic();
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
  



//----------------------------------------------------Main Gate------------------------------------------------------------------------------------


 
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


//-------------------------------------------------Distance Calc------------------------------------------


int ultrasonic(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  int duration = pulseIn(echo, HIGH);
  
  int distance= duration*0.034/2;
  
  Serial.print("Distance: ");
  Serial.println(distance);
  
  return distance;
  }


//-------------------------------------------------Lock Security------------------------------------------


  
void Lock(){
   int alert=1,dist;
   dist=ultrasonic();
   if(dist<100){    // we Assumed that there is wall at distance of 100 cm
      alert=0;
      ThingSpeak.writeField(myChannelNumber, 1, alert , myWriteAPIKey);
      Serial.println(".....Alert.....")
      alert=1;
    }  
  }
