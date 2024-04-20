#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9,10); //Rx Tx
LiquidCrystal_I2C lcd(0x27, 16, 4);
bool messageSentHigh=false;
bool messageSentLow=false;
// bool messageSentMid=false;

 
void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();  
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM IS ON ");
  lcd.print("");
  delay(3000);
  lcd.clear();
}
 
void loop() {
  int value = analogRead(A0);
  Serial.println(value);
  if (value > 950) {
    messageSentHigh=false;
    digitalWrite(2, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is ON ");
    lcd.print("Moisture : LOW ");
    if(!messageSentLow){
      mySerial.println("AT+CMGF=1");   // text mode on
      delay(1000);                      
      mySerial.println("AT+CMGS=\"+917020425327\"\r"); 
      delay(1000);
      mySerial.println("Pump is turned On as moisture Content is low.");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
      messageSentLow=true;
      // Serial.println("Message sent!");
    }
    
  } else {
    digitalWrite(2, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is OFF");
  }
 
  if (value < 500) {
    messageSentLow=false;
    // messageSentMid=false;
    lcd.setCursor(0, 1);
    lcd.print("Moisture : HIGH");
    if(!messageSentHigh){
      mySerial.println("AT+CMGF=1");   // text mode on
      delay(1000);                      
      mySerial.println("AT+CMGS=\"+917020425327\"\r"); 
      delay(1000);
      mySerial.println("Water in Field is More than Required\n Crop will be damanged\nPlease look into it.");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
      messageSentHigh=true;
    }
  } else if (value > 300 && value < 950) {
    // messageSent=false;
    messageSentHigh=false;
    messageSentLow=false;
    lcd.setCursor(0, 1);
    lcd.print("Moisture : MID ");
  } else if (value > 950) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture : LOW ");
  }
}
