#include <MI0283QT2.h>
#include<stdlib.h>
#include <Wire.h>
#include <Dictator.h>
#include <Button.h>
#include <avr/interrupt.h>

//#define TP_EEPROMADDR (0x00) //eeprom address for calibration data

MI0283QT2 lcd;
Button btn;

uint16_t BackgroundColor = RGB( 255, 255, 255);
uint16_t ButtonColor = RGB( 0, 0, 0);
uint16_t TextColor = RGB( 255, 255, 255);
uint16_t LogColor = RGB( 255, 255, 255);
uint16_t WarnColor = RGB( 255, 255, 0);
uint16_t ErrorColor = RGB( 255, 0, 0);
int ButtonRadius = 5;
int MenuButton1 = 126;
int MenuButton2 = 125;
int MenuButton3 = 124;
String Debugtext = "loading";
int I2CInterruptPin = 2;
int count = 0;




void DrawButton(String Text, int x, int y, int width, int height) {
  //lcd.fillRect(x, y, x+width, y+height, ButtonColor);

  lcd.fillCircle(x+ButtonRadius, y+ButtonRadius, ButtonRadius, ButtonColor);
  lcd.fillCircle(x+width-ButtonRadius, y+ButtonRadius, ButtonRadius, ButtonColor);
  lcd.fillCircle(x+ButtonRadius, y+height-ButtonRadius, ButtonRadius, ButtonColor);
  lcd.fillCircle(x+width-ButtonRadius, y+height-ButtonRadius, ButtonRadius, ButtonColor);

  lcd.fillRect(x, y+ButtonRadius, x+ButtonRadius, y+height-ButtonRadius, ButtonColor);
  lcd.fillRect(x+ButtonRadius, y, x+width-ButtonRadius, y+height, ButtonColor);
  lcd.fillRect(x+width-ButtonRadius, y+ButtonRadius, x+width, y+height-ButtonRadius, ButtonColor);

  lcd.drawText(x+10, y+(height/2)-5, Text, 1, TextColor, ButtonColor);
}

void DrawTC(int x, int y) {
  lcd.drawRect(x, y, x+180, y+40, ButtonColor);
  lcd.drawText(x+10, y+10, "0:00:10:24", 2, ButtonColor, TextColor);
}


void ButtonPressed(){
  sei(); // to prevent the wire library from not being able to use interrupts as well
  //Serial.print("interrupt");
  //Debugtext = "interrupt";
  Debugtext = "";

  Wire.requestFrom(32, 2); 
  //int count = 0;
  while(Wire.available())  
  {  
    char c = Wire.read();
    //Debugtext += "Byte ";// + count;
    //Debugtext += String(c, DEC) + "\n";
    //    Debugtext += "" + String(c, HEX);

    if (c == MenuButton1)
      MenuButton1Pressed();
    if (c == MenuButton2)
      MenuButton2Pressed();
    if (c == MenuButton3)
      MenuButton3Pressed();
  }
}

void MenuButton1Pressed () {
  Debugtext += "MenuButton1Pressed\n";
  //Serial.println("MenuButton1Pressed");
}

void MenuButton2Pressed () {
  Debugtext += "MenuButton2Pressed\n";
  // Serial.println("MenuButton2Pressed");
}

void MenuButton3Pressed () {
  Debugtext += "MenuButton3Pressed\n";
  //Serial.println("MenuButton3Pressed");
}

void setup()
{
  lcd.init(2); //spi-clk = Fcpu/2
  Wire.begin();  

  //Serial.begin(9600); 
  //Serial.println("starting");

  interrupts();
  pinMode(I2CInterruptPin, INPUT);
  digitalWrite(I2CInterruptPin, HIGH);       // turn on pullup resistors
  attachInterrupt(0, ButtonPressed, CHANGE);

  interrupts();  // enable interrupts
  sei(); // enable interrupts
  lcd.clear(BackgroundColor);  //clear screen
  lcd.led(10); // DIm the LCD down as the arduino alone cannot power it fully -> requires better power supply in the future
  lcd.setOrientation(180); // Rotate the LCD 180°
/*
  DrawButton("Test1", 10, 1, 90, 25);
  DrawButton("Test2", 115, 1, 90, 25);
  DrawButton("Test3", 220, 1, 90, 25);

  DrawButton("Test4", 10, 213, 90, 25);
  DrawButton("Test5", 115, 213, 90, 25);
  DrawButton("Test6", 220, 213, 90, 25);
*/
  //DrawTC(40, 100);

  Debugtext = "running ";
  
  btn = Button("TestButton", 100, 100, 90, 25);
  btn.Draw(lcd);
}

void loop()
{
  lcd.drawText(10, 40, count + Debugtext, 1, ButtonColor, TextColor);
  //lcd.drawText(100, 60, Debugtext, 1, ButtonColor, TextColor);
  count++;
  Debugtext = "";
  delay(100);
}



























