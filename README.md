# Arduino chicken door

Minimalist automated chicken door with Arduino nano

- 220V AC power supply
- Opening and closing time are defined in a monthly calendar recorded in the RTC module memory

---

## Required Components

- Arduino nano
- power supply: HLK-10M05
- RTC module: DS3231
- motor driver: TB6612FNG
- motor: 1:90 TT DC Gear Motor

---

## pin mapping

- Motor connected to TB6612FNG AO1 and AO2

### Arduino

- A2: TB6612FNG GND
- A4: DS3231 SDA
- A5: DS3231 SCL
- D2: DS3231 SQW
- D3: TB6612FNG AIN2
- D4: TB6612FNG AIN1
- D5: TB6612FNG STBY

### HLK-10M05

- +vo: DS3231 VCC, TB6612FNG VM, Arduino 5V
- -vo: DS3231 GND, Arduino GND, 10 ohm resistor to TB6612FNG GND

---

## Setting up the RTC module

First, you'll have to set up the monthly calendar alarm accordingly to your geographic location by editing the InitRTC.ino file then loading it into the arduino.

### Time setting

if you need to adjust the clock, uncomment the second line and set the correct date and time.

```
  // set time (format : year, month, day of month, h, m, s)
  //rtc.adjust(DateTime(2021, 11, 26, 17, 33, 0));
```

### Alarm clock setting

configure the desired opening and closing time for each month of the year.

```
  //alarm calendar settings (am and pm for each month)
  struct calendarAlarmStruc monthlyCalendarAlarm[monthCount] = {
    {DateTime(0, 0, 0, 8, 45, 0), DateTime(0, 0, 0, 18, 05, 0)},
    {DateTime(0, 0, 0, 8, 05, 0), DateTime(0, 0, 0, 18, 50, 0)},
    {DateTime(0, 0, 0, 7, 10, 0), DateTime(0, 0, 0, 19, 35, 0)},
    {DateTime(0, 0, 0, 6, 10, 0), DateTime(0, 0, 0, 20, 20, 0)},
    {DateTime(0, 0, 0, 5, 50, 0), DateTime(0, 0, 0, 20, 00, 0)},
    {DateTime(0, 0, 0, 5, 00, 0), DateTime(0, 0, 0, 21, 30, 0)},
    {DateTime(0, 0, 0, 5, 15, 0), DateTime(0, 0, 0, 21, 25, 0)},
    {DateTime(0, 0, 0, 5, 55, 0), DateTime(0, 0, 0, 20, 40, 0)},
    {DateTime(0, 0, 0, 6, 35, 0), DateTime(0, 0, 0, 19, 40, 0)},
    {DateTime(0, 0, 0, 7, 20, 0), DateTime(0, 0, 0, 18, 40, 0)},
    {DateTime(0, 0, 0, 8, 05, 0), DateTime(0, 0, 0, 17, 50, 0)},
    {DateTime(0, 0, 0, 8, 40, 0), DateTime(0, 0, 0, 17, 40, 0)},
  };
```

Once the file is run by the arduino, the alarm clock settings will be recorded in the AT24C32 memory of the DS3231 RTC clock module.

Then you can load the chickenDoor.ino file in the arduino.

---

## inspirations

- [http://seb.lobo.pagesperso-orange.fr/Porte-poulailler/motorisation-porte-eco.html](http://seb.lobo.pagesperso-orange.fr/Porte-poulailler/motorisation-porte-eco.html)
- [http://moncastel.free.fr/forum/trappePoule/](http://moncastel.free.fr/forum/trappePoule/)
