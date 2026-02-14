// #include <Arduino.h>
// #include <U8g2lib.h>
// #include "Seeed_SHT35.h"
// #include <WiFi.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
// #include <time.h>
// #include "PCF85063TP.h"
// #include "secrets.h"

// // U8G2 display initialization
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);
// // SHT35 sensor initialization
// SHT35 sensor(SCL);
// // RTC initialization
// PCF85063TP RTclock;

// // // Timezone and DST configuration (default: Mountain Time with DST)
// // int tzOffsetHours = -7;   // Mountain Time
// // bool dstActive = true;     // Daylight Saving Time

// // NTP client
// WiFiUDP udp;
// NTPClient timeClient(udp, "pool.ntp.org", 0, 3600000); // offset 0, update every hour

// // Store last known time and date
// int lastDay = 0, lastMonth = 0, lastYear = 0;
// int lastHour = 0, lastMinute = 0, lastSecond = 0;

// // Function to update last known time from RTC
// void updateTimeFromRTC() {
//   RTclock.getTime();
//   lastHour = RTclock.hour;
//   lastMinute = RTclock.minute;
//   lastSecond = RTclock.second;
//   lastDay = RTclock.dayOfMonth;
//   lastMonth = RTclock.month;
//   lastYear = RTclock.year + 2000;
// }

// // Function to sync RTC with NTP
// void syncRTCwithNTP() {
//   struct tm timeinfo;
//   if(getLocalTime(&timeinfo)) {
//     RTclock.stopClock();
//     RTclock.fillByYMD(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
//     RTclock.fillByHMS(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
//     RTclock.fillDayOfWeek(timeinfo.tm_wday);
//     RTclock.setTime();
//     RTclock.startClock();
//     updateTimeFromRTC();
//   }
// }

// void setup() {
//   Serial.begin(115200);
//   Wire.begin(4, 5); // Required for XIAO ESP32C3 I2C
//   u8g2.begin();
//   RTclock.begin();

//     if (sensor.init()) {
//     Serial.println("Sensor init failed!!!");
//   } else {
//     Serial.println("Sensor initialized");
//   }

//   WiFi.begin(ssid, password);

//   int attempts = 0;
//   while (WiFi.status() != WL_CONNECTED && attempts < 20) {
//     delay(500);
//     attempts++;
//   }

//   if (WiFi.status() == WL_CONNECTED) {
//     // This string sets the clock to Mountain Time and handles DST automatically
//     configTime("MST7MDT,M3.2.0,M11.1.0", "pool.ntp.org");

//     // Crucial: Wait for NTP to actually grab the time before syncing
//     struct tm timeinfo;
//     if (getLocalTime(&timeinfo, 5000)) {
//       syncRTCwithNTP();
//       Serial.println("RTC Synced with NTP");
//     }
//   }
//   WiFi.disconnect(true);
//   WiFi.mode(WIFI_OFF);
// }
// // void setup() {
// //   Serial.begin(115200);
// //   delay(10);
// //   Wire.begin(4, 5);
// //   u8g2.begin();
// //   RTclock.begin();
// //   Serial.println("Serial started");

// //   if (sensor.init()) {
// //     Serial.println("Sensor init failed!!!");
// //   } else {
// //     Serial.println("Sensor initialized");
// //   }

// //   // Initialize RTC with a default time if needed
// //   RTclock.begin();
// //   RTclock.stopClock();
// //   RTclock.fillByYMD(2025, 1, 1);
// //   RTclock.fillByHMS(12, 0, 0);
// //   RTclock.fillDayOfWeek(TUE);
// //   RTclock.setTime();
// //   RTclock.startClock();
// //   updateTimeFromRTC();

// //   // Connect to Wi-Fi
// //   WiFi.begin(ssid, password);
// //   Serial.println("Connecting to WiFi...");
// //   int wifi_attempts = 0;
// //   while (WiFi.status() != WL_CONNECTED && wifi_attempts < 20) {
// //     delay(500);
// //     Serial.print(".");
// //     wifi_attempts++;
// //   }

