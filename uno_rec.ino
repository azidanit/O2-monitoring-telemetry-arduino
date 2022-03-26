//#include <SoftwareSerial.h>

//SoftwareSerial mySerial(7, 8); // RX, TX
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 4); 


char buff_rec[7];
char byte_rec;
short counter;
bool is_complete;
bool is_true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
//  mySerial.begin(57600);
  
  counter = 0;
  is_complete = false;
  is_true = false;

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Modul I2C LCD"); 
  lcd.setCursor(0, 1); 
  lcd.print("www.ardutech.com");
}

void loop() {
  // put your main code here, to run repeatedly:
//  mySerial.print("abcdef");
  
//  delay(10);
//  Serial.println(mySerial.available());
  if (Serial.available()){
//    Serial.println(mySerial.read());
    byte_rec = Serial.read();
//    Serial.println(byte_rec);
    if (byte_rec == 'a'){
      counter = 0;
      buff_rec[counter] = byte_rec;
      counter = 1;
      is_complete = false;
    }else if(byte_rec == 'd' && counter != 3){
      //drop all message
      counter = 0;
      is_complete = false;
    }else if(byte_rec == 'f' && counter == 5){
      is_complete = true;
      buff_rec[counter] = byte_rec;
    }else{
      buff_rec[counter] = byte_rec;
      counter++;
    }
//    
//    Serial.print(byte_rec);
//    Serial.print("  ");
//    Serial.print(is_complete);
//    Serial.print("  ");
//    Serial.println(buff_rec);

    if (is_complete && (buff_rec[1] == buff_rec[2] && buff_rec[2] == buff_rec[4])){
        is_true = true;
    }else{
        is_true = false;
    }

//    Serial.print(byte_rec);
//    Serial.print("  ");
//    Serial.print(is_complete);
//    Serial.print("  ");
//    Serial.print((int)is_true);
//    Serial.print("  ");
//    Serial.println(buff_rec);
  lcd.setCursor(0, 1); 
    lcd.print(buff_rec);
  }


}
