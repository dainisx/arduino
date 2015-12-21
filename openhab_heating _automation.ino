#include <OneWire.h>                //pievienojam OneWire bilioteeku
#include <Time.h>                  //pievienojam laika un datuma bilioteeku
#include <DallasTemperature.h>    //pievienojam DS18B20 sensoru bilioteeku
#include <Wire.h>                // pievienojam biblioteeku lai nolasiitu un atteelotu analogo ieju datus kur piesleegts fotorezistoors
#include <DS1307RTC.h>          //pievienojam Real Time modulja bilioteeku
#include <NewPing.h>           //pievienojam distances meeritaaja bilioteeku

#define TRIGGER_PIN 8
#define ECHO_PIN 9
#define MAX_DISTANCE 450
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
#define ONE_WIRE_BUS 10

int DistanceCm;
int relejs_sukni = 11;      // relejs suuknjiem uz 12 PINu
int relejs_deglis = 12;      // relejs deglim uz 11 PINu
int apkure_in = 13;        // Digitaalaa ieeeja kur nolasiim vai sanjemam signaalu par apkures iesleegshanu no telpas temperatuuras deveeja vai no releja, ko vada OPENHAB, vai sleedzja apkures telpaa
int val = 0;

int val_ldr = 0;               // Foto rezistora veertiibu uzstaadaam = 0
int val_ldr1 = 0;
int relejs_varstam = 2;       // relejs vārstam uz 2 PINu
int pirPin = 3;              // kustiibas detektora ieeja uz 3 PINu
int relejs_prozektori = 4;  //relejs prožektoriem uz 4 PINu
int relejs_Led_lentas = 5; //relejs Led lentaam uz 5 PINu
int in_apg = 6;
int ldr = A0;             //fotorezistors analogajaa 0 PINAa prieksh aara apgaismojuma
int ldr1 = A1;           //fotorezistors analogajaa 1 PINAa prieksh iekshas apgaismojuma
//int vartu_relejs = 11;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Apkures_katla_temp = { 0x28, 0xB8, 0xF4, 0xB5, 0x06, 0x00, 0x00, 0xC0 };  // Apkures katla DS18B20 sensora adrese
DeviceAddress Boilera_temp = { 0x28, 0x80, 0xC8, 0xB5, 0x06, 0x00, 0x00, 0x29 };       // Ūdens boilera DS18B20 sensora adrese
DeviceAddress Pagalma_temp = { 0x28, 0xDC, 0x17, 0xB5, 0x06, 0x00, 0x00, 0x4B };      // Pagalma DS18B20 sensora adrese

void setup() {

  digitalWrite(ECHO_PIN, LOW);
  digitalWrite(apkure_in, HIGH);                 //uzstaadam ieejosho apkures deveeja signaalu HIGH liimeni, jo HIGH liimenis dod informaaciju, ka nav nostraadaajis telpas temperatuuras deveejs(SALUS), vai apkures palaishanas sleedzis vai no OpenHab iesleedzams relejs
  pinMode(apkure_in, INPUT);                    //uzstādām, pin ir ieeja
  pinMode(relejs_deglis, OUTPUT);              //uzstādām, pin ir izeja
  pinMode(relejs_sukni, OUTPUT);              //uzstādām, pin ir izeja
  pinMode(relejs_varstam, OUTPUT);           //uzstādām, pin ir izeja
  pinMode(relejs_prozektori, OUTPUT);       //norādam, ka relejs led prožektoriemm ir izeja
  pinMode(relejs_Led_lentas, OUTPUT);      //norādam, ka relejs led lentaam ir izeja
  pinMode(pirPin, INPUT);                 //norādam, ka kustības detektors ir ieeja
  pinMode(in_apg, OUTPUT);               //norādam, ka foto rezistors ir analogaa ieeja
  pinMode(ldr, INPUT);                  //noraadam, ka fotorezistori ir ieeja
  pinMode(ldr1, INPUT);                //noraadam, ka fotorezistori ir ieeja
  delay(1000);                        // stabilizeejam PIR sensoru
   digitalWrite(relejs_sukni, HIGH);  //uzstaadaam releju suuknjiem izsleegtaa staavoklii
  digitalWrite(relejs_deglis, HIGH);  //uzstaadaam releju granulu deglim izsleegtaa staavokli          
  int relejs_varstam = HIGH;           //uzstaadaam releju vaarstam izsleegtaa staavoklii
  int relejs_prozektori = HIGH;       //uzstaadaam releju prozektoriem izsleegtaa staavoklii
  int relejs_Led_lentas = HIGH;      //uzstaadaam releju Led lentaam izsleegtaa staavoklii
  int pirPin = LOW;                 //uzstaadaam PIR sensora ienaakosho signaalu LOW staavoklii

  sensors.begin();                               // starteejam temperatuuras sensoru biblioteeku
  sensors.setResolution(Apkures_katla_temp, 8); // uzstādam attēlojamās temperatūras precizitāti līdz 0.5 grādi
  sensors.setResolution(Boilera_temp, 8);      // uzstādam attēlojamās temperatūras precizitāti līdz 0.5 grādi
  sensors.setResolution(Pagalma_temp, 8);     // uzstādam attēlojamās temperatūras precizitāti līdz 0.5 grādi
  Serial.begin(9600);
}