// //   if (WiFi.status() == WL_CONNECTED) {
// //     Serial.println("\nConnected to WiFi");
// //     timeClient.begin();
// //     delay(1000);

// //     // Apply timezone offset and DST
// //     long tzOffsetSeconds = tzOffsetHours * 3600;
// //     long dstOffsetSeconds = dstActive ? 3600 : 0;
// //     configTime(tzOffsetSeconds, dstOffsetSeconds, "pool.ntp.org");

// //     // Sync RTC with NTP
// //     syncRTCwithNTP();
// //   } else {
// //     Serial.println("\nWiFi not connected, using RTC time");
// //   }

// //   WiFi.disconnect(true);
// //   WiFi.mode(WIFI_OFF);
// // }

// void loop() {
//   // Update time from RTC
//   updateTimeFromRTC();

//   // Periodically sync RTC with NTP if Wi-Fi is available
//   static unsigned long lastSync = 0;
//   if (millis() - lastSync > 3600000) { // every hour
//     WiFi.begin(ssid, password);
//     if (WiFi.status() == WL_CONNECTED) {
//       timeClient.begin();
//       delay(1000);
//       long tzOffsetSeconds = tzOffsetHours * 3600;
//       long dstOffsetSeconds = dstActive ? 3600 : 0;
//       configTime(tzOffsetSeconds, dstOffsetSeconds, "pool.ntp.org");
//       syncRTCwithNTP();
//     }
//     WiFi.disconnect(true);
//     WiFi.mode(WIFI_OFF);
//     lastSync = millis();
//   }

//   // Clear display
//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB10_tr);

//   // Read temperature and humidity
//   float temp, hum;
//   if (NO_ERROR != sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &temp, &hum)) {
//     u8g2.println("Read temp failed!!");
//   } else {
//     u8g2.setCursor(0, 12);
//     u8g2.print("Temp: ");
//     u8g2.print((temp * 1.8) + 32);
//     u8g2.setFont(u8g2_font_unifont_t_symbols);
//     u8g2.drawGlyph(88, 10, 0x00b0);
//     u8g2.setFont(u8g2_font_ncenB10_tr);
//     u8g2.print(" F");

//     u8g2.setCursor(0, 27);
//     u8g2.print("Hum: ");
//     u8g2.print(hum);
//     u8g2.print(" %");

//     u8g2.setCursor(0, 42);
//     u8g2.print("Date: ");
//     u8g2.print(lastMonth);
//     u8g2.print("/");
//     u8g2.print(lastDay);
//     u8g2.print("/");
//     u8g2.print(lastYear);

//     u8g2.setCursor(0, 57);
//     char formattedTime[12];
//     int displayHour = lastHour % 12;
//     if (displayHour == 0) displayHour = 12;
//     const char* period = (lastHour >= 12) ? "PM" : "AM";
//     sprintf(formattedTime, "%02d:%02d:%02d %s", displayHour, lastMinute, lastSecond, period);
//     u8g2.print("Time: ");
//     u8g2.print(formattedTime);

//     u8g2.sendBuffer();
//   }

//   delay(1000);
// }

//////////////////////////////////////////////////////////////////////////////

// #include <Arduino.h>
// #include <U8g2lib.h>
// #include "Seeed_SHT35.h"
// #include <WiFi.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
// #include <time.h>
// #include "PCF85063TP.h"
// #include "secrets.h"

// // SHT35 sensor initialization
// SHT35 sensor(SCL);

// // U8G2 display initialization
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

// // RTC initialization
// PCF85063TP RTclock;

// // NTP client
// WiFiUDP udp;
// NTPClient timeClient(udp, "pool.ntp.org", 0, 3600000);

// // Store last known time and date
// int lastDay, lastMonth, lastYear;
// int lastHour, lastMinute, lastSecond;

// void updateTimeFromRTC()
// {
//   RTclock.getTime();
//   lastHour = RTclock.hour;
//   lastMinute = RTclock.minute;
//   lastSecond = RTclock.second;
//   lastDay = RTclock.dayOfMonth;
//   lastMonth = RTclock.month;
//   lastYear = RTclock.year + 2000;
// }

