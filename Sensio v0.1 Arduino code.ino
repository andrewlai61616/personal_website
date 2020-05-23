// DV project Main Program

////////// Code Start //////////
#include <Time.h>
#include <avr/pgmspace.h>
#include <DS1302RTC.h>
#include <EEPROM.h>
#include "pitches.h"

// Music Files
const int MUSICFILENUM = 10;
#define pgm_read_float_near(address_short) __LPM_float((uint16_t)(address_short))
const int melodyn[] = { 19, 16, 41, 18, 14, 22, 42, 87, 96, 152 };
const int PROGMEM melody[10][160] = {
    {NOTE_C5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_C5, 0,
    NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_C5, 0},
    {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_F4,
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4},
    {NOTE_G5, NOTE_G5, NOTE_E5, NOTE_F5, NOTE_F5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_D5,
    NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
    NOTE_G5, NOTE_G5, NOTE_E5, NOTE_A5, NOTE_A5, NOTE_D5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5,
    NOTE_CS5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_D5, NOTE_C5},
    {NOTE_G5, NOTE_D6, NOTE_G5, NOTE_AS5, NOTE_D6, NOTE_F6, NOTE_DS6, NOTE_D6, NOTE_DS6,
    NOTE_F5, NOTE_C6, NOTE_F5, NOTE_A5, NOTE_C6, NOTE_DS6, NOTE_D6, NOTE_C6, NOTE_D6},
    {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5,
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_F5},
    {NOTE_E5, NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_G5,
    NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_F5, NOTE_B4, NOTE_D5,
    NOTE_G4, NOTE_D5, NOTE_F5, NOTE_F5, NOTE_D5, NOTE_B4, NOTE_D5, NOTE_C5},
    {NOTE_C5, NOTE_C5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_G5,
    NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5,
    NOTE_G5, NOTE_G5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5,
    NOTE_G5, NOTE_G5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5,
    NOTE_C5, NOTE_C5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_A5, NOTE_G5,
    NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5},
    {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_G5, NOTE_C6, NOTE_B5, 
    NOTE_C6, NOTE_D6, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_A5, NOTE_B5, NOTE_G5, NOTE_D6, 
    NOTE_G6, NOTE_F6, NOTE_G6, NOTE_E6, NOTE_A6, NOTE_G6, NOTE_F6, NOTE_E6, NOTE_G6, NOTE_F6, 
    NOTE_A6, NOTE_G6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_E6, NOTE_D6, NOTE_F6, NOTE_E6, 
    NOTE_D6, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_C6, NOTE_B5, NOTE_D6, NOTE_C6, NOTE_B5, NOTE_A5, 
    NOTE_G5, NOTE_FS5, NOTE_A5, NOTE_G5, NOTE_B5, NOTE_A5, NOTE_D5, NOTE_C6, NOTE_D6, NOTE_B5,
    NOTE_A5, NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_G5, NOTE_FS5, NOTE_A5, NOTE_G5, NOTE_B5, NOTE_A5, 
    NOTE_C6, NOTE_B5, NOTE_D6, NOTE_C6, NOTE_E6, NOTE_D6, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_G6, 
    NOTE_C6, NOTE_B5, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_G5},
    {NOTE_G6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_G6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6, 
    NOTE_G6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_G6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6, 
    NOTE_G6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_G6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_D6,
    NOTE_G6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_G6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_D6,
    NOTE_G6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_F6, NOTE_E6, NOTE_G6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_F6, NOTE_E6,
    NOTE_F6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_F6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_D6,
    NOTE_E6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_E6, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6, 
    NOTE_D6, NOTE_E6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_E6, NOTE_F6},
    {NOTE_E6, NOTE_E6, NOTE_E6, NOTE_C6, NOTE_A5, NOTE_G5, NOTE_C6, NOTE_C6, NOTE_C6, NOTE_A5, NOTE_FS5, NOTE_E5,
    NOTE_E6, NOTE_E6, NOTE_E6, NOTE_C6, NOTE_A5, NOTE_E6, NOTE_E6, NOTE_C6,
    NOTE_E6, NOTE_E6, NOTE_E6, NOTE_C6, NOTE_A5, NOTE_G5, NOTE_C6, NOTE_C6, NOTE_C6, NOTE_A5, NOTE_FS5, NOTE_E5,
    NOTE_E6, NOTE_E6, NOTE_E6, NOTE_C6, NOTE_A5, NOTE_E6, NOTE_E6, NOTE_C6,      NOTE_C6, NOTE_D6, NOTE_DS6,
    NOTE_E6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_D6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_C6, NOTE_B5, NOTE_C6, NOTE_A5, NOTE_B5, NOTE_C6,
    NOTE_D6, NOTE_D6, NOTE_D6, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_CS6 ,NOTE_D6, NOTE_G5,
    NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_B6, NOTE_GS6, NOTE_E6, NOTE_E7, NOTE_B6, NOTE_D7, NOTE_B6, NOTE_A6, NOTE_B6,
    NOTE_GS6, NOTE_F6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_A6, 
    NOTE_GS6, NOTE_E6, NOTE_D6, NOTE_DS6, NOTE_A6, NOTE_E6, NOTE_G6, NOTE_FS6, NOTE_F6, NOTE_E6, NOTE_F6, NOTE_A6,
    NOTE_E7, NOTE_D7, NOTE_A6, NOTE_GS6, NOTE_G6, NOTE_E7, NOTE_G6, NOTE_FS6, NOTE_F6, NOTE_D7, NOTE_A6, NOTE_AS6,
    NOTE_B6, NOTE_AS6, NOTE_B6, NOTE_A6, NOTE_G6, NOTE_F6, NOTE_A5, NOTE_B5, NOTE_G6, NOTE_F6, NOTE_B5, NOTE_A5, 
    NOTE_F6, NOTE_B5, NOTE_A5, NOTE_G5,
    NOTE_E6, NOTE_E6, NOTE_E6, NOTE_C6, NOTE_A5, NOTE_G5, NOTE_C6, NOTE_C6, NOTE_C6, NOTE_A5, NOTE_FS5, NOTE_E5,
    NOTE_E6, NOTE_E6, NOTE_E6, NOTE_C6, NOTE_A5, NOTE_E6, NOTE_E6, NOTE_C6}
};
const float PROGMEM noteDurations[10][160] = {
    {4, 8, 8, 4, 4, 4, 4, 4, 4, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4},
    {4, 4, 2, 2, 2, 2, 1.33, 4, 2, 4, 4, 2, 2, 4, 2, 0.57},
    {4, 4, 2, 4, 4, 2, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    4, 4, 2, 4, 4, 2, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2},
    {2, 2, 4, 4, 4, 2, 4, 4, 0.8, 2, 2, 4, 4, 4, 2, 4, 4, 0.8},
    {2, 6, 2, 6, 3, 3, 1.5, 2, 6, 6, 6, 6, 6, 0.75},
    {4, 2, 4, 2, 4, 4, 8, 8, 8, 8, 2, 4, 2, 4, 2, 4, 4, 8, 8, 8, 8, 2},
    {3, 3, 3, 3, 3, 3, 1.5, 3, 3, 3, 3, 3, 3, 1.5, 3, 3, 3, 3, 3, 3, 1.5,
    3, 3, 3, 3, 3, 3, 1.5, 3, 3, 3, 3, 3, 3, 1.5, 3, 3, 3, 3, 3, 3, 1.5},
    {10, 10, 10, 10, 10, 10, 10, 5, 5, 5,
    5, 10, 10, 10, 10, 10, 10, 10, 10, 5,
    5, 5, 5, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 5, 5, 3.333, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 20, 20, 10, 10,
    20, 20, 20, 6.67, 10, 10, 2},
    {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    {2, 2.66, 4, 4, 8, 2, 2, 2.66, 4, 4, 8, 2, 
    2, 2.66, 4, 4, 8, 4, 4, 0.5,
    2, 2.66, 4, 4, 8, 2, 2, 2.66, 4, 4, 8, 2, 
    2, 2.66, 4, 4, 8, 4, 4, 0.8, 4, 4, 4,
    2, 2.66, 4, 2.66, 4, 4, 4, 4, 8, 4, 2.66, 4, 4, 4,
    2, 2.66, 4, 2.66, 4, 2, 4, 4, 4, 1.33, 4,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    2, 2.66, 4, 4, 8, 2, 2, 2.66, 4, 4, 8, 2, 
    2, 2.66, 4, 4, 8, 4, 4, 0.5}
    
    
};

const int MAXFANDELAY = 50, MINFANDELAY = 1;

const int NormalDisplayMode = 1, SetTimeMode = 2, SetAlarmMode = 3,
	SetAlarmMusicMode = 4, SetFanDelayMode = 5, RingingMode = 6;
const int SetMinute = 1, SetHour = 2, SetDate = 3, SetMonth = 4, SetYear = 5, AlarmSetMinute = 1, AlarmSetHour = 2;

// EEPROM Save Address
const int FanDelay_EEPROM = 21, FanOn_EEPROM = 22, SoundOn_EEPROM = 23, 
	AlarmMusicSelect_EEPROM = 24, AlarmHour_EEPROM = 25, AlarmMinute_EEPROM = 26;

// I/O Pins
const int SoundYellowLightPin = 4, FanRedLightPin = 2, FanPin = 12, BeeperPin = 13;
const int CEPin = A0, IOPin = A4, CLKPin = A5;
const int BlueButtonPin = A1, GreenButtonPin = A2, OrangeButtonPin = A3;
const int latchPin = 7, clockPin = 10, dataPin = 3;
const int HR_ten_Pin = 5, HR_one_Pin = 9, MIN_ten_Pin = 8, MIN_one_Pin = 6;


const int MonthMaxDay[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
const int digit_Pin[] = { 6,8,9,5 }, dotsn[] = { 6,2,5,5,4,5,6,4,7,6,0 };
const byte dots[][7] = {
    { 1,2,3,4,5,6,0 }, /*0*/ { 2,3,0,0,0,0,0 }, /*1*/ { 1,2,4,5,7,0,0 }, /*2*/
    { 1,2,3,4,7,0,0 }, /*3*/ { 2,3,6,7,0,0,0 }, /*4*/ { 1,3,4,6,7,0,0 }, /*5*/
    { 1,3,4,5,6,7,0 }, /*6*/ { 1,2,3,6,0,0,0 }, /*7*/ { 1,2,3,4,5,6,7 }, /*8*/
    { 1,2,3,4,6,7,0 }  /*9*/
};

// Settings Variable
boolean FanOn = false, SoundOn = false;
byte alarmhour, alarmminute, alarmmusicselect, fandelay;

DS1302RTC RTC(CEPin, IOPin, CLKPin); // Initialize RTC
tmElements_t tm;
boolean doton, houron, minuteon, dateon, monthon, yearon, fanison;
int digit[4], digiti, doti;
byte prevalarmhour,prevalarmminute,lastSec;
long lastmilli,nextUseCnt;
int Mode,TimeSetMode,AlarmSetMode,ShowDateCnt,notedelaycountdown,notei,fandelaycountdown;

void ResetALL(){
    tm.Year = CalendarYrToTm(2000);
    tm.Hour = tm.Minute = tm.Second = 0;
    tm.Day = tm.Month = 1;
    RTC.write(tm);
    EEPROM.write(FanDelay_EEPROM, 5);
    EEPROM.write(FanOn_EEPROM, 0);
    EEPROM.write(SoundOn_EEPROM, 0);
    EEPROM.write(AlarmMusicSelect_EEPROM, 0);
    EEPROM.write(AlarmHour_EEPROM, 0);
    EEPROM.write(AlarmMinute_EEPROM, 0);
}
void LoopEndConfiguration(){
    if(!yearon){
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH);
        return;
    }
    if(digiti==0 && (!minuteon || !dateon))digiti=2;
    if(digiti==2 && (!houron || !monthon))digiti=0;
    DisplayDot(digiti, doti);
    doti++;
    if(doti>=dotsn[digit[digiti]]){
        doti=0;
        digiti++;
    }
    if(digiti==2 && doti+1==dotsn[digit[2]] && doton && Mode == NormalDisplayMode){
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 1);
        digitalWrite(latchPin, HIGH);
    }
    if(digiti==3 && doti+1==dotsn[digit[3]] && Mode == SetTimeMode){
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 1);
        digitalWrite(latchPin, HIGH);
    }
    if(digiti==2 && doti+1==dotsn[digit[2]] && Mode == SetAlarmMode){
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 1);
        digitalWrite(latchPin, HIGH);
    }
    if(digiti==1 && doti+1==dotsn[digit[1]] && Mode == SetAlarmMusicMode){
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 1);
        digitalWrite(latchPin, HIGH);
    }
    if(digiti==0 && doti+1==dotsn[digit[0]] && Mode == SetFanDelayMode){
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, 1);
        digitalWrite(latchPin, HIGH);
    }
    if(digiti==4)digiti=0;
}
void ShowTime(){
    digit[0] = tm.Minute%10, digit[1] = tm.Minute/10;
    digit[2] = tm.Hour%10, digit[3] = tm.Hour/10;
}
void ShowDate(){
    digit[0] = tm.Day%10, digit[1] = tm.Day/10;
    digit[2] = tm.Month%10, digit[3] = tm.Month/10;
}
void ShowYear(){
    int realYear = int(tm.Year)+1970;
    digit[0] = realYear%10, digit[1] = (realYear/10)%10;
    digit[2] = (realYear/100)%10, digit[3] = realYear/1000;
}
void ShowAlarm(){
    digit[0] = alarmminute%10, digit[1] = alarmminute/10;
    digit[2] = alarmhour%10, digit[3] = alarmhour/10;
}
void ShowMusicSelect(){ digit[0] = alarmmusicselect%10, digit[1] = alarmmusicselect/10; }
void ShowFanDelay(){ digit[0] = fandelay%10, digit[1] = fandelay/10; }

