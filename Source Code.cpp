
#define BLYNK_TEMPLATE_ID "TMPL6qv5b1dl-"
#define BLYNK_TEMPLATE_NAME "Teknik Akuisisi Data"
#define BLYNK_AUTH_TOKEN "hFJ_AbJ8X3CRoBuC7_sm6fapTx5lQfUx"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Servo.h>
Servo myservo;

char ssid[] = "Nusa kos";  // Ganti dengan nama WiFi Anda
char pass[] = "1223345";  // Ganti dengan password WiFi Anda

int echoPin = D6;
int trigPin = D5;
int servoPin = D4;

int servostandby = 100;
int servoaktif = 90;
int jarak = 10; // Mengubah jarak menjadi 10 cm

BlynkTimer timer; // Buat objek BlynkTimer

void sendSensorData(); // Deklarasikan fungsi

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Menggunakan token yang dideklarasikan sebelumnya
  myservo.attach(servoPin);
  myservo.write(servostandby);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  timer.setInterval(1000L, sendSensorData); // Atur interval timer untuk memanggil fungsi sendSensorData setiap 1 detik
}

void loop(){
  Blynk.run();
  timer.run(); // Jalankan timer
}

void sendSensorData() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  unsigned long duration = pulseIn(echoPin, HIGH);
  float jarakukur = duration / 29.0 / 2.0;
  Serial.print("distance: "); // Mencetak "distance: " ke serial monitor
  Serial.print(jarakukur);    // Mencetak hasil pengukuran variabel "jarakukur"
  Serial.println(" cm");      // Mencetak " cm" ke serial monitor setelah variabe
 
  if (jarakukur <= jarak){
    myservo.write(180); // Memutar servo ke posisi 180 derajat
    delay(400);
    myservo.write(servostandby);
    delay(800);
  }

  // Mengirim data jarak ke widget V1
  Blynk.virtualWrite(V1, jarakukur);

  // Mengirim data status servo ke widget V0
  int statusServo = myservo.read();
  Blynk.virtualWrite(V0, statusServo);
}