// void syncRTCwithNTP()
// {
//   struct tm timeinfo;
//   if (getLocalTime(&timeinfo))
//   {
//     RTclock.stopClock();
//     RTclock.fillByYMD(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
//     RTclock.fillByHMS(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
//     RTclock.fillDayOfWeek(timeinfo.tm_wday);
//     RTclock.setTime();
//     RTclock.startClock();
//     updateTimeFromRTC();
//   }
// }

// void setup()
// {
//   Serial.begin(115200);
//   delay(2000);
//   Serial.println("System Starting...");

//   Wire.begin(4, 5);
//   delay(100);

//   if (sensor.init())
//   {
//     Serial.println("SHT35 init failed!!!");
//   }
//   else
//   {
//     Serial.println("SHT35 Online.");
//   }

//   u8g2.begin();
//   RTclock.begin();

//   WiFi.begin(ssid, password);

//   int attempts = 0;
//   while (WiFi.status() != WL_CONNECTED && attempts < 20)
//   {
//     delay(500);
//     attempts++;
//   }

//   //   if (WiFi.status() == WL_CONNECTED) {
//   //     // 1. Set the NTP servers (Standard numbers version)
//   // configTime(0, 0, "pool.ntp.org", "time.nist.gov");

//   // // 2. Set the Rexburg/Mountain Time environment variable
//   // setenv("TZ", "MST7MDT,M3.2.0,M11.1.0", 1);

//   // // 3. Apply the changes to the system clock
//   // tzset();

//   //     struct tm timeinfo;
//   //     if (getLocalTime(&timeinfo, 5000)) {
//   //       syncRTCwithNTP();
//   //       Serial.println("RTC Synced with NTP");
//   //     }
//   //   }
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     // 1. Set NTP servers with 0 offsets to satisfy the compiler
//     configTime(0, 0, "pool.ntp.org", "time.nist.gov");

//     // 2. Manually set the Rexburg/Mountain Time string
//     setenv("TZ", "MST7MDT,M3.2.0,M11.1.0", 1);
//     tzset();

//     struct tm timeinfo;
//     if (getLocalTime(&timeinfo, 5000))
//     {
//       syncRTCwithNTP();
//       Serial.println("RTC Synced with NTP");
//     }
//   }
//   WiFi.disconnect(true);
//   WiFi.mode(WIFI_OFF);
// }

// void loop()
// {
//   updateTimeFromRTC();

//   static unsigned long lastSync = 0;
//   if (millis() - lastSync > 3600000)
//   {
//     WiFi.begin(ssid, password);
//     //     if (WiFi.status() == WL_CONNECTED) {
//     //       // 1. Set the NTP servers (Standard numbers version)
//     // configTime(0, 0, "pool.ntp.org", "time.nist.gov");

//     // // 2. Set the Rexburg/Mountain Time environment variable
//     // setenv("TZ", "MST7MDT,M3.2.0,M11.1.0", 1);

//     // // 3. Apply the changes to the system clock
//     // tzset();
//     //       delay(2000);
//     //       syncRTCwithNTP();
//     //     }
//     if (WiFi.status() == WL_CONNECTED)
//     {
//       configTime(0, 0, "pool.ntp.org", "time.nist.gov");
//       setenv("TZ", "MST7MDT,M3.2.0,M11.1.0", 1);
//       tzset();
//       delay(2000);
//       syncRTCwithNTP();
//     }
//     WiFi.disconnect(true);
//     WiFi.mode(WIFI_OFF);
//     lastSync = millis();
//   }

//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB10_tr);

//   float temp, hum;
//   if (NO_ERROR != sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &temp, &hum))
//   {
//     u8g2.setCursor(0, 12);
//     u8g2.print("SHT35 Error");
//   }
//   else
//   {
//     // Temperature
//     u8g2.setCursor(0, 12);
//     u8g2.print("Temp: ");
//     u8g2.print((temp * 1.8) + 32);
//     u8g2.setFont(u8g2_font_unifont_t_symbols);
//     u8g2.drawGlyph(88, 10, 0x00b0);
//     u8g2.setFont(u8g2_font_ncenB10_tr);
//     u8g2.print(" F");