void BlowTheFan(){
    if(fanison==true)digitalWrite(FanPin, HIGH);
    else digitalWrite(FanPin, LOW);
    fandelaycountdown--;
    if(fandelaycountdown<=0){
        if(fanison==true){
            fanison=false;
            if(fandelay<2)fandelaycountdown=1;
            else{
                fandelaycountdown = (int)fandelay*100+1000;
            }
        }else{
            fanison=true;
            fandelaycountdown=1500;
        }
    }
}
void RingTheBell(){
    notedelaycountdown--;
    if(!notedelaycountdown){
        noTone(BeeperPin);
        notei++;
        if(notei==melodyn[alarmmusicselect])notei=0;
        int noteDuration = 800 / pgm_read_float_near(noteDurations[alarmmusicselect] + notei);
        tone(BeeperPin, pgm_read_word_near(melody[alarmmusicselect] + notei), noteDuration);
        notedelaycountdown = noteDuration * 1.3;
    }
}
void setup() {
    pinMode(BeeperPin, OUTPUT);
    
    Mode = NormalDisplayMode;
    minuteon = houron = dateon = monthon = yearon = true;
    
    digiti=doti=0;
    Serial.begin(9600);

    // Defines FanPin Mode
    pinMode(FanPin, OUTPUT);
    
    // Makes Yellow Light Off
    pinMode(SoundYellowLightPin, OUTPUT);
    digitalWrite(SoundYellowLightPin, LOW);
    
    // Makes Red Light Off
    pinMode(FanRedLightPin, OUTPUT);
    digitalWrite(FanRedLightPin, LOW);
    
    // Defines OrangeButtonPin Mode
    pinMode(OrangeButtonPin, INPUT);
    
    // 將 latchPin, clockPin, dataPin 設置為輸出
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    
    pinMode(HR_ten_Pin, OUTPUT);
    pinMode(HR_one_Pin, OUTPUT);
    pinMode(MIN_ten_Pin, OUTPUT);
    pinMode(MIN_one_Pin, OUTPUT);
    
    lastmilli = millis();

    RTC.haltRTC(false);
    RTC.read(tm); //ReadRTC DATA
    lastSec = tm.Second;
    
    //ReadEEPROM DATA Setting start
    FanOn = EEPROM.read(FanOn_EEPROM);
    SoundOn = EEPROM.read(SoundOn_EEPROM);
    alarmhour = EEPROM.read(AlarmHour_EEPROM);
    alarmminute = EEPROM.read(AlarmMinute_EEPROM);
    alarmmusicselect = EEPROM.read(AlarmMusicSelect_EEPROM);
    fandelay = EEPROM.read(FanDelay_EEPROM);
    //ReadEEPROM DATA Setting end
}

