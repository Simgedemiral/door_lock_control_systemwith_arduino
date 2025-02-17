#include <Wire.h>

#include <SoftwareSerial.h>

#include <MFRC522.h>
#include <Servo.h>
#include <SPI.h>


int RST_PIN = 9; //RC522 modülü reset pinini tanımlıyoruz.
int SS_PIN = 10; //RC522 modülü chip select pinini tanımlıyoruz.
int servoPin = 8; //Servo motor pinini tanımlıyoruz.


Servo motor; //Servo motor için değişken oluşturuyoruz.

MFRC522 rfid(SS_PIN, RST_PIN); //RC522 modülü ayarlarını yapıyoruz.
byte ID[4] = {214,80,110,175};
 //Yetkili kart ID'sini tanımlıyoruz. 
void setup() {
  motor.attach(servoPin); //Servo motor pinini motor değişkeni ile ilişkilendiriyoruz.
  Serial.begin(9600); //Seri haberleşmeyi başlatıyoruz.
  SPI.begin(); //SPI iletişimini başlatıyoruz.
  rfid.PCD_Init(); //RC522 modülünü başlatıyoruz.

}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) //Yeni kartın okunmasını bekliyoruz.
    return;
  if (!rfid.PICC_ReadCardSerial()) //Kart okunmadığı zaman bekliyoruz.
    return;
  if (rfid.uid.uidByte[0] == ID[0] && //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
    rfid.uid.uidByte[1] == ID[1] &&
    rfid.uid.uidByte[2] == ID[2] &&
    rfid.uid.uidByte[3] == ID[3]) {
    Serial.print("Kapi acildi");
    ekranaYazdir();
    motor.write(180); //Servo motoru 180 dereceye getiriyoruz.
    delay(3000);
    motor.write(0); //Servo motoru 0 dereceye getiriyoruz.
    delay(1000);
  } else { //Yetkisiz girişte içerideki komutlar çalıştırılır.
    Serial.print("Yetkisiz Kart");
    ekranaYazdir();
  }
  rfid.PICC_HaltA();
}
void ekranaYazdir() {
  Serial.print("ID Numarasi: ");
  for (int sayac = 0; sayac < 4; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.print("");
}