//     // Humidity
//     u8g2.setCursor(0, 27);
//     u8g2.print("Hum: ");
//     u8g2.print(hum);
//     u8g2.print(" %");

//     // Date
//     u8g2.setCursor(0, 42);
//     u8g2.print("Date: ");
//     u8g2.print(lastMonth);
//     u8g2.print("/");
//     u8g2.print(lastDay);
//     u8g2.print("/");
//     u8g2.print(lastYear);

//     // Time
//     u8g2.setCursor(0, 57);
//     char formattedTime[12];
//     int displayHour = lastHour % 12;
//     if (displayHour == 0)
//       displayHour = 12;
//     const char *period = (lastHour >= 12) ? "PM" : "AM";
//     sprintf(formattedTime, "%02d:%02d:%02d %s", displayHour, lastMinute, lastSecond, period);
//     u8g2.print("Time: ");
//     u8g2.print(formattedTime);
//   }
//   u8g2.sendBuffer();
//   delay(1000);
// }

/////////////////////////////////////////////////////////////////////////////////////////
// #include <Arduino.h>
// #include <Wire.h>
// #include <U8g2lib.h>
// #include "Seeed_SHT35.h"
// #include <WiFi.h>
// #include <NTPClient.h>
// #include <WiFiUdp.h>
// #include <time.h>
// #include "PCF85063TP.h"
// #include "secrets.h"

// // Initialize without pins to avoid early locking
// SHT35 sensor(SCL);
// // U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);
// U8G2_SSD1315_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// PCF85063TP RTclock;

// WiFiUDP udp;
// NTPClient timeClient(udp, "pool.ntp.org", 0, 3600000);

// int lastDay, lastMonth, lastYear, lastHour, lastMinute, lastSecond;

// void updateTimeFromRTC()
// {
//   RTclock.getTime();
//   lastHour = RTclock.hour;
//   lastMinute = RTclock.minute;
//   lastSecond = RTclock.second;
//   lastDay = RTclock.dayOfMonth;
//   lastMonth = RTclock.month;
//   lastYear = RTclock.year + 2000;
// }

// void syncRTCwithNTP()
// {
//   struct tm timeinfo;
//   if (getLocalTime(&timeinfo))
//   {
//     RTclock.stopClock();
//     RTclock.fillByYMD(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
//     RTclock.fillByHMS(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
//     RTclock.setTime();
//     RTclock.startClock();
//     updateTimeFromRTC();
//   }
// }

// void setup()
// {
//   Serial.begin(115200);
//   delay(2000);

//   // 1. START WIRE FIRST (The most important step)
//   Wire.begin(SDA, SCL);
//   delay(200);

//   u8g2.setI2CAddress(0x3C << 1); // U8g2 wants 8-bit address
//   u8g2.begin();
//   u8g2.setI2CAddress(0x3C << 1);
//   u8g2.begin();
//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB10_tr);
//   u8g2.drawStr(0, 30, "OLED OK");
//   u8g2.sendBuffer();
//   while (true)
//   {
//     delay(1000);
//   }

//   // 2. Start display and show "Loading"
//   u8g2.begin();
//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB10_tr);
//   u8g2.drawStr(0, 30, "Booting...");
//   u8g2.sendBuffer();

//   // 3. Start sensors
//   if (sensor.init())
//   {
//     Serial.println("SHT35 failed!!!");
//   }
//   else
//   {
//     Serial.println("SHT35 Online.");
//   }
//   RTclock.begin();

//   // 4. Time Sync
//   WiFi.begin(ssid, password);
//   int attempts = 0;
//   while (WiFi.status() != WL_CONNECTED && attempts < 15)
//   {
//     delay(500);
//     attempts++;
//   }

//   if (WiFi.status() == WL_CONNECTED)
//   {
//     configTime(0, 0, "pool.ntp.org", "time.nist.gov");
//     setenv("TZ", "MST7MDT,M3.2.0,M11.1.0", 1);
//     tzset();
//     struct tm timeinfo;
//     if (getLocalTime(&timeinfo, 5000))
//     {
//       syncRTCwithNTP();
//       Serial.println("RTC Synced");
//     }
//   }
//   WiFi.disconnect(true);
// }

