#include "DFRobot_OxygenSensor.h" //include library O2 DFrobot sesuai sensor yang digunakan

#define COLLECT_NUMBER    10             // collect number, the collection range is 1-100.
#define Oxygen_IICAddress ADDRESS_3
/*   iic slave Address, The default is ADDRESS_3.
       ADDRESS_0               0x70      // iic device address.
       ADDRESS_1               0x71
       ADDRESS_2               0x72
       ADDRESS_3               0x73
*/

DFRobot_OxygenSensor Oxygen; //inisialisasi object sensor O2

void setup() 
{
  //Serial ini kemudian digunakan untuk komunikasi antara sensor yg ada pada drone dengan
  //device monitor yang dipegang oleh operator
  Serial.begin(57600); //Menyesuaikan dengan baudrate telemetry RFD900 di 57600
  
  while(!Oxygen.begin(Oxygen_IICAddress)) { //Inisialisasi pin i2c untuk sensor O2
    delay(500);
  }

  
}

void loop()
{
  //pooling data dari sensor O2 dengan output berupa float 
  float oxygenData = Oxygen.getOxygenData(COLLECT_NUMBER); 

  //kemudian data yang didapatkan berupa float dipisahkan menjadi dua value integer
  //dari    :  
  //  oxygenData = 20.78
  //menjadi :
  // oxygen_data_1 = 20
  // oxygen_data_2 = 78
  //hal ini dilakukan untuk memudahkan proses pengiriman data

  int oxygen_data_1 = (int)oxygenData;
  int oxygen_data_2 =  (int) ((float)(oxygenData - oxygen_data_1) * 100);
  
  char msg_to_transmit[5]; //deklarasi variabel untuk string yang akan dikirimkan

  msg_to_transmit[0] = 'A'; //karakter pertama Digunakan huruf A sebagai header dari string msg
  msg_to_transmit[1] = (char)oxygen_data_1; //karakter kedua Digunakan untuk membawa value oxygen1 yang sebelumnya diubah menjadi huruf
  msg_to_transmit[2] = 'b'; //karakter ketiga Digunakan huruf b sebagai penanda tambahan
  msg_to_transmit[3] = (char)oxygen_data_2; //karakter keempat Digunakan untuk membawa value oxygen2 yang sebelumnya diubah menjadi huruf
  msg_to_transmit[4] = 'B'; //karakter kelima Digunakan huruf B sebagai penanda akhir dari sebuah string msg

  Serial.write(msg_to_transmit); //String msg dikirimkan ke telemetry melalui serial
  
  
}
