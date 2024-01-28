/*

Ime i prezime: Nikola Vidović

Naziv projekta: Pametna kuća

Ključne komponente: DHT11, 16x2 LED Display,
potenciometri, reed sklopka, obične sklopke,
RGB dioda, diode, fotootpornik


Kratki opis zadatka: Zadatak je jednostavno upravljanje
pametnom kućom te prikaz informacija o trenutnom stanju korisniku.
Senzor DHT11 će mjeriti temperaturu i vlagu čije će se vrijednosti
ispisivati na LED Displayu.
Korištenjem potenciometara korisnik će biti u mogućnosti
postaviti željenu temperaturu i vlagu koje će se isto ispisati na displayu.
RGB dioda će svijetliti crveno ako klima grije,
u suprotnom ako hladi svijetlit će plavo,
obična dioda biti će indikator za ovlaživač zraka.
Klima i ovlaživač biti će uključeni samo ako prozor nije otvoren
što će se provjeravati pomoću reed sklopke,
a korisnik će moći isključiti i klimu i ovlaživač manualno
preko obične sklopke.
Fotootpornik će se koristiti kao senzor za paljenje rasvjete
(još jedna obična dioda) kuće po noći,
dok će po danu rasvjeta biti isključena.
Sve informacije će također biti ispisane u određenom formatu preko Serial monitora.


NAPOMENA: Tinkercad ne posjeduje DHT11 senzor, stoga ću
improvizirati sa dva senzora temperature od kojih će jedan
"glumiti" senzor za vlagu, a pri spajanju na fizičkoj pločici
ću koristiti DHT11, pa će iz toga razloga kod biti drugačiji 
kako bi zadovoljio shemu sa Tinkercada.

Također ne posjeduje ni reed sklopku,
pa ću nju zamijeniti običnom sklopkom.

Ako želite koristiti DHT11 u svome projektu, potrebne informacije možete pronaći ovdje https://projecthub.arduino.cc/arcaegecengiz/using-dht11-12f621.

Možda će biti potrebno kalibrirati osjetljivost senzora ovisno o vrsti senzora i uvjetima u kojima se nalaze kako bi funkcionirali na pravi način.

*/

// --Biblioteke--

#include <LiquidCrystal.h>

// --LCD EKRAN--

#define RS 2
#define E 3
#define DB4 4
#define DB5 5
#define DB6 6
#define DB7 7

LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7); 

// --RASVJETA--

#define kalibracija_osvijetljenosti 950

#define rasvjeta A1
#define fotootpornik A0

int osvijetljenost;

// --TEMPERATURA--

#define temp_min 10
#define temp_max 30

#define temperatura_senzor A5
#define temperatura_pot A4

#define grijanje 9
#define hladenje 8

int  temperatura, zeljena_temp;

// --VLAGA--

#define vlaga_min 0
#define vlaga_max 100

#define vlaga_senzor A2
#define vlaga_pot A3

#define ovlazivac 10

int vlaga, zeljena_vlaga;

// --PROZOR--

#define reed 11

bool zatvoren;

// --SKLOPKA--

#define sklopka 12
bool ukljuci;

void setup()
{
  // inicijalizacija serial monitora 
  
  Serial.begin(9600);
  
  // inicijalizacija LCD ekrana
  
  lcd.begin(16, 2);
  
  lcd.setCursor(0, 0);
  lcd.print("T:");
  
  lcd.setCursor(8, 0);
  lcd.print("T0:");
  
  lcd.setCursor(0, 1);
  lcd.print("H:");
  
  lcd.setCursor(8, 1);
  lcd.print("H0:");
  
  // --RASVJETA--
  pinMode(rasvjeta, OUTPUT); 
  
  // --KLIMA--
  pinMode(grijanje, OUTPUT); 
  pinMode(hladenje, OUTPUT); 
  
  // --OVLAZIVAC--
  pinMode(ovlazivac, OUTPUT);
  
  // --SKLOPKE / PROZOR--
  pinMode(reed, INPUT_PULLUP);
  pinMode(sklopka, INPUT_PULLUP);
}

void loop()
{
  //sve funkcije pametne kuće izvršavaju se u real time-u
  
  // --RASVJETA--
  
  osvijetljenost = analogRead(fotootpornik);
  
  digitalWrite(rasvjeta, osvijetljenost < kalibracija_osvijetljenosti ? HIGH : LOW);
  
  // --PROZOR--
  
  zatvoren = digitalRead(reed) == LOW ? true : false;
  
  // --SKLOPKA--
  
  ukljuci = digitalRead(sklopka) == LOW ? true : false;
  
  // --TEMPERATURA--
  
  temperatura = map(analogRead(temperatura_senzor), 20, 358, 0, 40);
  zeljena_temp = map(analogRead(temperatura_pot), 0, 1023, temp_min, temp_max);
  
  if(temperatura < zeljena_temp && zatvoren && ukljuci){
  	digitalWrite(grijanje, HIGH);
    digitalWrite(hladenje, LOW);
  }
  else if(temperatura > zeljena_temp && zatvoren && ukljuci){
  	digitalWrite(grijanje, LOW);
    digitalWrite(hladenje, HIGH);
  }
  else{
  	digitalWrite(grijanje, LOW);
    digitalWrite(hladenje, LOW);
  }
    
  
  // --VLAGA--
  
  vlaga = map(analogRead(vlaga_senzor), 20, 358, 0, 100);
  zeljena_vlaga = map(analogRead(vlaga_pot), 0, 1023, vlaga_min, vlaga_max);
  
  digitalWrite(ovlazivac, vlaga < zeljena_vlaga && zatvoren && ukljuci ? HIGH : LOW);
  
  // --LCD--
  
  lcd.setCursor(2, 0);
  lcd.print(temperatura);
  lcd.print("C ");
  
  lcd.setCursor(11, 0);
  lcd.print(zeljena_temp);
  lcd.print("C ");
  
  lcd.setCursor(2, 1);
  lcd.print(vlaga);
  lcd.print("% ");
  
  lcd.setCursor(11, 1);
  lcd.print(zeljena_vlaga);
  lcd.print("% ");
  
  // --SERIAL MONITOR--
  
  Serial.print("Rasvjeta: ");
  Serial.print(digitalRead(rasvjeta) == HIGH ? "ON" : "OFF");
  
  Serial.print(" | Temp: ");
  Serial.print(temperatura);
  
  Serial.print("C | Temp0: ");
  Serial.print(zeljena_temp);
  
  Serial.print("C | Klima: ");
  Serial.print(digitalRead(hladenje) == HIGH ? "HLADI" : digitalRead(grijanje) == HIGH ? "GRIJE" : "OFF");
  
  Serial.print(" | Hum: ");
  Serial.print(vlaga);
  
  Serial.print("% | Hum0: ");
  Serial.print(zeljena_vlaga);
  
  Serial.print("% | Ovlazivac: ");
  Serial.print(digitalRead(ovlazivac) == HIGH ? "ON" : "OFF");
  
  
  Serial.print(" | Prozor: ");
  Serial.println(zatvoren == true ? "ZATVOREN" : "OTVOREN");
  
}