// void loop()
// {
//   updateTimeFromRTC();

//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB08_tr);

//   float temp, hum;
//   if (NO_ERROR != sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &temp, &hum))
//   {
//     u8g2.drawStr(0, 12, "Sensor Error");
//   }
//   else
//   {
//     u8g2.setCursor(0, 12);
//     u8g2.print("Temp: ");
//     u8g2.print((temp * 1.8) + 32);
//     u8g2.print(" F");

//     u8g2.setCursor(0, 26);
//     u8g2.print("Hum: ");
//     u8g2.print(hum);
//     u8g2.print(" %");

//     u8g2.setCursor(0, 40);
//     u8g2.print("Date: ");
//     u8g2.print(lastMonth);
//     u8g2.print("/");
//     u8g2.print(lastDay);

//     u8g2.setCursor(0, 54);
//     char tBuffer[16];
//     int h12 = (lastHour % 12 == 0) ? 12 : lastHour % 12;
//     sprintf(tBuffer, "%02d:%02d:%02d %s", h12, lastMinute, lastSecond, (lastHour >= 12 ? "PM" : "AM"));
//     u8g2.print(tBuffer);
//   }
//   u8g2.sendBuffer();
//   delay(1000);
// }
/////////////////////////////////////////////////////////////////////////////
// #include <Arduino.h>
// #include <Wire.h>
// #include <U8g2lib.h>
// #include "Seeed_SHT35.h"
// #include <WiFi.h>
// #include <WiFiUdp.h>
// #include <NTPClient.h>
// #include <time.h>
// #include "PCF85063TP.h"
// #include "secrets.h"

// // Display: use SSD1315 + explicit address
// U8G2_SSD1315_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// // Sensor + RTC
// SHT35 sensor(SCL);
// PCF85063TP RTclock;

// WiFiUDP udp;
// NTPClient timeClient(udp, "pool.ntp.org", 0, 3600000);

// int lastDay = 0, lastMonth = 0, lastYear = 0;
// int lastHour = 0, lastMinute = 0, lastSecond = 0;

// void updateTimeFromRTC()
// {
//   RTclock.getTime();
//   lastHour = RTclock.hour;
//   lastMinute = RTclock.minute;
//   lastSecond = RTclock.second;
//   lastDay = RTclock.dayOfMonth;
//   lastMonth = RTclock.month;
//   lastYear = RTclock.year + 2000;
// }

// void syncRTCwithNTP()
// {
//   struct tm timeinfo;
//   if (getLocalTime(&timeinfo, 5000))
//   {
//     RTclock.stopClock();
//     RTclock.fillByYMD(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
//     RTclock.fillByHMS(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
//     RTclock.fillDayOfWeek(timeinfo.tm_wday);
//     RTclock.setTime();
//     RTclock.startClock();
//     updateTimeFromRTC();
//   }
// }

// bool connectWifi(int maxAttempts, int delayMs)
// {
//   WiFi.begin(ssid, password);
//   for (int i = 0; i < maxAttempts; i++)
//   {
//     if (WiFi.status() == WL_CONNECTED)
//       return true;
//     delay(delayMs);
//   }
//   return (WiFi.status() == WL_CONNECTED);
// }

// void setup()
// {
//   Serial.begin(115200);
//   delay(1500);

//   // I2C: use variant SDA/SCL (critical for your PIO setup)
//   Wire.begin(SDA, SCL);
//   Wire.setClock(100000);
//   delay(200);

//   // OLED init (critical)
//   u8g2.setI2CAddress(0x3C << 1);
//   u8g2.begin();
//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB10_tr);
//   u8g2.drawStr(0, 30, "Booting...");
//   u8g2.sendBuffer();

//   RTclock.begin();

//   if (sensor.init())
//   {
//     Serial.println("Sensor init failed!!!");
//   }
//   else
//   {
//     Serial.println("Sensor initialized");
//   }

//   // NTP -> POSIX TZ for MST/MDT (DST automatic)
//   if (connectWifi(20, 500))
//   {
//     configTime(0, 0, "pool.ntp.org", "time.nist.gov");
//     setenv("TZ", "MST7MDT,M3.2.0,M11.1.0", 1);
//     tzset();

