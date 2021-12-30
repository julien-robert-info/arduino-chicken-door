#include <RTClib.h>
#include <AT24Cxx.h>
#define at24Address 0x57 // nvram address
#define at24Size 32      // nvram size in KB
#define monthCount 12

AT24Cxx nvram(at24Address, at24Size);
RTC_DS3231 rtc;
int address;

struct calendarAlarmStruc {
  DateTime am;
  DateTime pm;
};

void setup() {
  Serial.begin(9600);
  rtc.begin();

  // set time (format : year, month, day of month, h, m, s)
  //rtc.adjust(DateTime(2021, 11, 26, 17, 33, 0));

  //alarm calendar settings (am and pm for each month)
  struct calendarAlarmStruc monthlyCalendarAlarm[monthCount] = {
    {DateTime(0, 0, 0, 8, 15, 0), DateTime(0, 0, 0, 18, 35, 0)},
    {DateTime(0, 0, 0, 7, 35, 0), DateTime(0, 0, 0, 19, 20, 0)},
    {DateTime(0, 0, 0, 6, 40, 0), DateTime(0, 0, 0, 20, 05, 0)},
    {DateTime(0, 0, 0, 5, 40, 0), DateTime(0, 0, 0, 20, 50, 0)},
    {DateTime(0, 0, 0, 5, 20, 0), DateTime(0, 0, 0, 20, 30, 0)},
    {DateTime(0, 0, 0, 4, 30, 0), DateTime(0, 0, 0, 22, 00, 0)},
    {DateTime(0, 0, 0, 4, 45, 0), DateTime(0, 0, 0, 21, 55, 0)},
    {DateTime(0, 0, 0, 5, 25, 0), DateTime(0, 0, 0, 21, 10, 0)},
    {DateTime(0, 0, 0, 6, 05, 0), DateTime(0, 0, 0, 20, 10, 0)},
    {DateTime(0, 0, 0, 6, 50, 0), DateTime(0, 0, 0, 19, 10, 0)},
    {DateTime(0, 0, 0, 7, 35, 0), DateTime(0, 0, 0, 18, 20, 0)},
    {DateTime(0, 0, 0, 8, 10, 0), DateTime(0, 0, 0, 18, 10, 0)},
  };

  //alarm calendar writting into nvram
  for(int i = 0; i < monthCount; i++){
    address = i * 4;

    nvram.write(address, monthlyCalendarAlarm[i].am.hour());
    nvram.write(address + 1, monthlyCalendarAlarm[i].am.minute());
    nvram.write(address + 2, monthlyCalendarAlarm[i].pm.hour());
    nvram.write(address + 3, monthlyCalendarAlarm[i].pm.minute());
  }
}

void loop() {
  DateTime now = rtc.now();  // Get current time
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.println("\nMonthly alarm calendar : ");
  for(int i = 0; i < monthCount; i++){
    address = i * 4;

    Serial.print("Month : ");
    Serial.print(i + 1);
    Serial.print(" am : ");
    Serial.print(nvram.read(address));
    Serial.print(":");
    Serial.print(nvram.read(address + 1));
    Serial.print(", pm : ");
    Serial.print(nvram.read(address + 2));
    Serial.print(":");
    Serial.println(nvram.read(address + 3));
  }

  delay(5000);
}
