#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Wire.h>
const int MPU=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
#include <SPI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LED1 = 7;
const int LED2 = 2;
const int GREEN = 10;
const int RED = 11;
const int BLUE = 9;

const int TEMP = A3;
const int LIGHT = A7;
const int AIR = A2;

int hour = 13;
int min = 2;
int sec = 0;


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(TEMP, INPUT);
  pinMode(LIGHT, INPUT);
  pinMode(AIR, INPUT);
}

float voltage = getVoltage(TEMP);//get voltage from the temperature pin
float degreesC = (voltage - 0.5) * 100.0;//calculate the celcius degree
float degreesF = degreesC * (9.0/5.0) + 32.0;// calculate the Fahrenheit degree

void loop() {
   // put your main code here, to run repeatedly:
   uint16_t temp = analogRead(TEMP);
   uint16_t light = analogRead(LIGHT);
   uint16_t air = analogRead(AIR);
   
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
   Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  //display.clearDisplay(); 
  display.setTextSize(1.5);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
   //Display static text
  display.print("Temp = ");
  display.println(temp);
  display.println(" ");
  display.print("LLvl = ");
  display.println(light);
  display.println(" ");
  display.print("Air = ");
  display.println(air);
  display.println(" ");
  display.display();


  //display.setCursor(45, 0);
 // display.println(temp);
  //display.println("  ");
  //display.setCursor(45, 10);
  //display.println(light);
  //display.println("  ");
  //display.setCursor(45, 20);
  //display.println(air);
  //display.println("  ");
  //display.display();

 
  Serial.print("voltage: ");//print volrage in serial monitor
  Serial.print(voltage);
  Serial.print("  deg C: ");//print the celcious degree in serial monitor
  Serial.print(degreesC);
  Serial.print("  deg F: ");//print the farenheit degree in serial monitor 
  Serial.println(degreesF);
 


  
  if (degreesF > 20) //if the temperature is greather than 40 degree, turn off the RGB
  {
  // Off (all LEDs off):
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
  }
 
  else if (degreesF == 20)
  // Blue (turn just the blue LED on): //  if the temperature is larger than 20 degrees and smaller than 40 degrees, show blue color 
 {
   digitalWrite(RED, LOW);
   digitalWrite(GREEN, LOW);
   digitalWrite(BLUE, HIGH);
  }
  
if (degreesF < 20)//if the temperature is below 20 degrees, show purple color
  {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH);
  }
 
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");

  Serial.print("Temp = ");Serial.print(temp, DEC);
  Serial.print(" Light Lvl = ");Serial.print(light, DEC);
  Serial.print(" Air = ");Serial.println(air, DEC);

    if (temp <= 19)
{
      digitalWrite(BLUE, HIGH);
}
else
{
      digitalWrite(BLUE, LOW);
      digitalWrite(RED, HIGH);
      digitalWrite(GREEN, LOW);
}

 

}

    float getVoltage(int pin)//get voltage 
 
 {
  return (analogRead(pin) * 0.004882814);// conver 0 to 1023 value to 0 to 5 value (the true voltage)
 }
