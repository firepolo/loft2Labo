/*
   TimeAlarmExample.pde

   This example calls alarm functions at 8:30 am and at 5:45 pm (17:45)
   and simulates turning lights on at night and off in the morning
   A weekly timer is set for Saturdays at 8:30:30

   A timer is called every 15 seconds
   Another timer is called once only after 10 seconds

   At startup the time is set to Jan 1 2011  8:29 am
   add eeprom every
*/

// Questions?  Ask them here:
// http://forum.arduino.cc/index.php?topic=66054.0

#include <TimeLib.h>
#include <TimeAlarms.h>
#include "DHT.h"
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
AlarmId id;

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor

  dht.begin();

  pinMode(23, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(37, OUTPUT);

  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(53, OUTPUT);

  digitalWrite(23, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(29, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(33, HIGH);
  digitalWrite(35, HIGH);
  digitalWrite(37, HIGH);

  digitalWrite(39, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(43, HIGH);
  digitalWrite(45, HIGH);
  digitalWrite(47, HIGH);
  digitalWrite(49, HIGH);
  digitalWrite(51, HIGH);
  digitalWrite(53, HIGH);

  tmElements_t tm;

if (RTC.read(tm)) {
  setTime(tm.Hour, tm.Minute, tm.Second, tm.Day, tm.Month, tmYearToCalendar(tm.Year)); // set time to Saturday 8:29:00am Jan 1 2011
}

  // create the alarms, to trigger at specific times
  Alarm.alarmRepeat(9, 00, 00, MorningAlarm); // 9:00am every day
  Alarm.alarmRepeat(15, 00, 10, GrowAlarm); // 15:00pm every day
  Alarm.alarmRepeat(18, 00, 20, FlowAlarmStart); // 5:45pm every day
  Alarm.alarmRepeat(6, 00, 30, FlowAlarmStop); // 5:45pm every day
  Alarm.alarmRepeat(12, 00, 40, AlarmVentilat10); // 5:45pm every day
  Alarm.alarmRepeat(12, 00, 40, AlarmVentilat1030); // 5:45pm every day
  Alarm.alarmRepeat(dowSaturday, 8, 30, 30, WeeklyAlarm); // 8:30:30 every Saturday

  // create timers, to trigger relative to when they're created
  Alarm.timerRepeat(15, Repeats);           // timer for every 15 seconds
  id = Alarm.timerRepeat(2, Repeats2);      // timer for every 2 seconds
  Alarm.timerOnce(10, OnceOnly);            // called once after 10 seconds
}

void loop() {
  digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display
}

// functions to be called when an alarm triggers:
void MorningAlarm() {
  Serial.println("Alarm: - turn lights off");
  digitalWrite(23, HIGH); //relais 1
  digitalWrite(25, HIGH);//relais 2
  digitalWrite(27, HIGH);//relais 3
  digitalWrite(29, HIGH);//relais 4
}
void AlarmVentilat10() {
  Serial.println("Alarm: - turn fan on");
  digitalWrite(23, HIGH); //relais 1
  digitalWrite(25, HIGH);//relais 2
  digitalWrite(27, HIGH);//relais 3
  digitalWrite(29, HIGH);//relais 4
  digitalWrite(53, LOW); //relais 16
}
void AlarmVentilat1030() {
  Serial.println("Alarm: - turn fan on");
  digitalWrite(23, HIGH); //relais 1
  digitalWrite(25, HIGH);//relais 2
  digitalWrite(27, HIGH);//relais 3
  digitalWrite(29, HIGH);//relais 4
  digitalWrite(53, HIGH); //relais 16
}

void GrowAlarm() {
  Serial.println("Alarm: - turn lights on");
  digitalWrite(23, LOW); // relais 2 allumé
  digitalWrite(25, LOW); //relais 1 allumé
  digitalWrite(27, LOW); // relais 3 allumé
  digitalWrite(29, HIGH); // relais 4 éteint
}
void FlowAlarmStart() {
  Serial.println("Alarm: - turn lights on");
  digitalWrite(23, LOW); // relais 1 allumé
  digitalWrite(25, LOW); // relais 2 allumé
  digitalWrite(27, LOW); // relais 3 allumé
  digitalWrite(29, LOW); // relais 4 allumé
}
void FlowAlarmStop() {
  Serial.println("Alarm: - turn lights off");
  digitalWrite(25, LOW); //relais 1 allumé
  digitalWrite(23, LOW); // relais 2 allumé
  digitalWrite(27, LOW); // relais 3allumé
  digitalWrite(29, HIGH); // relais4 éteint
}

void WeeklyAlarm() {
  Serial.println("Alarm: - its Monday Morning");
}

void ExplicitAlarm() {
  Serial.println("Alarm: - this triggers only at the given date and time");
}

void Repeats() {
  Serial.println("15 second timer read sensors");
  getdht();
}

void Repeats2() {
  Serial.println("2 second timer check rtc");
  rtc();
  //
}

void OnceOnly() {
  Serial.println("This timer only triggers once, stop the 2 second timer");
  // use Alarm.free() to disable a timer and recycle its memory.
  Alarm.free(id);
  // optional, but safest to "forget" the ID after memory recycled
  id = dtINVALID_ALARM_ID;
  // you can also use Alarm.disable() to turn the timer off, but keep
  // it in memory, to turn back on later with Alarm.enable().
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}

void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void getdht() {
  // Wait a few seconds between measurements.
  //delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}
void rtc() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
