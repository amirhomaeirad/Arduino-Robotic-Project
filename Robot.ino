
//Sensor
int s1 = 2; //Sensor1 ==> Pin2
int s2 = 3; //Sensor2 ==> Pin3
int s3 = 4; //Sensor3 ==> Pin4
int s4 = 5; //Sensor4 ==> Pin5
boolean sens1,sens2,sens3,sens4; // Variables for Sensor


// Motor & L298
//Channel A
int IN1 = 7; // IN1 ==> Pin7
int IN2 = 8; // IN2 ==> Pin8
int ENA = 9; // Enable A ==> Pin9

//Channel B
int IN3 = 13; // IN3 ==> 13
int IN4 = 12; // IN4 ==> 12
int ENB = 11; // Enable B ==> Pin11

int MotorSpeedA=0;
int MotorSpeedB=0;


//UltraSonic SRF04
int PingPin = 6; // TrigPin ==> Pin6
int EchoPin = A0; // EchoPin ==> A0
unsigned long duration, cm;


//Sound of Robot
 #include "pitches.h"
 
 int tonePin = 10; // connect Speaker ==> Pin10
 
 //Sound on Start Up
int mobile_notes[]= {NOTE_C4, NOTE_G3,NOTE_GS3, NOTE_AS3, NOTE_C4};
int mobile_Durations[]={4,4,8,8,4};
//Damage Sound when Too Close
int damage_notes[] = {NOTE_A3, NOTE_F3, NOTE_A2, 0, NOTE_A3, NOTE_F3, NOTE_A2, 0, NOTE_A3, NOTE_F3, NOTE_A2, };
int damage_Durations[] = {8, 8, 8, 12, 8, 8, 8, 12, 8, 8, 8, };



void setup(){
  //Activate Serial Monitor
 Serial.begin(9600);

//Sensor Setting
pinMode(s1, INPUT);
pinMode(s2, INPUT);
pinMode(s3, INPUT);
pinMode(s4, INPUT);

//L298 Setting
//Channel A  Motor in Right Side
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  digitalWrite(ENA,LOW);
//Channel B  Motor in Left Side
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  digitalWrite(ENA,LOW);

//UltraSonic Setting
 pinMode(PingPin, OUTPUT);
 pinMode(EchoPin, INPUT);
 
 
 //Sound on StartUP
 music();
}


void loop(){
  
 //Enable UltraSonic Module
UltraSonic();

SensorDigitalRead();

//if You Want to Show Sensor Result
//SensorPrint();


//Enable Alarm on Robot
alarm();


//Move forward ==> Sensor1 and Sensor2 are in Black line
//Sensors in White line ==> 0 Value
//Sensors in Black line ==> 1 Value


//Robot Turn a little ro Right
if( sens4 && !sens1){ 
   TurnRight();
 }

//Robot Turn a little ro Left 
  else if( !sens4 && sens1 ){
    TurnLeft();
}

//Move Forward when Sens2 and sens3 in Black line
else if( sens2 && sens3){
  MoveForward(); 
  } 
 else if( (sens2 || sens3) &&  !sens1 && !sens4){
   MoveForward();}
   
//MoveBack When all sensor in White Area  
else if( !sens1 && !sens2 && !sens3 && !sens4){
analogWrite(ENA,0);
analogWrite(ENB,0);
  MoveBackWard();
    }
   
}


// Read Value From All Sensor
int SensorDigitalRead(){
 sens1 = digitalRead(s1);
 sens2 = digitalRead(s2);
 sens3 = digitalRead(s3);
 sens4 = digitalRead(s4);
 return sens1,sens2,sens3,sens4;   
}

// Print Sensors Value on Screen
void SensorPrint(){
  Serial.print("S1= ");
  Serial.print(sens1);
    Serial.print(" S2= ");
    Serial.print(sens2);
        Serial.print(" S3= ");
        Serial.print(sens3);
            Serial.print(" S4= ");
            Serial.print(sens4);
                    Serial.println();
      Serial.print("Distance is= ");
      Serial.println(cm);
      
delay(200);}

//Function for Moving Forward
void MoveForward(){

MotorSpeedA= 200;
MotorSpeedB= 200;
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
    
analogWrite(ENA,MotorSpeedA);
analogWrite(ENB,MotorSpeedB);
}

//function for Moving BackWard
void MoveBackWard(){
MotorSpeedA=  150;
MotorSpeedB= 150;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
    
analogWrite(ENA,MotorSpeedA);
analogWrite(ENB,MotorSpeedB); 
  
}

// Function for Turning Robot to left
void TurnLeft(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  
    MotorSpeedA = 200;
    MotorSpeedB = 200;
    
    analogWrite(ENA,MotorSpeedA);
    analogWrite(ENB,MotorSpeedB);
}

// Function For Turning Robot to Right
void TurnRight(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  
    MotorSpeedA = 200;
    MotorSpeedB = 200;
    
    analogWrite(ENA,MotorSpeedA);
    analogWrite(ENB,MotorSpeedB);
}

// Function for Calculate Ultrasonic Distance
int UltraSonic(){
  
  digitalWrite(PingPin,LOW);
  delayMicroseconds(2);
  digitalWrite(PingPin,HIGH);
  delayMicroseconds(5);
  digitalWrite(PingPin, LOW);
   
  duration = pulseIn(EchoPin, HIGH); // Measure Time of Fligh in Micro Secondes
  
  cm = MicroStoCm(duration);
  
  return cm;  
}

//Function For Convert MicroSecondes to Centimeter
unsigned long MicroStoCm( unsigned long MicroS){
 // Speed of Sound in the Air 340 m/s
// Result is ==> 29 microseconds / cm

return (MicroS/29)/2;
}
  

//Damage Sound
void damage(){
  
     for (int thisNote = 0; thisNote < 11; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int damage_Duration = 1000/damage_Durations[thisNote];
    tone(tonePin, damage_notes[thisNote],damage_Duration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = damage_Duration * 1.30;
    delay(pauseBetweenNotes);

  }
  
}

// Start Sound
void music(){
  for(int i=0 ; i<3;i++){
       for (int thisNote = 0; thisNote < 5; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int mobile_Duration = 1000/mobile_Durations[thisNote];
    tone(tonePin, mobile_notes[thisNote],mobile_Duration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = mobile_Duration * 1.43;
    delay(pauseBetweenNotes);
  }  
  }
}

// Function for avoid an obstacle 
void alarm(){
  delay(20);
 if(cm < 5){
   // Turn Motors OFF
   digitalWrite(ENA,LOW);
   digitalWrite(ENB,LOW);
  damage(); // play Damage Sound
 }

}
