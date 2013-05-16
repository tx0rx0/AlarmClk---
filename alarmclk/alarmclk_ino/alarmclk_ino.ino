#include <rtc_clock.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SPI.h>
#include <Audio.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
RTC_clock rtc_clock(XTAL);
const int button1 = 12;
const int button2 = 13;
int mode = 0;
int b1state, b2state;
int a,b,c,y,z,t,n,h=12,i=0;



void setup() {
  Serial.begin(9600);
  rtc_clock.init();
  rtc_clock.set_time(12,0,0);   //set time, hour, minutes, seconds
  rtc_clock.set_date(14,5,2013); // set date day, month, year
  lcd.begin(16,2); 
  pinMode(button1, INPUT); //set left button
  pinMode(button2, INPUT); //set right button
  rtc_clock.set_alarmtime(h,i,0); //set alarm time
  rtc_clock.attachalarm(announcement);  
  
  
Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println(" failed!");
    return;
  }
  Serial.println(" done.");
  // hi-speed SPI transfers
  SPI.setClockDivider(4);
  // 44100Khz stereo => 88200 sample rate
  // 100 mSec of prebuffering.
  Audio.begin(88200, 100);
}

void loop() {
    if(b1state==0 && b2state == 0) //if both buttons are pressed, enters mode select
    {
      selectmode();
    }
    else
    {
      if(t != rtc_clock.get_seconds())
      {
        lcd.print(rtc_clock.get_hours()); //prints hours
        lcd.print(":");
        lcd.print(rtc_clock.get_minutes()); //prints minutes
        lcd.print(":");
        lcd.print(rtc_clock.get_seconds()); //prints seconds
        t = rtc_clock.get_seconds();
      }
    
    b1state = digitalRead(button1); 
    b2state = digitalRead(button2);
    }
  
}

void selectmode()
{
  mode = 1;
  z = 0;
  y = 0;
  while(mode == 1)
  {
        for(n=0;n<100;n++)
        {
           b1state = digitalRead(button1); 
           b2state = digitalRead(button2);
               if (b1state == 0) //if button 1 is pressed then it changes the item one item to the right
               {                //order goes select menu, set time, alarm, other
                 z++;
                 y=0;
                 n=0;    
               }
               if (b2state == 0) //same as button 1 but opposite order
               {
                 z--; //sets the mode 1 = time, 2= alarm, 3 = other, 0 = nothing
                 y=0; //denys lcd print the right to refresh itself unless I tell it to...hahahha
                 n=0;
               }
               if(z==1 && y==0)
               {
                 lcd.print("set time");
                 y = 1;
               }
               if(z==2 && y==0)
               {
                 lcd.print("set alarm");
                 y = 1;
               } 
               if(z==3 && y==0)
               {
                  lcd.print("other");
                  y = 1;
               }
               if(z==0 && y==0)
               {
                  lcd.print("select menu");
                  y = 1;
               }
               if(z==4)
               {
                   z=0;
               } 
         delay(30);
        }
        y=0;
         while(z==1) //if after a set amount of time, and the set time is displayed, it will go into set time mode
         {
            b = rtc_clock.get_minutes();
            a = rtc_clock.get_hours();
            c = rtc_clock.get_seconds();
            
            b1state = digitalRead(button1); 
            b2state = digitalRead(button2);
            if(b1state==0)
            {
              b++;
              if(b>60)
              {
                b=0;
                a++;
              }
              y=0;
              delay(50);
            }
            if(b2state==0)
            {
              b--;
              if(b<0)
              {
                b=0;
                a--;
              }
              y=0;
              delay(50);
            }
            if((a!=rtc_clock.get_hours()) || (b!= rtc_clock.get_minutes()) || (c!= rtc_clock.get_seconds()) || (y==0))
            {
              lcd.print(rtc_clock.get_hours()); //prints hours
              lcd.print(":");
              lcd.print(rtc_clock.get_minutes()); //prints minutes
              lcd.print(":");
              lcd.print(rtc_clock.get_seconds()); //prints seconds
            }
            rtc_clock.set_time(a,b,c);
            y++;
            if(y==100)
            {
              z=0;
              mode=1;
            }
         }
         while(z==2)
         {
           b1state = digitalRead(button1); 
           b2state = digitalRead(button2);
           a=h;
           b=i;
           if(b1state==0)
            {
              b++;
              if(b>60)
              {
                b=0;
                a++;
              }
              y=0;
              delay(50);
            }
            if(b2state==0)
            {
              b--;
              if(b<0)
              {
                b=0;
                a--;
              }
              delay(50);
            }
           if((a!=h) || (b!= i)|| (y==0))
            {
              lcd.print(h); //prints alarm hours
              lcd.print(":");
              lcd.print(i); //prints alarm minutes
              lcd.print(":");
              lcd.print(00); //prints alarm seconds
            }
           
            
         }
  }
}
  
  
void announcement() {
  Serial.println();
  Serial.println("ALARM");
  
  int count=0;
  File myFile = SD.open("test.wav");
  if (!myFile) {
    // if the file didn't open, print an error and stop
    Serial.println("error opening test.wav");
    while (true);
  }

  const int S=1024; // Number of samples to read in block
  short buffer[S];

  Serial.print("Playing");
  // until the file is not finished
  while (myFile.available()) {
    // read from the file into buffer
    myFile.read(buffer, sizeof(buffer));

    // Prepare samples
    int volume = 1024;
    Audio.prepare(buffer, S, volume);
    // Feed samples to audio
    Audio.write(buffer, S);

    // Every 100 block print a '.'
    count++;
    if (count == 100) {
      Serial.print(".");
      count = 0;
    }
  }
  myFile.close();

  Serial.println("End of file. Thank you for listening!");
  while (true) ;
  
}