//     syncRTCwithNTP();
//     Serial.println("RTC Synced with NTP");
//   }
//   else
//   {
//     Serial.println("WiFi not connected, using RTC time");
//     updateTimeFromRTC();
//   }

//   WiFi.disconnect(true);
//   WiFi.mode(WIFI_OFF);
// }

// void loop()
// {
//   updateTimeFromRTC();

//   // Hourly resync
//   static unsigned long lastSync = 0;
//   if (millis() - lastSync > 3600000UL)
//   {
//     if (connectWifi(15, 500))
//     {
//       configTime(0, 0, "pool.ntp.org", "time.nist.gov");
//       setenv("TZ", "MST7MDT,M3.2.0,M11.1.0", 1);
//       tzset();
//       syncRTCwithNTP();
//       Serial.println("RTC re-synced");
//     }
//     WiFi.disconnect(true);
//     WiFi.mode(WIFI_OFF);
//     lastSync = millis();
//   }

//   u8g2.clearBuffer();
//   u8g2.setFont(u8g2_font_ncenB10_tr);

//   float temp, hum;
//   if (NO_ERROR != sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &temp, &hum))
//   {
//     u8g2.drawStr(0, 12, "Read temp failed!!");
//   }
//   else
//   {
//     u8g2.setCursor(0, 12);
//     u8g2.print("Temp: ");
//     u8g2.print((temp * 1.8) + 32);
//     u8g2.print(" F");

//     u8g2.setCursor(0, 27);
//     u8g2.print("Hum: ");
//     u8g2.print(hum);
//     u8g2.print(" %");

//     u8g2.setCursor(0, 42);
//     u8g2.print("Date: ");
//     u8g2.print(lastMonth);
//     u8g2.print("/");
//     u8g2.print(lastDay);
//     u8g2.print("/");
//     u8g2.print(lastYear);

//     u8g2.setCursor(0, 57);
//     char formattedTime[16];
//     int displayHour = lastHour % 12;
//     if (displayHour == 0)
//       displayHour = 12;
//     const char *period = (lastHour >= 12) ? "PM" : "AM";
//     sprintf(formattedTime, "%02d:%02d:%02d %s", displayHour, lastMinute, lastSecond, period);
//     u8g2.print("Time: ");
//     u8g2.print(formattedTime);
//   }

//   u8g2.sendBuffer();
//   delay(1000);
// }
///////////////////////////////////////////////////////
#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "Seeed_SHT35.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <time.h>
#include "PCF85063TP.h"
#include "secrets.h"

// --------------------
// Hardware / Libraries
// --------------------

// Grove SHT35: this library expects the SCL pin constant
SHT35 sensor(SCL);

// Grove OLED SSD1315: use the SSD1315 constructor and set address explicitly
U8G2_SSD1315_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
static constexpr uint8_t OLED_ADDR_7BIT = 0x3C;

// Grove High Precision RTC v1.0 (PCF85063)
PCF85063TP RTclock;
static constexpr uint8_t RTC_ADDR_7BIT = 0x51;

// NTP (we'll use configTime + getLocalTime; NTPClient not required but kept for your structure)
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 0, 3600000);

// --------------------
// Time configuration (match your original approach)
// --------------------
int tzOffsetHours = -7; // Mountain Time standard offset
bool dstActive = true;  // add 1 hour when DST active

// --------------------
// Stored last-known-good time (what you display)
// --------------------
int lastDay = 0, lastMonth = 0, lastYear = 0;
int lastHour = 0, lastMinute = 0, lastSecond = 0;

bool rtcPresent = false;      // detected on I2C
bool rtcTrusted = false;      // reads produce sane values
bool systemTimeValid = false; // NTP (or RTC->system) set system clock at least once

// --------------------
// Helpers
// --------------------
bool i2cDevicePresent(uint8_t addr7)
{
  Wire.beginTransmission(addr7);
  return (Wire.endTransmission() == 0);
}

