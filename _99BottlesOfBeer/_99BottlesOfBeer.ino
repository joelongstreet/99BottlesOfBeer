// configuration options
int ledSpeed    = 50;

// alcohol capturing
int alcoholSensor = A0;
int captureButton = 8;

// first shift register (A)
int aLatchPin   = 3;
int aClockPin   = 4;
int aDataPin    = 2;
byte aLeds      = 0;

// second shift register (B)
int bLatchPin   = 6;
int bClockPin   = 7;
int bDataPin    = 5;
byte bLeds      = 0;


void setup() 
{ 
  // establish button as an input
  pinMode(captureButton, INPUT);

  // establish registers as outputs 
  pinMode(aLatchPin, OUTPUT);
  pinMode(aDataPin,  OUTPUT);  
  pinMode(aClockPin, OUTPUT);
  pinMode(bLatchPin, OUTPUT);
  pinMode(bDataPin,  OUTPUT);  
  pinMode(bClockPin, OUTPUT);
}


void loop() 
{
  updateShiftRegisterA();
  updateShiftRegisterB();
  
  // when the button is pressed...
  if(digitalRead(captureButton)){
    // capture 100 alcohol values over the course of a
    // single second and report the greatest value
    int maxAlcoholValue = 0;
    for(int i=0; i<100; i++){
      int alcoholValue = analogRead(alcoholSensor);
      if(alcoholValue > maxAlcoholValue){
        maxAlcoholValue = alcoholValue;
      }
      delay(10);
    }
    
    // take the max alcohol value and map from potential low/high
    // to number of LEDs. Then light
    int ledCount = map(maxAlcoholValue, 5, 40, 0, 16);
    if(ledCount < 0) ledCount = 1;
    lightLEDs(ledCount);
    
    delay(2000);
    lightLEDs(0);
  }
}


void lightLEDs(int ledCount){
  // reset byte sets and shift registers to zero
  aLeds = 0;
  bLeds = 0;
  updateShiftRegisterA();
  updateShiftRegisterB();

  // count up and update the first register
  int countA = ledCount;
  if(ledCount > 8) countA = 8;
  for (int i = 0; i < countA; i++)
  {
    bitSet(aLeds, i);
    updateShiftRegisterA();
    delay(ledSpeed);
  }

  // count up and update the second register  
  int countB = 0;
  if(ledCount > 8) countB = ledCount - 8;
  for (int i = 0; i < countB; i++)
  {
    bitSet(bLeds, i);
    updateShiftRegisterB();
    delay(ledSpeed);
  }
}


void updateShiftRegisterA(){
  digitalWrite(aLatchPin, LOW);
  shiftOut(aDataPin, aClockPin, LSBFIRST, aLeds);
  digitalWrite(aLatchPin, HIGH); 
}


void updateShiftRegisterB(){
  digitalWrite(bLatchPin, LOW);
  shiftOut(bDataPin, bClockPin, LSBFIRST, bLeds);
  digitalWrite(bLatchPin, HIGH); 
}
