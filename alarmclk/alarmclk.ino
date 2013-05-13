
#include <rtc_clock.h>

//Set Clock Source
RTC_clock rtc_clock(XTAL);

char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
int hh,mm,ss,dow,dd,mon,yyyy;

void setup() {

  Serial.begin(9600);
  rtc_clock.init();
  rtc_clock.set_time(__TIME__);
  rtc_clock.set_date(__DATE__);

}

void loop() {

  
}
