#include <RTClib.h>
#include <AT24Cxx.h>
#include <avr/sleep.h>
// AT24C32
#define at24Address 0x57 // nvram address
#define at24Size 32      // nvram size in KB
// DS3231
#define SQW 2
// TB6612FNG
#define STBY 3
#define AIN1 4
#define AIN2 5
// Motor current
#define MOT A2

RTC_DS3231 rtc;
AT24Cxx nvram(at24Address, at24Size);

int motor_current;
unsigned long drive_start;
unsigned long blocking_start;
int blocking_delai = 200;
int blocking_current = 450;
int max_drive_delai = 15000;

void setup()
{
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(MOT, INPUT);
  pinMode(SQW, INPUT_PULLUP);
  interrupts();

  rtc.begin();
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.disableAlarm(2);
  rtc.disable32K();
  rtc.writeSqwPinMode(DS3231_OFF); // Place SQW pin into alarm interrupt mode
}

void loop()
{
  DateTime nextAlarm = get_next_alarm_from_nvram();
  set_alarm(nextAlarm);

  motorDrive(!nextAlarm.isPM(), max_drive_delai);

  sleepPwrDown();
}

// get monthly alarm record from AT24C32
DateTime get_next_alarm_from_nvram(void)
{
  DateTime now = rtc.now();

  int address = (now.month() - 1) * 4;
  int offset = now.isPM() * 2;
  int hour = nvram.read(address + offset);
  int minute = nvram.read(address + offset + 1);

  TimeSpan ts1 = DateTime(now.year(), now.month(), now.day(), hour, minute, 0) - now;

  if (ts1.totalseconds() < 0)
  {
    offset = !now.isPM() * 2;
    hour = nvram.read(address + offset);
    minute = nvram.read(address + offset + 1);
  }
  DateTime nextAlarm = DateTime(0, 0, 0, hour, minute, 0);

  return nextAlarm;
}

// set alarm1 from DS3231
void set_alarm(DateTime alarmTime)
{
  rtc.clearAlarm(1);
  rtc.setAlarm1(alarmTime, DS3231_A1_Hour);
  attachInterrupt(digitalPinToInterrupt(SQW), onAlarm, LOW);
}

// put arduino to sleep
void sleepPwrDown()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
}

// DS3231 alarm interruption
void onAlarm()
{
  detachInterrupt(digitalPinToInterrupt(SQW));
}

// motor A control from TB6612FNG
void motorDrive(boolean direction, int duration)
{
  digitalWrite(STBY, HIGH);
  switch (direction)
  {
  case 0: // up
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    break;
  case 1: // down
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    break;
  }
  drive_start = millis();

  // anti blocking protection
  do
  {
    motor_current = analogRead(MOT);

    if (motor_current < blocking_current)
    {
      blocking_start = millis();
    }
  } while ((millis() - drive_start) < duration && ((millis() - blocking_start) < blocking_delai));
  motorStop();
}

void motorStop()
{
  digitalWrite(STBY, LOW);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
}
