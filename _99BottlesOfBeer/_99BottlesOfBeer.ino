// configuration options
int ledSpeed    = 50;
int loAlcValue  = 0;
int hiAlcValue  = 100;

// calibrate low alcohol readings
int busyIndicator   = 10;
int calibrateButton = 9;

// alcohol capturing
int maxAlcoholVal   = 0;
int alcoholSensor   = A0;
int captureButton   = 8;
boolean buttonDown  = false;

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
  
  pinMode(busyIndicator, OUTPUT);

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
  
  // allow calibration for the low alcohol value
  if(digitalRead(calibrateButton)){
    digitalWrite(busyIndicator, 1);
    // take calibration over the course of a second
    for(int i=0; i<100; i++){
      int val = analogRead(alcoholSensor);
      if(val > loAlcValue) loAlcValue = val;
      delay(10);
    }
  }
  
  // when the button is pressed, start capturing alcohol values
  if(digitalRead(captureButton)){
    digitalWrite(busyIndicator, 1);
    buttonDown = true;
    int alcoholVal = analogRead(alcoholSensor);
    if(alcoholVal > maxAlcoholVal){
      maxAlcoholVal = alcoholVal;
    }
  } else if(buttonDown){
    // when the button is released...
    // take the max alcohol value and map from potential low/high
    // to number of LEDs
    int ledCount = map(maxAlcoholVal, loAlcValue, hiAlcValue, 0, 16);
    if(ledCount <= 0) ledCount = 1;
    lightLEDs(ledCount);
    
    // reset
    maxAlcoholVal = 0;
    buttonDown = false;
    delay(2000);
    lightLEDs(0);
  }
  
  digitalWrite(busyIndicator, 0);
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
