/*

Author: Nikola Vidović

Project: Smart House

Key components: DHT11, 16x2 LCD Display,
potentiometers, reed switch, switches,
RGB diode, LED, photoresistor

Short description: 

  - The task of this project is control of all Smart House systems 
    and display information about current state of systems
  - DHT11 is used to measure temperature and humidity, the values will be printed on LCD display (T & H)
  - Using potentiometers, user will be able to set temperature and humidity, which will also be printed on LCD display (T0 & H0)
  - RGB diode will represent an air conditioner, and will light red as a sign of air conditioner heating up the house,
    and blue as a sign of cooling the house
  - LED diode will represent humidifier (ON and OFF accordingly)
  - Air conditioner and humidifier can only be ON if the window is closed (check using reed switch)
  - Manual air conditioner and humidifier switch
  - Photoresistor will be used as a sensor to turn on the lighting(another LED) at night,
    during the day the lighting will be switched off.
  - All information will also be printed in a specific format via the Serial monitor.

  NOTICE: 
  
  Tinkercad doesn't own DHT11 sensor,
  so I'll improvise using two temperature sensors.
  One of them will act like a humidity sensor. 
  In reality I'll use DHT11 and because of that,
  the code will be altered to suit the needs of Tinkercad.
  Also there is no reed switch, so I'll replace it
  with a basic switch.

  If you want to use DHT11 in your project,
  you can find valuable information here https://projecthub.arduino.cc/arcaegecengiz/using-dht11-12f621.

  You may need to alter calibration data based on what sensor are you using or type of enviroment you are testing the project.
*/

// --Libraries--

#include <LiquidCrystal.h>

// --LCD--

#define RS 2
#define E 3
#define DB4 4
#define DB5 5
#define DB6 6
#define DB7 7

LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7); 

// --Light--

#define illumination_calibration 950

#define light A1
#define photoresistor A0

int illumination;

// --TEMPERATURE--

#define temperature_min 10
#define temperature_max 30

#define temperature_sensor A5
#define temperature_potentiometer A4

#define heating 9
#define cooling 8

int  temperature, set_temperature;

// --HUMIDITY--

#define humidity_min 0
#define humidity_max 100

#define humidity_sensor A2
#define humidity_potentiometer A3

#define humidifier 10

int humidity, set_humidity;

// --WINDOW--

#define reed 11

bool closed;

// --SWITCH--

#define _switch 12
bool turn_on;

void setup()
{
  // initialization of serial monitor 
  
  Serial.begin(9600);
  
  // initialization of LCD
  
  lcd.begin(16, 2);
  
  lcd.setCursor(0, 0);
  lcd.print("T:");
  
  lcd.setCursor(8, 0);
  lcd.print("T0:");
  
  lcd.setCursor(0, 1);
  lcd.print("H:");
  
  lcd.setCursor(8, 1);
  lcd.print("H0:");
  
  // --LIGHT--
  pinMode(light, OUTPUT); 
  
  // --AC--
  pinMode(heating, OUTPUT); 
  pinMode(cooling, OUTPUT); 
  
  // --HUMIDIFIER--
  pinMode(humidifier, OUTPUT);
  
  // --SWITCHES / WINDOW--
  pinMode(reed, INPUT_PULLUP);
  pinMode(_switch, INPUT_PULLUP);
}

void loop()
{
  //all functions have to be executed in real-time
  
  // --LIGHT--
  
  illumination = analogRead(photoresistor);
  
  digitalWrite(light, illumination < illumination_calibration ? HIGH : LOW);
  
  // --WINDOW--
  
  closed = digitalRead(reed) == LOW ? true : false;
  
  // --SWITCH--
  
  turn_on = digitalRead(_switch) == LOW ? true : false;
  
  // --TEMPERATURE--
  
  temperature = map(analogRead(temperature_sensor), 20, 358, 0, 40);
  set_temperature = map(analogRead(temperature_potentiometer), 0, 1023, temperature_min, temperature_max);
  
  if(temperature < set_temperature && closed && turn_on){
  	digitalWrite(heating, HIGH);
    digitalWrite(cooling, LOW);
  }
  else if(temperature > set_temperature && closed && turn_on){
  	digitalWrite(heating, LOW);
    digitalWrite(cooling, HIGH);
  }
  else{
  	digitalWrite(heating, LOW);
    digitalWrite(cooling, LOW);
  }
    
  
  // --HUMIDITY--
  
  humidity = map(analogRead(humidity_sensor), 20, 358, 0, 100);
  set_humidity = map(analogRead(humidity_potentiometer), 0, 1023, humidity_min, humidity_max);
  
  digitalWrite(humidifier, humidity < set_humidity && closed && turn_on ? HIGH : LOW);
  
  // --LCD--
  
  lcd.setCursor(2, 0);
  lcd.print(temperature);
  lcd.print("C ");
  
  lcd.setCursor(11, 0);
  lcd.print(set_temperature);
  lcd.print("C ");
  
  lcd.setCursor(2, 1);
  lcd.print(humidity);
  lcd.print("% ");
  
  lcd.setCursor(11, 1);
  lcd.print(set_humidity);
  lcd.print("% ");
  
  // --SERIAL MONITOR--
  
  Serial.print("Light: ");
  Serial.print(digitalRead(light) == HIGH ? "ON" : "OFF");
  
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  
  Serial.print("C | Temp0: ");
  Serial.print(set_temperature);
  
  Serial.print("C | AC: ");
  Serial.print(digitalRead(cooling) == HIGH ? "COOL" : digitalRead(heating) == HIGH ? "HEAT" : "OFF");
  
  Serial.print(" | Hum: ");
  Serial.print(humidity);
  
  Serial.print("% | Hum0: ");
  Serial.print(set_humidity);
  
  Serial.print("% | Humidifier: ");
  Serial.print(digitalRead(humidifier) == HIGH ? "ON" : "OFF");
  
  Serial.print(" | Window: ");
  Serial.println(closed == true ? "CLOSED" : "OPEN");
  
}