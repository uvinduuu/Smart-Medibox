//EN2853 - Embedded Systems and Applications - Programming Assignment 01
//Medibox
//Kodikara U. S. S. 210293K (ENTC)

//Libraries
#include <Wire.h> //I2C 
#include <Adafruit_GFX.h> //Graphics
#include <Adafruit_SSD1306.h> //OLED Display
#include <DHTesp.h> //DHT
#include <WiFi.h> //WiFi

//Screen parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1
#define SCREEN_ADDRESS 0x3C

//NTP Server
#define NTP_SERVER   "pool.ntp.org"
int UTC_OFFSET = 0;
int UTC_OFFSET_DST = 0;

//Object Creation
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,SCREEN_RESET);
DHTesp dhtSensor;

//Pin definition
#define BUZZER 16 
#define LED_1 5
#define PB_CANCEL 33
#define PB_OK 34
#define PB_UP 35
#define PB_DOWN 32
#define DHTPIN 17

// Gllobal Variables
//Time
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long  time_now = 0;
unsigned long  time_last = 0;

//alarms
bool alarm_enabled = true;
int n_alarms = 3;
int alarm_hours[] = {0,1,2};
int alarm_minutes[] = {0,10,20};
bool alarm_triggered[] = {false,false,false};

//Notes for the buzzer
int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;

int notes[] = {C,D,E,F,G,A,B,C_H};

//States of Menu
int current_mode = 0;
int max_modes = 5;
String modes[] = {"1 - Set Time Zone","2 - Set Alarm 1","3 - Set Alarm 2","4 - Set Alarm 3","5 - Disable Alarms"};

void setup() {

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK,INPUT);
  pinMode(PB_UP,INPUT);
  pinMode(PB_DOWN,INPUT);

  dhtSensor.setup(DHTPIN,DHTesp::DHT22);
  
  //Initializing serial monitor and OLED display
  Serial.begin(115200);
  Serial.println("MediBox is Initializing!");

  if(!display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS)){
    Serial.println(F("Medibox SSD1306 Display Allocation Failed"));
    for(;;);
  }
  //Turn on the display
  display.display();
  delay(2000);
  //Clear display
  display.clearDisplay();
  printScreen("  Medibox",0,0,2);

  WiFi.begin("Wokwi-GUEST","",6);
  while(WiFi.status() != WL_CONNECTED){
    delay(250);
    display.clearDisplay();
    printScreen("Connecting to WiFi",0,0,2);
  }

  display.clearDisplay();
  printScreen(" Connected to WiFi",0,0,2);
  delay(1000);

  configTime(UTC_OFFSET,UTC_OFFSET_DST,NTP_SERVER);

  display.clearDisplay();
  printScreen("Welcome to Medibox!",0,0,2);

  for(int i=0;i<8;i++){
    tone(BUZZER,notes[i]); //Welcome sound
    delay(60);
  }
  noTone(BUZZER);

  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  update_time_with_check_alarm();
  if(digitalRead(PB_OK) == LOW){
    beep();
    delay(200);
    go_to_menu();
  }
  checkTemp();
}


//Function - Printing in OLED display
void printScreen(String text,int column,int row,int text_size){
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column,row); // Column,Row
  display.println(text);
  display.display();
}

//Function - check temperature and Humidity
void checkTemp(){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if(data.temperature > 32){
    display.clearDisplay();
    printScreen(" TEMP HIGH",0,30,2);
    delay(1000);
  }
  else if(data.temperature < 26){
    display.clearDisplay();
    printScreen(" TEMP LOW",0,30,2);
    delay(1000);
  }
  if(data.humidity > 80){
    display.clearDisplay();
    printScreen("  HUMIDITY",0,20,2);
    printScreen("    HIGH",0,40,2);
    delay(1000);
  }
  else if(data.humidity < 60){
    display.clearDisplay();
    printScreen("  HUMIDITY",0,20,2);
    printScreen("    LOW",0,40,2);
    delay(1000);
  }
}

//Function - Print time
void print_time_now(void){

  display.clearDisplay();
  printScreen(String(days), 0, 0, 2);
  printScreen("| ", 20, 0, 2);
  printScreen(String (hours), 30, 0, 2);
  printScreen(":", 50, 0, 2);
  printScreen(String(minutes),60, 0, 2);
  printScreen(" :", 80, 0, 2);
  printScreen(String(seconds),90, 0, 2);
}

//Function - Update time
void update_time(void){

  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char timeHour[3];
  strftime(timeHour,3,"%H",&timeinfo);
  hours = atoi(timeHour);

  char timeMinute[3];
  strftime(timeMinute,3,"%M",&timeinfo);
  minutes = atoi(timeMinute);

  char timeSecond[3];
  strftime(timeSecond,3,"%S",&timeinfo);
  seconds = atoi(timeSecond);

  char timeDay[3];
  strftime(timeDay,3,"%d",&timeinfo);
  days = atoi(timeDay);
}