void loop() {
    ////////// Reading Section Start //////////

    switch(Mode){
        case NormalDisplayMode:
        // Orange Button () Read
            if ( analogRead(OrangeButtonPin) > 500 && nextUseCnt == 0 ) {
                nextUseCnt = 400;
                if ( FanOn == false && SoundOn == false ) {
                    FanOn = true;
                    EEPROM.write(FanOn_EEPROM, 1);
                } else if ( FanOn == true && SoundOn == false ) {
                    FanOn = false;
                    SoundOn = true;
                    EEPROM.write(FanOn_EEPROM, 0);
                    EEPROM.write(SoundOn_EEPROM, 1);
                } else if ( FanOn == false && SoundOn == true ) {
                    FanOn = true;
                    EEPROM.write(FanOn_EEPROM, 1);
                } else {
                    FanOn = false;
                    SoundOn = false;
                    EEPROM.write(FanOn_EEPROM, 0);
                    EEPROM.write(SoundOn_EEPROM, 0);
                }
            }
        // Green Button () Read
            ShowDateCnt=0;
            while( analogRead(GreenButtonPin) > 500 ){
                doton = false;
                if ( analogRead(BlueButtonPin) > 500 && analogRead(OrangeButtonPin) > 500 ){
                    ResetALL(); // 2000/01/01 SUN 00:00:00
                    return;
                }
                if(ShowDateCnt>4000)ShowDateCnt=0;
                else if(ShowDateCnt>2000)ShowYear();
                else ShowDate();
                LoopEndConfiguration();
                digitalWrite(SoundYellowLightPin, SoundOn ? HIGH : LOW);
                digitalWrite(FanRedLightPin, FanOn ? HIGH : LOW);
                ShowDateCnt++;
            }
        // Blue Button () Read
            if ( analogRead(BlueButtonPin) > 500 && nextUseCnt == 0 ) {
                Mode = SetTimeMode;
                TimeSetMode = SetMinute;
                nextUseCnt = 400;
            }

            if( millis() > lastmilli + 998 ){
                RTC.read(tm);
                if(tm.Second&1)doton = false;
                else doton = true;
                if(lastSec != tm.Second){
					Serial.print(tm.Year);
					Serial.print("/");
					Serial.print(tm.Month);
					Serial.print("/");
					Serial.print(tm.Day);
					Serial.print(" ");
					Serial.print(tm.Hour);
					Serial.print(":");
					Serial.print(tm.Minute);
					Serial.print(":");
					Serial.println(tm.Second);
                    lastmilli = millis();
                    lastSec = tm.Second;
                    if(tm.Minute == alarmminute && tm.Hour == alarmhour){
                        if(FanOn || SoundOn){
                            Mode = RingingMode;
                        }
                    }
                    ShowTime();
                }
            }
            break;
        case SetTimeMode:
        // Orange Button () Read
            if ( analogRead(OrangeButtonPin) > 500 && nextUseCnt == 0 ) {
                lastmilli = millis();
                minuteon = houron = dateon = monthon = yearon = true;
                nextUseCnt = 300;
                switch(TimeSetMode){
                    case SetMinute:
                        tm.Minute++;
                        tm.Second=0;
                        if(tm.Minute==60)tm.Minute=0;
                        break;
                    case SetHour:
                        tm.Hour++;
                        if(tm.Hour==24)tm.Hour=0;
                        break;
                    case SetDate:
                        tm.Day++;
                        if(tm.Day>MonthMaxDay[tm.Month])tm.Day=1;
                        break;
                    case SetMonth:
                        tm.Month++;
                        if(tm.Month==13)tm.Month=1;
                        if(tm.Day>MonthMaxDay[tm.Month])tm.Day=1;
                        break;
                    case SetYear:
                        tm.Year++;
                        if(int(tm.Year)+1970==2100)tm.Year=30;
                        break;
                }
                RTC.write(tm);
                switch(TimeSetMode){
                    case SetMinute:
                        ShowTime();
                        break;
                    case SetHour:
                        ShowTime();
                        break;
                    case SetDate:
                        ShowDate();
                        break;
                    case SetMonth:
                        ShowDate();
                        break;
                    case SetYear:
                        ShowYear();
                        break;
                }
                //MonthMaxDay
            }
        // Green Button () Read
            if ( analogRead(GreenButtonPin) > 500 && nextUseCnt == 0 ) {
                lastmilli = millis();
                minuteon = houron = dateon = monthon = yearon = true;
                nextUseCnt = 400;
                switch(TimeSetMode){
                    case SetMinute:
                        TimeSetMode = SetHour;
                        break;
                    case SetHour:
                        TimeSetMode = SetDate;
                        break;
                    case SetDate:
                        TimeSetMode = SetMonth;
                        break;
                    case SetMonth:
                        TimeSetMode = SetYear;
                        break;
                    case SetYear:
                        TimeSetMode = SetMinute;
                        break;
                }
                switch(TimeSetMode){
                    case SetMinute:
                        ShowTime();
                        break;
                    case SetHour:
                        ShowTime();
                        break;
                    case SetDate:
                        ShowDate();
                        break;
                    case SetMonth:
                        ShowDate();
                        break;
                    case SetYear:
                        ShowYear();
                        break;
                }
            }
        // Blue Button () Read
            if ( analogRead(BlueButtonPin) > 500 && nextUseCnt == 0 ) {
                AlarmSetMode = AlarmSetMinute;
                prevalarmhour = alarmhour;
                prevalarmminute = alarmminute;
                Mode = SetAlarmMode;
                ShowAlarm();
                nextUseCnt = 400;
                break;
            }

            if( millis() > lastmilli + 998 ){
                RTC.read(tm);
                minuteon = houron = dateon = monthon = yearon = true;
                switch(TimeSetMode){
                    case SetMinute:
                        if(tm.Second&1)minuteon = false;
                        break;
                    case SetHour:
                        if(tm.Second&1)houron = false;
                        break;
                    case SetDate:
                        if(tm.Second&1)dateon = false;
                        break;
                    case SetMonth:
                        if(tm.Second&1)monthon = false;
                        break;
                    case SetYear:
                        if(tm.Second&1)yearon = false;
                        break;
                }
                if(lastSec != tm.Second){
                    lastmilli = millis();
                    lastSec = tm.Second;
                    switch(TimeSetMode){
                        case SetMinute:
                            ShowTime();
                            break;
                        case SetHour:
                            ShowTime();
                            break;
                        case SetDate:
                            ShowDate();
                            break;
                        case SetMonth:
                            ShowDate();
                            break;
                        case SetYear:
                            ShowYear();
                            break;
                    }
                }
            }
            break;
        case SetAlarmMode:
        // Orange Button () Read
            if ( analogRead(OrangeButtonPin) > 500 && nextUseCnt == 0 ) {
                minuteon = houron = dateon = monthon = yearon = true;
                nextUseCnt = 300;
                switch(AlarmSetMode){
                    case AlarmSetMinute:
                        alarmminute++;
                        if(alarmminute==60)alarmminute=0;
                        break;
                    case AlarmSetHour:
                        alarmhour++;
                        if(alarmhour==24)alarmhour=0;
                        break;
                }
                ShowAlarm();
            }
        // Green Button () Read
            if ( analogRead(GreenButtonPin) > 500 && nextUseCnt == 0 ) {
                minuteon = houron = dateon = monthon = yearon = true;
                nextUseCnt = 400;
                switch(AlarmSetMode){
                    case AlarmSetMinute:
                        AlarmSetMode = AlarmSetHour;
                        break;
                    case AlarmSetHour:
                        AlarmSetMode = AlarmSetMinute;
                        break;
                }
                ShowAlarm();
            }
        // Blue Button () Read
            if ( analogRead(BlueButtonPin) > 500 && nextUseCnt == 0 ) {
                if(prevalarmhour != alarmhour || prevalarmminute != alarmminute){
                    EEPROM.write(AlarmHour_EEPROM, alarmhour);
                    EEPROM.write(AlarmMinute_EEPROM, alarmminute);
                }
                ShowMusicSelect();
                minuteon = dateon = yearon = true;
                houron = monthon = false;
                notei = -1;
                notedelaycountdown = 1;
                Mode = SetAlarmMusicMode;
                nextUseCnt = 400;
            }
            
            if( millis() > lastmilli + 998 ){
                RTC.read(tm);
                minuteon = houron = dateon = monthon = yearon = true;
                switch(AlarmSetMode){
                    case AlarmSetMinute:
                        if(tm.Second&1)minuteon = false;
                        break;
                    case AlarmSetHour:
                        if(tm.Second&1)houron = false;
                        break;
                }
                if(lastSec != tm.Second){
                    lastmilli = millis();
                    lastSec = tm.Second;
                }
            }
            break;
//const int AlarmMusicSelect_EEPROM = 24; //  <----  AlarmMusicSetMode

        case SetAlarmMusicMode:
        // Orange Button () Read
            if ( analogRead(OrangeButtonPin) > 500 && nextUseCnt == 0 ) {
                alarmmusicselect++;
                if(alarmmusicselect==MUSICFILENUM)alarmmusicselect=0;
                ShowMusicSelect();
                notei = -1;
                notedelaycountdown = 1;
                nextUseCnt = 400;
            }
        // Green Button () Read
            if ( analogRead(GreenButtonPin) > 500 && nextUseCnt == 0 ) {
                if(alarmmusicselect==0)alarmmusicselect=MUSICFILENUM-1;
                else alarmmusicselect--;
                ShowMusicSelect();
                notei = -1;
                notedelaycountdown = 1;
                nextUseCnt = 400;
            }
        // Blue Button () Read
            if ( analogRead(BlueButtonPin) > 500 && nextUseCnt == 0 ) {
                EEPROM.write(AlarmMusicSelect_EEPROM, alarmmusicselect);
                fanison = true;
                ShowFanDelay();
                minuteon = dateon = yearon = true;
                houron = monthon = false;
                Mode = SetFanDelayMode;
                nextUseCnt = 400;
            }
            
            LoopEndConfiguration();
            RingTheBell();
            
            break;
        case SetFanDelayMode:
        // Orange Button () Read
            if ( analogRead(OrangeButtonPin) > 500 && nextUseCnt == 0 ) {
                fandelay++;
                if(fandelay > MAXFANDELAY)fandelay=MINFANDELAY;
                ShowFanDelay();
                nextUseCnt = 100;
            }
        // Green Button () Read
            if ( analogRead(GreenButtonPin) > 500 && nextUseCnt == 0 ) {
                fandelay--;
                if(fandelay < MINFANDELAY)fandelay=MAXFANDELAY;
                ShowFanDelay();
                nextUseCnt = 100;
                
            }
        // Blue Button () Read
            if ( analogRead(BlueButtonPin) > 500 && nextUseCnt == 0 ) {
                EEPROM.write(FanDelay_EEPROM, fandelay);
                digitalWrite(FanPin, LOW);
                minuteon = houron = dateon = monthon = yearon = true;
                Mode = NormalDisplayMode;
                nextUseCnt = 2000;
                break;
            }

            BlowTheFan();
            
            break;
        case RingingMode:
            if ( analogRead(BlueButtonPin) > 500 && nextUseCnt == 0 ) {
                digitalWrite(FanPin, LOW);
                FanOn = SoundOn = false;
                minuteon = houron = dateon = monthon = yearon = true;
                Mode = NormalDisplayMode;
                nextUseCnt = 2000;
                break;
            }
        
            LoopEndConfiguration();
            if(FanOn)BlowTheFan();
            if(SoundOn)RingTheBell();
            
            break;
    }
    
    ////////// Reading Section End //////////

    ////////// Running Section Start //////////
    digitalWrite(SoundYellowLightPin, SoundOn ? HIGH : LOW);
    digitalWrite(FanRedLightPin, FanOn ? HIGH : LOW);
    if( nextUseCnt > 0 )nextUseCnt--;
    LoopEndConfiguration();
    ////////// Running Section End //////////
}

void DisplayDot(int digitval, int dotval){
    int i;
    byte bitsToSend;
    digitalWrite(latchPin, LOW);
    bitWrite(bitsToSend, dots[digit[digitval]][dotval], HIGH);
    shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
    
    for(i=0;i<4;i++){
        digitalWrite(digit_Pin[i], HIGH);
    }
    digitalWrite(digit_Pin[digitval], LOW);

    digitalWrite(latchPin, HIGH);
}