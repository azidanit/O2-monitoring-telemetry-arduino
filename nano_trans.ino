#include <SoftwareSerial.h>

//SoftwareSerial mySerial(7, 8); // RX, TX


char buff_rec[6];

char buff_trans[6];

char byte_rec;
short counter;
char to_send;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
//  mySerial.begin(57600);
  
  counter = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  
//  mySerial.print("a21def");
  Serial.write('a');
  delay(20);
  Serial.write((char)(65+counter));
  delay(20);
  Serial.write((char)(65+counter));
  delay(20);
  Serial.write('d');
  delay(20);
  Serial.write((char)(65+counter));
  delay(20);
  Serial.write('f');
  
  delay(50);

  counter++;
  if (counter > 26)
    counter = 0;

}
