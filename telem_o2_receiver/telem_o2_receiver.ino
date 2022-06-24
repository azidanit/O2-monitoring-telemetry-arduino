#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 4); 

char buff_rec[5];
char byte_rec;
short counter, ping_counter, fail_counter;
bool is_complete, is_true, is_connected;
int oxygen_data_1, oxygen_data_2;

void setup() {
  //Serial ini kemudian digunakan untuk komunikasi antara sensor yg ada pada drone dengan
  //device monitor yang dipegang oleh operator
  Serial.begin(57600); //Menyesuaikan dengan baudrate telemetry RFD900 di 57600

  //inisialisasi varibel
  counter = ping_counter = fail_counter = 0;
  is_complete = false;
  is_true = false;
  is_connected = false;
  oxygen_data_1 = oxygen_data_2 = 0;

  //Menulis Teks di LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Initiating...");
  lcd.setCursor(0, 1);
  lcd.print("Telemetry...."); 
  lcd.setCursor(0, 2); 
  lcd.print("Please Wait..");

  delay(10000); // menunuggu 10 detik untuk telemetry siap digunakan

  is_connected = true;
  
  lcd.clear();
  lcd.print("---Data Telem---");
  lcd.setCursor(0, 1);
  if(is_connected)
    lcd.print("Connected    "); 
  else
    lcd.print("Not Connected"); 
  lcd.setCursor(0, 2); 
  lcd.print("  O2 Reading :  ");
}

void loop() {
  //menerima data per byte (setiap karakter)
  if (Serial.available()){
    byte_rec = Serial.read();
    if (byte_rec == 'A'){
      buff_rec[counter] = byte_rec;
      counter = 1;
      is_complete = false;
    }else if(byte_rec == 'b' && counter != 2){
      //drop all message, karena tidak urut atau data ada yang hilang
      counter = 0;
      is_complete = false;
    }else if(byte_rec == 'B' && counter == 4){
      is_complete = true;
      buff_rec[counter] = byte_rec;
    }else{
      buff_rec[counter] = byte_rec;
      counter++;
    }

  
    if (is_complete){ //apabila string msg yang diterima lengkap, maka lanjut proses data untuk kemudian ditampilkan di LCD
      oxygen_data_1 = buff_rec[1];
      oxygen_data_2 = buff_rec[3];
      ping_counter = 0;
    }else{ //apabila data string msg belum lengkap, tambah increment variabel ping counter, 
      //untuk memonitoring delay antara
      //pesan dari drone ke device monitoring operator
      //semakin kecil ping, semakin cepat koneksi
      if(ping_counter<999){
        ping_counter++; 
        fail_counter = 0;
      }else{
        if(fail_counter > 3000) //apabila ping counter melebihi 999 + 3000 maka dianggap koneksi terputus
          is_connected = false;
        else
          fail_counter++;
      }
    }
  }


  //Menulis Teks di LCD
  lcd.setCursor(0, 0);
  lcd.print("---Data Telem---");
  lcd.setCursor(0, 1);
  
  if(is_connected) //apabila connected, maka tulis connected dan menampilkan ping
  //jika tidak maka tulisan menjadi Not Connected
    lcd.print("Connected    "); 
    char ping_str[16];
    sprintf(ping_str, "%d",ping_counter);
    lcd.print(ping_str);
  else
    lcd.print("Not Connected"); 
  lcd.setCursor(0, 2); 
  lcd.print("  O2 Reading :  ");
  lcd.setCursor(0, 3); 

  char reading_str[16];
  sprintf(reading_str, "     %d,%d%%     ",oxygen_data_1,oxygen_data_2);
  lcd.print(reading_str);

  
}
