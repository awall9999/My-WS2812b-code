/*
 * MY WS2812b Control Code
 * This code is for 16Mhz Arduinos. 
 * (C) Alain MAUER
 * www.alainsprojects.com
 */

#define HALT            asm("nop\n") // Do nothing, just waste 16,5ns
#define BITHIGH         asm ("sbi %0, %1 \n": : "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB4) ); //Assembler code to Set Port D12 (B4) to 1. This is the Data in Pin of your LED
#define BITLOW          asm ("cbi %0, %1 \n": : "I" (_SFR_IO_ADDR(PORTB)), "I" (PORTB4) ); //Assembler code to Set Port D12 (B4) to 0  This is the Data in Pin of your LED

int LN = 20; // Numbers of LEDs used
int LB=155; //Brightness LED

byte LEDPos[100]; // Led Array. Always a higher Number than LN


void Resetall(){
  // Set all the LEDs color to 0. 0=off
  noInterrupts();
  for (int i = 0; i <= (24*LN); i++) Zero();
  interrupts();
  Treset(); //50us delay to end the transfer of data
}

void Treset(){
  // 50uSec delay loop . This signals the LEDs that all the Data are transfered
  long int time_now = micros();
  BITLOW;
do {
  
} while ((unsigned long)(micros() - time_now) < 50);  
}

void One(){
cli(); // Disable Interupts
BITHIGH;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT; // 0,75us (0.0625 * 12)  ---  0.0625us one cycle on 16MHZ
BITLOW;HALT;HALT;HALT;HALT;HALT;HALT; // 0,375us (0.0625 * 6) --- 0.0625us one cycle on 16MHZ
sei(); // Enable Interupts
}

void Zero(){
cli(); // Disable Interupts
BITHIGH;HALT;HALT;HALT;HALT;HALT;  // 0,3125us (0.0625 * 5) --- 0.0625us one cycle on 16MHZ
BITLOW;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT;HALT; //0,8125us (0.0625 * 13) --- 0.0625us one cycle on 16MHZ
sei(); // Enable Interupts
}

void LEDcall(byte LC){
  byte r,g,b;
  // define the colors you need here. You can also define colors with a different brightnes here. Example LC == 15
  if (LC == 0) {r=0;g=0;b=0;}       //Blanc
  if (LC == 1) {r=LB;g=0;b=0;}      //Red
  if (LC == 2) {r=0;g=LB;b=0;}      //Green
  if (LC == 3) {r=0;g=0;b=LB;}      //Blue
  if (LC == 4) {r=LB;g=LB;b=0;}      //Yellow
  if (LC == 5) {r=0;g=LB;b=LB;}      //Cyan
  if (LC == 6) {r=LB;g=0;b=LB;}      //Magenta
  if (LC == 7) {r=LB;g=LB;b=LB;}      //White

  if (LC == 8) {r=LB;g=(LB/2);b=0;}      //Orange
  if (LC == 9) {r=(LB/2);g=0;b=LB;}      //Violet

  if (LC == 15) {r=LB/2;g=0;b=0;}      //Red 50% brighness
  noInterrupts();
  // send the 24 Bits of one Led. Green,Red and Blue
  // There is probably an more elegant way to do it, but it works
  for (int i = 0; i <= 7; i++){
    if (bitRead(g,7- i)) One(); else Zero();
    
  }
  for (int i = 0; i <= 7; i++){
    if (bitRead(r,7- i)) One(); else Zero();
    
    
  }
  for (int i = 0; i <= 7; i++){
    if (bitRead(b,7- i)) One(); else Zero();
  }
  interrupts();
 }

void UpdateLEDs() {
                  // This part repeats the Datatransfer for the number of LEDs defined
                  for (int LEDCounter = 0; LEDCounter <= LN; LEDCounter++) {LEDcall(LEDPos[LEDCounter]);} // SerialOut of the Led Data
                  Treset(); //50us delay to end the transfer of data
}






void setup() {
  
  DDRB = DDRB | B00010000; // Set PortB4 as Output. Ignores all the other Pins of PortB
  Resetall(); // All LEDs off

  // LEDPos [positon of what LED you want to set] = Color of this LED (0=off 1=red 2=green ...... defined in the void LEDCall. Feel free to add or change the colors)
  
  LEDPos[0]=1; //Led nr one goes on Red
  LEDPos[1]=2; //Led nr two goes on Green
  LEDPos[2]=3; //Led nr Three goes on Blue
  LEDPos[4]=3; //Led nr four goes on White
  UpdateLEDs(); // Execute the settings
  
}

void loop() {
  // change color from 0 (off) to 7 (white) of led 10 each second
  for (int i = 0; i <= 7; i++) {
                                LEDPos[9]=i; // Led nr 10 goes on color 0 to 7
                                UpdateLEDs(); // Execute the settings
                                delay(1000); // wait a second
                               }

}
