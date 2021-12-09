#include <RTClib.h>
#include <AT24Cxx.h>
#include <avr/sleep.h>
//AT24C32
#define at24Address 0x57 // nvram address
#define at24Size 32      // nvram size in KB
//DS3231
#define SQW 2
//TB6612FNG
#define STBY 3
#define AIN1 4
#define AIN2 5

RTC_DS3231 rtc;
AT24Cxx nvram(at24Address, at24Size);

void setup() {
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(SQW, INPUT_PULLUP);
  interrupts();

  rtc.begin();
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.disableAlarm(2);
  rtc.disable32K();
  rtc.writeSqwPinMode(DS3231_OFF); // Place SQW pin into alarm interrupt mode
}

void loop() {
  DateTime now = rtc.now();
  
  DateTime nextAlarm = get_alarm_from_nvram(now.month(), !now.isPM());
  set_alarm(nextAlarm);

  //move door
  motorDrive(now.isPM(), 2500);
  
  sleepPwrDown();
}

// set alarm1 from DS3231
void set_alarm(DateTime alarmTime)
{
  rtc.clearAlarm(1);
  rtc.setAlarm1(alarmTime, DS3231_A1_Hour);
  attachInterrupt(digitalPinToInterrupt(SQW), onAlarm, LOW);
}

// get monthly alarm record from AT24C32
DateTime get_alarm_from_nvram(int month, int isPm){
  int address = month * 4;
  int offset = isPm * 2;
  int hour = nvram.read(address + offset);
  int minute = nvram.read(address + offset + 1);

  return DateTime(0, 0, 0, hour, minute, 0);
}

// put arduino to sleep
void sleepPwrDown() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

// DS3231 alarm interruption
void onAlarm() {
  detachInterrupt(digitalPinToInterrupt(SQW));
}

// motor A control from TB6612FNG
void motorDrive(boolean direction, int duration){
  digitalWrite(STBY, HIGH);
  switch (direction) {
    case 0 : //up
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      break;
    case 1 : //down
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      break;
  }
  delay(duration);
  digitalWrite(STBY, LOW);
}