bool timeFieldsSane(int year, int month, int day, int hour, int minute, int second)
{
  if (year < 2020 || year > 2099)
    return false;
  if (month < 1 || month > 12)
    return false;
  if (day < 1 || day > 31)
    return false;
  if (hour < 0 || hour > 23)
    return false;
  if (minute < 0 || minute > 59)
    return false;
  if (second < 0 || second > 59)
    return false;
  return true;
}

bool updateDisplayedTimeFromSystem()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 1000))
    return false;

  const int y = timeinfo.tm_year + 1900;
  const int mo = timeinfo.tm_mon + 1;
  const int d = timeinfo.tm_mday;
  const int h = timeinfo.tm_hour;
  const int mi = timeinfo.tm_min;
  const int s = timeinfo.tm_sec;

  if (!timeFieldsSane(y, mo, d, h, mi, s))
    return false;

  lastYear = y;
  lastMonth = mo;
  lastDay = d;
  lastHour = h;
  lastMinute = mi;
  lastSecond = s;
  return true;
}

bool updateDisplayedTimeFromRTC()
{
  if (!rtcPresent)
    return false;

  RTclock.getTime();

  const int y = RTclock.year + 2000;
  const int mo = RTclock.month;
  const int d = RTclock.dayOfMonth;
  const int h = RTclock.hour;
  const int mi = RTclock.minute;
  const int s = RTclock.second;

  if (!timeFieldsSane(y, mo, d, h, mi, s))
    return false;

  lastYear = y;
  lastMonth = mo;
  lastDay = d;
  lastHour = h;
  lastMinute = mi;
  lastSecond = s;
  return true;
}