//Function - Updating the time while checking the alarm
void update_time_with_check_alarm(void){
  print_time_now();
  update_time();

  if(alarm_enabled == true){
    for(int i=0;i<n_alarms;i++){
      if(alarm_triggered[i] == false && alarm_hours[i] == hours && alarm_minutes[i] == minutes){
        ring_alarm();
        alarm_triggered[i] = true;
      }
    }
  }

}

//Function - Ringing the alarm
void ring_alarm(){
  display.clearDisplay();
  printScreen("Medicine Time!",0,0,2);
  digitalWrite(LED_1,HIGH);

  bool break_happened = false;

  while(digitalRead(PB_CANCEL) == HIGH && break_happened == false){
    // Ring the Buzzer
    for(int i=0;i<n_notes;i++){
      if(digitalRead(PB_CANCEL) == LOW){
        delay(200);
        break_happened = true;
        break;
      }
      tone(BUZZER,notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
    }
  }

  digitalWrite(LED_1,LOW);
  display.clearDisplay();
}

//Function - Playing a beep when press a button
void beep(){
  tone(BUZZER,notes[3]); 
  delay(10);
  noTone(BUZZER);
  delay(10);
}

//Function - Button press
int wait_for_button_press(){
    while(true){
    if(digitalRead(PB_UP) == LOW){
      beep();
      delay(200);
      return PB_UP;
    }
    else if(digitalRead(PB_DOWN) == LOW){
      beep();
      delay(200);
      return PB_DOWN;
    }
    else if(digitalRead(PB_OK) == LOW){
      beep();
      delay(200);
      return PB_OK;
    }
    else if(digitalRead(PB_CANCEL) == LOW){
      beep();
      delay(200);
      return PB_CANCEL;
    }
    update_time();
  }
}

//Function - Menu
void go_to_menu(void){  
  display.clearDisplay();
  printScreen("Menu",0,2,2);
  delay(1000);

  while(digitalRead(PB_CANCEL) == HIGH){
    display.clearDisplay();
    printScreen(modes[current_mode],0,2,2);
    delay(1000);

    int pressed = wait_for_button_press();

    if(pressed == PB_UP){
      delay(200);
      current_mode += 1;
      current_mode = current_mode % max_modes;
    }
    else if(pressed == PB_DOWN){
      delay(200);
      current_mode -= 1;
      if(current_mode <= -1){
        current_mode = max_modes - 1;
      }
    }
    else if(pressed == PB_OK){
      delay(200);
      run_mode(current_mode);
    }
    else if(pressed == PB_CANCEL){
      delay(200);
      break;
    }
  }
  }

//Function - Run the mode
void run_mode(int mode){
  if(mode == 0){
    set_time_zone();
  }
  else if(mode == 1 || mode == 2 || mode == 3){
    set_alarm(mode - 1);
  }
  else if(mode == 4){
    alarm_enabled = false;
  }
}

//Function - Set the time zone
void set_time_zone(){
  int temp_hour = 0;

  while(true){
    display.clearDisplay();

    printScreen("Enter hour " + String(temp_hour),0,0,2); //setting the hour

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_hour += 1;
        if (temp_hour > 12) {
          temp_hour = -12; //maximum time zone difference = 12 hours
      }
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_hour -= 1;
        if (temp_hour < -12) {
          temp_hour = 12; //minimum time zone difference = -12 hours
      }
      }
      else if(pressed == PB_OK){
        delay(200);
        UTC_OFFSET = (temp_hour * 3600); //offset must be added in seconds
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  int temp_minute = 0;

  while(true){
    display.clearDisplay();

    printScreen("Enter Minute: " + String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_minute += 30; //time zone changing by 30 minutes
        temp_minute = temp_minute % 60;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_minute -= 30;
        if(temp_minute <= -1){
          temp_minute = 60;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        if(temp_hour >= 0){
          UTC_OFFSET = UTC_OFFSET+(temp_minute * 60); //converting into seconds
        }
        else{
          UTC_OFFSET = UTC_OFFSET-(temp_minute * 60);
        }
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  configTime(UTC_OFFSET,UTC_OFFSET,NTP_SERVER); //setting the time


  display.clearDisplay();
  printScreen("Time Zone is set!",0,0,2); //display message
  delay(1000);

}


//Function - Set alarms
void set_alarm(int alarm){
  int temp_hour = alarm_hours[alarm];

  while(true){
    display.clearDisplay();
    printScreen("Enter hour: " + String(temp_hour),0,0,2); 

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_hour += 1;
        temp_hour = temp_hour % 24;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_hour -= 1;
        if(temp_hour <= -1){
          temp_hour = 23;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        alarm_hours[alarm] = temp_hour;
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  int temp_minute = alarm_minutes[alarm];

  while(true){
    display.clearDisplay();
    printScreen("Enter Minute: " + String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_minute += 1;
        temp_minute = temp_minute % 60;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_minute -= 1;
        if(temp_minute <= -1){
          temp_minute = 59;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        alarm_minutes[alarm] = temp_minute;
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  display.clearDisplay();
  alarm_enabled = true;
  alarm_triggered[alarm] = false;
  printScreen("Alarm "+String(alarm+1)+" is set",0,0,2);
  delay(1000);
}