void printTemperature(DeviceAddress deviceAddress)   //ieguustam temperatuuras veertiibas no sensoriem un paarbaudam vai nav kljuudu nolasot datus
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.println("Error");
  }
  else  {
    Serial.println(tempC);
  }
}

void loop() {

  sensors.requestTemperatures(); // nolasaam temperatuuru no sensoriem
  delay(1000);

  float tempApkures_katla_temp = sensors.getTempC(Apkures_katla_temp); // ieguustam temperatuuras veertiibas celsija graados
  float tempBoilera_temp = sensors.getTempC(Boilera_temp);            // ieguustam temperatuuras veertiibas celsija graados

  Serial.print("Apkures-Katls-temperatura:");      // Drukājam uz serialo portu Apkures katla temperatūru
  Serial.print(tempApkures_katla_temp);

  Serial.print(" C Boilera-temperatura:");       // Drukajam uz serialo portu ū«dens boilera temperatūru
  Serial.print(tempBoilera_temp);              
  Serial.println("C:");

  Serial.print("Pagalma-temperatura:");      // Drukājam uz seriālo portu pagalma temperatūru
  printTemperature(Pagalma_temp);
  Serial.print("C1");
  
  val_ldr = analogRead(ldr);             // nolasaam analogo ieeju fotorezistoram PINaa A0
  val_ldr1 = analogRead(ldr1);          // nolasaam analogo ieeju fotorezistoram PINaa A1
  val = digitalRead(apkure_in);        // nolasaam telpas temperatuuras deveeja,vai OPENHAB vadiitaa, vai manuaalaa sleedzja staavokli staavokli kas ieleeedz vai izsleedz granulu degli
  
  tmElements_t tm;
  RTC.read(tm);                      // Nolasām no Real Time modulja pulksteni
  
  if ( val == LOW  && tempApkures_katla_temp > 45) { // ja ir signaals no telpas temperaturas deveeja un temperatuura katlaa ir lielaaka par 45 graadiem - iesleedzam granulu degli un iesleedzam apkures suuknjus
    digitalWrite(relejs_deglis, LOW);
    digitalWrite(relejs_sukni, LOW);
    Serial.print(" Deglis-ir:");
    Serial.print("IESLEGTS:1");
    Serial.print(" Apkures-sukni-ir:");
    Serial.print("IESLEGTI:2");
  }
  else if ( val == LOW && tempApkures_katla_temp <= 45 ) { //ja ir signaals no telpas temperaturas deveeja un temperatuura katlaa ir mazaaka vai vienaada ar 45 graadiem - iesleedzam granulu degli un izsleedzam apkures suuknjus
    digitalWrite(relejs_deglis, LOW);
    digitalWrite(relejs_sukni, HIGH);
    Serial.print(" Deglis-ir:");
    Serial.print("IESLEGTS:1");
    Serial.print(" Apkures-sukni-ir:");
    Serial.print("IZSLEGTI:2");
  }

//else if (val == HIGH){
//digitalWrite(relejs_deglis, HIGH);
 //   digitalWrite(relejs_sukni, HIGH);
//}

  else if (val == HIGH && tempBoilera_temp < 39 ) {         // ja nav signaala no telpas temperaturas deveeja bet uudens temperatuura boilerii ir mazaaka par 40 graadi izsleedzam granulu degli un atsleedzam apkures suuknjus laikā no plkst 6 līdz 23
    digitalWrite(relejs_deglis, LOW);
    digitalWrite(relejs_sukni, HIGH);
    Serial.print(" Deglis-ir:");
    Serial.print("IESLEGTS_Boilerim:1");
    Serial.print(" Apkures-sukni-ir:");
    Serial.print("IZSLEGTI:2");
  }

 else if (val == HIGH && tempBoilera_temp > 44 ) {       // ja nav signaala no telpas temperaturas deveeja un uudens temperatuura boilerii ir lielaaka par 50 graadi - izsleedzam granulu degli
    digitalWrite(relejs_deglis, HIGH);
    digitalWrite(relejs_sukni, HIGH);
   Serial.print(" Deglis-ir:");
   Serial.print("IZSLEGTS:1");
   Serial.print(" Apkures-sukni-ir:");
    Serial.print("IZSLEGTI:2");
}
//else if ( tempBoilera_temp < 45){
  //Serial.print(" Deglis-ir:");
    //Serial.print("IESLEGTS_Boilerim:1");
    //Serial.print(" Apkures-sukni-ir:");
    //Serial.print("IZSLEGTI:2");
  //}


  if ( val_ldr > 250  && tm.Hour >= 16 && tm.Hour <= 23)  { //ja fotorezistora rādītājs ir lielāks par 250 (ir tumsš) un ir plkst no 16 - 23 iesleedzam LEd lentu apgaismojumu
  digitalWrite(relejs_Led_lentas, LOW);
  }
  else if (val_ldr < 250)  { //ja fotorezistora raadiitaajs ir mazaaks  par 250 (ir gaisš)izsleezdam apgaismojumu
  digitalWrite(relejs_Led_lentas, HIGH);
    digitalWrite(relejs_prozektori, HIGH);
  }
  else if (val_ldr > 250 && pirPin == HIGH ) {
  digitalWrite(relejs_Led_lentas, LOW);
    digitalWrite(relejs_prozektori, LOW);
  }
 
  if (val_ldr1 < 200) {
  digitalWrite(in_apg, HIGH);
  }
  else if (val_ldr1 > 200)
  digitalWrite(in_apg, LOW);

  DistanceCm = sonar.ping_cm();  // Nolasaam distanci liidz vaartiem
  //Serial.print("Attaalums");
  //Serial.print(DistanceCm);

  if (DistanceCm < 30 ) {        // Ja distance ir mazaaka par 30cm, tad vaarti ir aizverti
    Serial.print("Vartu statuss:");
      Serial.print("CLOSE:0");
    }

    else if (DistanceCm > 30 ) {  // Ja distance irlielaaka par 30cm, tad vaarti ir atveerti
    Serial.print("Vartu statuss:");
      Serial.print("OPEN:0");
    }

  // else if (DistanceCm > 30  && (   ( tm.Hour >= 22 && tm.Hour <= 24) || (tm.Hour >= 0 && tm.Hour <= 7))      ) { //jāpiestrādā pagaidām tikai tests
  //  Serial.print("Vartu statuss:");
  //  Serial.print("Atveerti_naktii:0");
  // }
}

void print2digits(int number) {      // šis nepieciešams, lai salīdzinatu pulksteni
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}