void syncRTCwithSystemTime()
{
  // Only call this when getLocalTime() succeeds
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 2000))
    return;

  RTclock.stopClock();
  RTclock.fillByYMD(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
  RTclock.fillByHMS(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  RTclock.fillDayOfWeek(timeinfo.tm_wday);
  RTclock.setTime();
  RTclock.startClock();
}

bool connectWiFiWithTimeout(int maxAttempts, int delayMs)
{
  WiFi.begin(ssid, password);
  for (int i = 0; i < maxAttempts; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
      return true;
    delay(delayMs);
  }
  return (WiFi.status() == WL_CONNECTED);
}

void disconnectWiFi()
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

// --------------------
// Setup
// --------------------
void setup()
{
  Serial.begin(115200);
  delay(1200);

  // Critical for PlatformIO on your XIAO ESP32C3: use board variant SDA/SCL
  Wire.begin(SDA, SCL);
  Wire.setClock(100000);
  delay(200);

  // OLED init (critical): SSD1315 + explicit address
  u8g2.setI2CAddress(OLED_ADDR_7BIT << 1); // U8g2 uses 8-bit address
  u8g2.begin();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(0, 30, "Booting...");
  u8g2.sendBuffer();

  // Sensor init
  if (sensor.init())
    Serial.println("SHT35 init failed!!!");
  else
    Serial.println("SHT35 Online.");

  // Detect RTC presence before using it
  rtcPresent = i2cDevicePresent(RTC_ADDR_7BIT);
  if (rtcPresent)
  {
    RTclock.begin();
    rtcTrusted = updateDisplayedTimeFromRTC();
    Serial.println(rtcTrusted ? "RTC detected and readable." : "RTC detected but not trusted.");
  }
  else
  {
    rtcTrusted = false;
    Serial.println("RTC not detected (continuing without it).");
  }

  // Try to get system time from NTP at boot if Wi-Fi is available
  if (connectWiFiWithTimeout(20, 500))
  {
    // Match your original approach (offset + dstActive)
    long tzOffsetSeconds = (long)tzOffsetHours * 3600L;
    long dstOffsetSeconds = dstActive ? 3600L : 0L;
    configTime(tzOffsetSeconds, dstOffsetSeconds, "pool.ntp.org", "time.nist.gov");

    // Wait for time to become available
    if (updateDisplayedTimeFromSystem())
    {
      systemTimeValid = true;
      Serial.println("System time set from NTP.");

      // If RTC exists, sync it to NTP/system time
      if (rtcPresent)
      {
        syncRTCwithSystemTime();
        rtcTrusted = true; // after sync, it's very likely good
        Serial.println("RTC synced from NTP/system time.");
      }
    }
    else
    {
      Serial.println("NTP did not provide time (boot).");
      systemTimeValid = false;
    }

    disconnectWiFi();
  }
  else
  {
    Serial.println("WiFi not connected at boot.");
    systemTimeValid = false;

    // If no NTP, fall back to RTC if it is good
    if (rtcTrusted)
      Serial.println("Using RTC time.");
    else
      Serial.println("No valid time source yet (will show last-known/blank time fields).");
  }
}

// --------------------
// Loop
// --------------------
void loop()
{
  // Prefer system time if valid (NTP-set clock keeps ticking even with Wi-Fi off)
  bool timeOk = false;

  if (systemTimeValid)
    timeOk = updateDisplayedTimeFromSystem();

  // If system time isn't valid yet, try RTC (if present)
  if (!timeOk && rtcTrusted)
    timeOk = updateDisplayedTimeFromRTC();

  // Hourly resync attempt (only if Wi-Fi available)
  static unsigned long lastSyncMs = 0;
  if (millis() - lastSyncMs > 3600000UL)
  {
    if (connectWiFiWithTimeout(15, 500))
    {
      long tzOffsetSeconds = (long)tzOffsetHours * 3600L;
      long dstOffsetSeconds = dstActive ? 3600L : 0L;
      configTime(tzOffsetSeconds, dstOffsetSeconds, "pool.ntp.org", "time.nist.gov");

      if (updateDisplayedTimeFromSystem())
      {
        systemTimeValid = true;
        if (rtcPresent)
        {
          syncRTCwithSystemTime();
          rtcTrusted = true;
        }
        Serial.println("Hourly NTP sync OK.");
      }
      else
      {
        Serial.println("Hourly NTP sync failed.");
      }

      disconnectWiFi();
    }
    lastSyncMs = millis();
  }

  // --------------------
  // Display (restore your original font/placement + degree glyph)
  // --------------------
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr);

  float temp, hum;
  if (NO_ERROR != sensor.read_meas_data_single_shot(HIGH_REP_WITH_STRCH, &temp, &hum))
  {
    u8g2.setCursor(0, 12);
    u8g2.print("Read temp failed!!");
  }
  else
  {
    u8g2.setCursor(0, 12);
    u8g2.print("Temp: ");

    int tempX = u8g2.getCursorX(); // position after "Temp: "
    u8g2.print((temp * 1.8) + 32); // prints the number

    int afterNumX = u8g2.getCursorX();

    // add a normal space before the degree symbol
    u8g2.print(" ");
    afterNumX = u8g2.getCursorX();

    // draw the degree slightly above the baseline, and a little right of the space
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(afterNumX, 10, 0x00b0); // y=10 matches your old look; adjust to 9 if you want it slightly higher
    // u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.setFont(u8g2_font_helvB10_tr);

    // then print "F" with a small space after the degree
    u8g2.print(" F");

    u8g2.setCursor(0, 27);
    u8g2.print("Hum: ");
    u8g2.print(hum);
    u8g2.print(" %");

    u8g2.setCursor(0, 42);
    u8g2.print("Date: ");
    if (timeOk || systemTimeValid || rtcTrusted)
    {
      u8g2.print(lastMonth);
      u8g2.print("/");
      u8g2.print(lastDay);
      u8g2.print("/");
      u8g2.print(lastYear);
    }
    else
    {
      u8g2.print("--/--/----");
    }

    u8g2.setCursor(0, 57);
    u8g2.print("Time: ");
    if (timeOk || systemTimeValid || rtcTrusted)
    {
      char formattedTime[12];
      int displayHour = lastHour % 12;
      if (displayHour == 0)
        displayHour = 12;
      const char *period = (lastHour >= 12) ? "PM" : "AM";
      sprintf(formattedTime, "%02d:%02d:%02d %s", displayHour, lastMinute, lastSecond, period);
      u8g2.print(formattedTime);
    }
    else
    {
      u8g2.print("--:--:--");
    }
  }

  u8g2.sendBuffer();
  delay(1000);
}
