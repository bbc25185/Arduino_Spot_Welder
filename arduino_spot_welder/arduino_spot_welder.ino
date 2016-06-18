// Arduino Battery Spot Welder
// Project Page: http://www.instructables.com/id/DIY-Arduino-Battery-Spot-Welder/

const byte mosfetPin = 5;  //pin that controls the mosfet drivers
const byte ledPin = 4;     //pin to control the status led
const byte tasterPin = 2;  //pin to trigger a pulse with the foot switch
const byte sensorPin = A7; //pin where the potentiometer value is read
#define DIVIDER 20.48   //Divider for the maximun pulse time

int sensorValue = 0;  //initalize sensor value with zero
int pulse_time;   //variable to store the pulse time
int pulse_short;  //variable to store the short pulse time



// Define the possible numbers to be displayed on the 7-segment display in an array
//            0           1          2          3          4          5          6          7          8          9          .        empty
byte z[12]={B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B00000001, B000000};


//**************************************  SETUP Routine  ****************************************
void setup() {

pinMode(tasterPin,INPUT);  // set tasterPin as Input
pinMode(mosfetPin,OUTPUT);  // set mosfetPin as Output
pinMode(ledPin,OUTPUT);  // set ledPin as Output
pinMode(sensorPin,INPUT);  // set sensorPin as Input

// 7 Segment Pins
for (int i=6; i <= 13; i++) // 6-13 are set as Outputs
    pinMode(i,OUTPUT);


//Light up the led and the display 2 seconds to verify correct placement and function
  digitalWrite(ledPin, HIGH);
  for (byte i=6; i <= 13; i++) // 6-13 are set to High which means all segments on
    digitalWrite(i, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);
  for (byte i=6; i <= 13; i++) // 6-13 are set to LOW which means all segments off
    digitalWrite(i, LOW);

}

//********************************  LOOP Routine  ***********************************
void loop() {

time_display();

// impuls activated by taster
if (digitalRead(tasterPin)==HIGH){
    DoppelImpuls();
    do {
  digitalWrite(mosfetPin, LOW);
  digitalWrite(ledPin, LOW);
  delay (100);
} while (digitalRead(tasterPin)==HIGH);
  delay(500);
                                  } 
else 
{
    digitalWrite(mosfetPin, LOW);
    digitalWrite(ledPin, LOW);
}

}

//***********************************  Show Number Subroutine  **********************************

void segmente(byte n) {
// controll all 7 Segments
 
  for(int k=6; k <= 13; k++) {
    if((n & B10000000) > 0)
      digitalWrite(k, HIGH);
    else 
      digitalWrite(k, LOW);
    n = n << 1;
  } 
}

//***************************************  No-Stop Show Display Number  *****************************  
void time_display(){  // 7 segment pulse time displaying
static byte _STATO=0;  
static unsigned long _LAST_TIMER=0;
static byte a,b;
pulse_time = analogRead(sensorPin)/DIVIDER;           //Old value: 51.25

switch (_STATO){                        //------Check the display STATE
  case 0:                               //------1 number
    if (b==0) {_STATO = 2;}             //Skip the first number if 0
    else {
      segmente(z[b]);
      if ((millis()-_LAST_TIMER) > 400) {         //Wait for 400ms
        if (a==b) {_STATO = 1;} else {_STATO = 2;}  //Check if the 2 number is equal or not
        _LAST_TIMER = millis();
      }
    }
  break;
  case 1:                               //------empty space if neccessary
    segmente(z[11]);                    // the empty space is used if the 2 number is identical
    if ((millis()-_LAST_TIMER) > 100) {         //Wait 100ms
        _STATO = 2;
        _LAST_TIMER = millis();
      }
  break;
  case 2:                               //------2 number
    segmente(z[a]);
    if ((millis()-_LAST_TIMER) > 400) {       //wait 400ms
        _STATO = 3;
        _LAST_TIMER = millis();
      }
  break;
  case 3:                               //------point pause
    segmente(z[10]);
    if ((millis()-_LAST_TIMER) > 700) {   //wait 700ms
        _STATO = 0;                       //Reset the show of the display
        _LAST_TIMER = millis();
        b=(pulse_time%100)/10;    //recalculate the 2 number showed in the display
        a=pulse_time%10;
      }
  break;
  default:                             //------_STATO reset
    _STATO = 0;
  break;
  } 
}

//-------------------------------------------------------------------------------------------

void DoppelImpuls(){  
 
  pulse_time = analogRead(sensorPin)/DIVIDER;  //makes a pulse of max 20 mS because SensorValue can max become 1023
  pulse_short = (pulse_time/8);     // Impuls with 1/8 of the time of pulse_time
 
  if (pulse_short < 1) //makes the short pulse to be at least one milli second 
     pulse_short = 1; 
 
  digitalWrite(ledPin, HIGH);
  digitalWrite(mosfetPin, HIGH);
  delay(pulse_short);
  digitalWrite(mosfetPin, LOW);
  delay(pulse_short);      
    digitalWrite(mosfetPin, HIGH);
  delay(pulse_time);   
  digitalWrite(mosfetPin, LOW);
  delay(sensorValue);
  digitalWrite(ledPin, LOW);
  
}
