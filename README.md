# SmartHouse - Arduino project

This repository contains materials used for SmartHouse Arduino university project. 
There are two .ino files, both doing the same job.
The difference is that one is written in english and the other one in croatian.

Whole project was made in Tinkercad and then in real world.

## Project description

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

## Used electronic components
  
  - Arduino UNO
  - DHT11
  - 16x2 LCD
  - potentiometers
  - reed switch
  - switches
  - RGB diode
  - LED
  - photoresistor

## NOTICE

  Tinkercad doesn't own DHT11 sensor, so I'll improvise using two temperature sensors. One of them will act like a humidity sensor. 
  In reality I'll use DHT11. The code will be altered to suit the needs of Tinkercad.
  Also there is no reed switch, so I'll replace it with a basic switch.

  If you want to use DHT11 in your project,
  you can find valuable information here https://projecthub.arduino.cc/arcaegecengiz/using-dht11-12f621.

  You may need to alter calibration data based on what sensor are you using or type of enviroment you are testing the project.

  
