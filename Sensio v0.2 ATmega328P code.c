#include <avr/io.h>
#include <avr/sleep.h>
#include <four_digit_seven_segment_74HC595N.h> //int digit[4], hasdot[4]
#include <notes_output_using_timer1.h>	// void set_f(int _frequency);
// void init_notes(char __NOTE_CTRL_PIN_IN, int __NOTE_ID_PIN_IN);
#include <timer0_OC0B(PD5)_fast_pwm_analog_output.h>	// void InitPWM();
// void SetPWMOutput(int duty);
#include <timer2_asynchronous_rtc.h>	// void RTCInit(int all_time_init);
// uint8_t __SEC, __MIN, __HR, __DATE, __MONTH, __YEAR, __LEAPYEAR;


/////////// control of Fan Music On Off Light ///////////
#define LIGHT_CONTROL		DDRC
#define LIGHT_PORT			PORTC
#define FAN_LIGHT_PORT		PC3
#define MUSIC_LIGHT_PORT	PC4
void change_light_switch();
void lights_init();
/////////// control of Fan Music On Off Light ///////////

/////////// control of Button ///////////
#define COUNT_DOWN_DELAY	100
#define BUTTON_CONTROL		DDRD
#define BUTTON_PIN			PIND
#define TOP_PIN				PD0
#define BOTTOM_RIGHT_PIN	PD4
#define BOTTOM_LEFT_PIN		PD3
void buttons_init();
int press(int pinid, int initcd);
int cd; // countdown within loop (prevents over frequent sensing)
/////////// control of Button ///////////

#define NORMAL_MODE				1
#define TIME_SET_MODE			2
#define ALARM_SET_MODE			3
#define MUSIC_SELECT_MODE		4
#define FAN_DELAY_TIME_SET_MODE 5
#define ALARM_BEEP_MODE			6
void init_sensio(); // initializes the whole program
int mode;

/////////// Normal_Mode ///////////
int fan_on = 0, music_on = 0, fan_music_presstwice; // to be modified
int show_date_year_mode, show_press_fourth;
void init_normal();
void normal_mode();
/////////// Normal_Mode ///////////

/////////// Time_Set_Mode ///////////
#define TIME_COUNT_DOWN_DELAY	50
int time_cnt, time_set_setting, show_num;
int type_change_press_fourth;
int day_of_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
void init_time_set();
void time_set_mode();
/////////// Time_Set_Mode ///////////

/////////// Alarm_Set_Mode ///////////
uint8_t alarmhr,alarmmin;
void init_alarm_set();
void alarm_set_mode();
/////////// Alarm_Set_Mode ///////////

/////////// Music_Select_Mode ///////////
#define MUSIC_PORT_CHAR	'B'
#define MUSIC_PORT		PB5
void init_music_select();
void music_select_mode();
/////////// Music_Select_Mode ///////////

/////////// Fan_Delay_Time_Set_Mode ///////////
#define FANSPEED_INTERVAL	50
int fanspeed;
void init_fan_delay_time_set();
void fan_delay_time_set_mode();
/////////// Fan_Delay_Time_Set_Mode ///////////

/////////// Alarm Ring Mode ///////////
void init_alarm_beep();
void alarm_beep_mode();
/////////// Alarm Ring Mode ///////////

/////////// Sleep Mode ///////////
#define CHECK_EXT_POWER_PIN_CHAR 'D'
#define CHECK_EXT_POWER_PIN PD2
/////////// Sleep Mode ///////////

int main(void){
	init_sensio();
	while(1){
		if(!(_SFR_IO8(__EXT_POWER_PIN_CTRL) & 1<<__EXT_POWER_PIN)){ // Check if Ext power is disconnected
			PORTB |= 0b00011110;
			LIGHT_PORT &= ~(1<<FAN_LIGHT_PORT);
			LIGHT_PORT &= ~(1<<MUSIC_LIGHT_PORT);
			ext_check = 1, ext_exist = 0;
			while(!ext_exist){
				SMCR|=(1<<SE);
				asm volatile("sleep"::);
				SMCR&=~(1<<SE);
			}
			change_light_switch();
		}
		
		switch(mode){
			case NORMAL_MODE:
			normal_mode();
			break;
			case TIME_SET_MODE:
			time_set_mode();
			break;
			case ALARM_SET_MODE:
			alarm_set_mode();
			break;
			case MUSIC_SELECT_MODE:
			music_select_mode();
			break;
			case FAN_DELAY_TIME_SET_MODE:
			fan_delay_time_set_mode();
			break;
			case ALARM_BEEP_MODE:
			alarm_beep_mode();
			break;
		}
		disp();
		if(cd>0)cd--;
	}  // end while
}	// end main

void init_normal(){
	mode = NORMAL_MODE;
	show_date_year_mode = fan_music_presstwice = 0;
	show_press_fourth = 3;
	digit[0] = digit[1] = digit[2] = digit[3] = 0;
}
void normal_mode(){
	if(alarmhr == __HR && alarmmin == __MIN && fan_on){
		digit[0] = digit[1] = digit[2] = digit[3] = 5;
		fan_on = 0;
		init_alarm_beep();
		change_light_switch();
		return;
	}
	
	if(TIMER2_OVF&&!press(BOTTOM_LEFT_PIN, 0)){
		hasdot[1] = __SEC&1;
		digit[0] = __HR/10;
		digit[1] = __HR%10;
		digit[2] = __MIN/10;
		digit[3] = __MIN%10;
		TIMER2_OVF = 0;
	}
	
	if(press(TOP_PIN, 1)){
		init_time_set(); // to time set mode
		hasdot[1] = 0;
		}else if(press(BOTTOM_LEFT_PIN, 1)){ // show date
		hasdot[1] = 0;
		if( show_press_fourth == 4-1 ){
			if(show_date_year_mode){
				digit[0] = __YEAR/1000;
				digit[1] = (__YEAR/100)%10;
				digit[2] = (__YEAR/10)%10;
				digit[3] = __YEAR%10;
				}else{
				digit[0] = __MONTH/10;
				digit[1] = __MONTH%10;
				digit[2] = __DATE/10;
				digit[3] = __DATE%10;
			}
			disp();
			show_date_year_mode = !show_date_year_mode;
			show_press_fourth = 0;
			}else{
			show_press_fourth++;
		}
		return;
		}else if(press(BOTTOM_RIGHT_PIN, 1)){ // set fan music on off
		if(fan_music_presstwice){
			// fm : 00 -> 01 -> 10 -> 11
			if( fan_on && music_on ){
				fan_on = music_on = 0; // 11 -> 00
				}else if( fan_on ){
				music_on = 1; // 10 -> 11
				}else if(music_on){
				fan_on = 1, music_on = 0; // 01 -> 10
				}else{
				music_on = 1; // 00 -> 01
			}
			change_light_switch();
			fan_music_presstwice = 0;
		}else fan_music_presstwice++;
		}else if(!press(BOTTOM_LEFT_PIN, 0)){
		show_press_fourth = 3;
	}
}

void init_time_set(){
	mode = TIME_SET_MODE;
	time_cnt = 0;
	show_num = 1;
	type_change_press_fourth = 1;
	time_set_setting = RTC_MIN;
	hasdot[0] = 1;
	digit[3]=__MIN%10 ,digit[2]=__MIN/10 ,digit[1]=__HR%10, digit[0]=__HR/10;
}
void time_set_mode(){
	if(press(TOP_PIN, 1)){ // to alarm set mode
		init_alarm_set();
		hasdot[0] = 0;
		}else if(press(BOTTOM_LEFT_PIN, 0)){ // change sec -> min -> hr -> date -> month -> year
		if(!time_cnt){
			time_cnt = TIME_COUNT_DOWN_DELAY;
			if(type_change_press_fourth == 1){
				switch(time_set_setting){
					case RTC_MIN:
					digit[3]=__MIN%10 ,digit[2]=__MIN/10;
					time_set_setting=RTC_HR;
					break;
					case RTC_HR:
					digit[3]=__DATE%10 ,digit[2]=__DATE/10;
					digit[1]=__MONTH%10, digit[0]=__MONTH/10;
					time_set_setting=RTC_DATE;
					break;
					case RTC_DATE:
					digit[3]=__DATE%10, digit[2]=__DATE/10;
					time_set_setting=RTC_MONTH;
					break;
					case RTC_MONTH:
					digit[0] = __YEAR/1000, digit[1] = (__YEAR/100)%10, digit[2] = (__YEAR/10)%10, digit[3] = __YEAR%10;
					time_set_setting=RTC_YEAR;
					break;
					case RTC_YEAR:
					digit[3]=__MIN%10 ,digit[2]=__MIN/10;
					digit[1]=__HR%10, digit[0]=__HR/10;
					time_set_setting=RTC_MIN;
					break;
				}
				type_change_press_fourth = 0;
			}else type_change_press_fourth++;
		}
		}else if(press(BOTTOM_RIGHT_PIN, 0)){ // +1
		if(!time_cnt){
			switch(time_set_setting){
				case RTC_MIN:
				__MIN = (__MIN+1)%60;
				digit[3]=__MIN%10 ,digit[2]=__MIN/10;
				__SEC = 0;
				break;
				case RTC_HR:
				__HR = (__HR+1)%24;
				digit[1]=__HR%10, digit[0]=__HR/10;
				break;
				case RTC_DATE:
				if(__MONTH==2 && !((__YEAR>>2)&1)) __DATE=__DATE%29+1;
				else __DATE=__DATE%__DAY_OF_MONTH[__MONTH]+1;
				digit[3]=__DATE%10, digit[2]=__DATE/10;
				break;
				case RTC_MONTH:
				digit[1]=__MONTH%10, digit[0]=__MONTH/10;
				__MONTH=__MONTH%12+1;
				break;
				case RTC_YEAR:
				__YEAR=__YEAR+1;
				if(__YEAR==2200)__YEAR=2000;
				digit[0] = __YEAR/1000;
				digit[1] = (__YEAR/100)%10;
				digit[2] = (__YEAR/10)%10;
				digit[3] = __YEAR%10;
				break;
			}
			time_cnt = TIME_COUNT_DOWN_DELAY;
		}
	}
	if(time_cnt)time_cnt--;
	if(TIMER2_OVF){
		show_num=!show_num;
		if(press(BOTTOM_RIGHT_PIN, 0))show_num=1;
		switch(time_set_setting){
			case RTC_MIN:
			if(show_num){
				digit[3]=__MIN%10 ,digit[2]=__MIN/10;
			}else digit[3]=digit[2]=10;
			break;
			case RTC_HR:
			if(show_num){
				digit[1]=__HR%10, digit[0]=__HR/10;
			}else digit[1]=digit[0]=10;
			break;
			case RTC_DATE:
			if(show_num){
				digit[3]=__DATE%10, digit[2]=__DATE/10;
			}else digit[3]=digit[2]=10;
			break;
			case RTC_MONTH:
			if(show_num){
				digit[1]=__MONTH%10, digit[0]=__MONTH/10;
			}else digit[1]=digit[0]=10;
			break;
			case RTC_YEAR:
			if(show_num){
				digit[0] = __YEAR/1000;
				digit[1] = (__YEAR/100)%10;
				digit[2] = (__YEAR/10)%10;
				digit[3] = __YEAR%10;
			}else digit[0]=digit[1]=digit[2]=digit[3]=10;
			break;
		}
		TIMER2_OVF = 0;
	}
}

void init_alarm_set(){
	mode = ALARM_SET_MODE;
	time_set_setting = RTC_MIN;
	time_cnt = 0;
	show_num = 1;
	type_change_press_fourth = 1;
	digit[3]=alarmmin%10 ,digit[2]=alarmmin/10 ,digit[1]=alarmhr%10, digit[0]=alarmhr/10;
	hasdot[1] = 1;
}
void alarm_set_mode(){
	disp();
	if(press(TOP_PIN, 1)){ // to music select mode
		init_music_select();
		hasdot[1] = 0;
		}else if(press(BOTTOM_LEFT_PIN, 0)){ // change min -> hr
		if(!time_cnt){
			time_cnt = TIME_COUNT_DOWN_DELAY;
			if(type_change_press_fourth == 1){
				time_set_setting = RTC_MIN + RTC_HR - time_set_setting;
				digit[0]=alarmhr/10 ,digit[1]=alarmhr%10 ,digit[2]=alarmmin/10, digit[3]=alarmmin%10;
				type_change_press_fourth = 0;
			}else type_change_press_fourth++;
		}
		}else if(press(BOTTOM_RIGHT_PIN, 0)){ // +1
		if(!time_cnt){
			if( time_set_setting == RTC_MIN ){
				alarmmin = (alarmmin+1)%60;
				digit[3]=alarmmin%10 ,digit[2]=alarmmin/10;
				//__EEPUT(ALARM_HR_BYTE, alarmhr);
				}else{
				alarmhr = (alarmhr+1)%24;
				digit[1]=alarmhr%10, digit[0]=alarmhr/10;
				//__EEPUT(ALARM_MIN_BYTE, alarmmin);
			}
			time_cnt = TIME_COUNT_DOWN_DELAY / 2;
		}
	}
	if(time_cnt)time_cnt--;
	if(TIMER2_OVF){
		show_num=!show_num;
		if(press(BOTTOM_RIGHT_PIN, 0))show_num=1;
		switch(time_set_setting){
			case RTC_MIN:
			if(show_num){
				digit[3]=alarmmin%10 ,digit[2]=alarmmin/10;
			}else digit[3]=digit[2]=10;
			break;
			case RTC_HR:
			if(show_num){
				digit[1]=alarmhr%10, digit[0]=alarmhr/10;
			}else digit[1]=digit[0]=10;
			break;
		}
		TIMER2_OVF = 0;
	}
}
void init_music_select(){
	time_cnt = 0;
	mode = MUSIC_SELECT_MODE;
	init_notes(MUSIC_PORT_CHAR, MUSIC_PORT);
	set_f(middle_freq);
	digit[0]=7 ,digit[1]=7 ,digit[2]=7, digit[3]=7;
	hasdot[2] = 1;
}
void music_select_mode(){
	disp();
	if(press(TOP_PIN, 1)){ // to fan delay time set mode
		pause_notes();
		init_fan_delay_time_set();
		hasdot[2] = 0;
		}else if(press(BOTTOM_LEFT_PIN, 0)){ // change music select - 1
		if(!time_cnt){
			time_cnt = TIME_COUNT_DOWN_DELAY;
			pause_notes();
		}
		}else if(press(BOTTOM_RIGHT_PIN, 0)){ // change music select + 1
		if(!time_cnt){
			time_cnt = TIME_COUNT_DOWN_DELAY;
			continue_notes();
		}
	}
	if(time_cnt)time_cnt--;
}
void init_fan_delay_time_set(){
	time_cnt = 0;
	mode = FAN_DELAY_TIME_SET_MODE;
	hasdot[3] = 1;
	digit[0]=10 ,digit[1]=10 ,digit[2]=fanspeed/10, digit[3]=fanspeed%10;
	InitPWM();
	SetPWMOutput(fanspeed*255/FANSPEED_INTERVAL);
}
void fan_delay_time_set_mode(){
	disp();
	if(press(TOP_PIN, 1)){ // to fan delay time set mode
		StopPWM();
		init_normal();
		hasdot[3] = 0;
		}else if(press(BOTTOM_LEFT_PIN, 0)){ // change fan speed - 1
		if(!time_cnt){
			if(fanspeed>0)fanspeed--;
			else fanspeed = 0;
			digit[2]=fanspeed/10, digit[3]=fanspeed%10;
			SetPWMOutput(fanspeed*255/FANSPEED_INTERVAL);
			time_cnt = TIME_COUNT_DOWN_DELAY / 3;
		}
		}else if(press(BOTTOM_RIGHT_PIN, 0)){ // change fan speed + 1
		if(!time_cnt){
			if(fanspeed<FANSPEED_INTERVAL)fanspeed++;
			digit[2]=fanspeed/10, digit[3]=fanspeed%10;
			SetPWMOutput(fanspeed*255/FANSPEED_INTERVAL);
			time_cnt = TIME_COUNT_DOWN_DELAY / 3;
		}
	}
	if(time_cnt)time_cnt--;
}
void init_alarm_beep(){
	hasdot[1] = 0;
	mode = ALARM_BEEP_MODE;
	InitPWM();
	SetPWMOutput(fanspeed*255/FANSPEED_INTERVAL);
}
void alarm_beep_mode(){
	disp();
	if(press(TOP_PIN, 1)||press(BOTTOM_RIGHT_PIN, 1)||press(BOTTOM_LEFT_PIN, 1)){ // close alarm and return to normal mode
		StopPWM();
		mode = NORMAL_MODE;
	}
}

void init_sensio(){
	lights_init();
	buttons_init();
	
	// important! Breadboard different from PCB
	
	if(press(TOP_PIN, 0))init_4digit_7segment( 0, 1, 6, 'B', 'D', 'D', 1, 2, 3, 4, 'B', 'B', 'B', 'B' ); // Breadboard
	else init_4digit_7segment( 0, 1, 6, 'B', 'D', 'D', 4, 3, 2, 1, 'B', 'B', 'B', 'B' ); // PCB
	
	RTCInit(1);
	sei();
	init_ext_power(CHECK_EXT_POWER_PIN_CHAR, CHECK_EXT_POWER_PIN);
	// Choose our preferred sleep mode:
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	
	cd = fan_on = music_on = 0;
	alarmhr = alarmmin = 0;
	change_light_switch();
	
	fanspeed = FANSPEED_INTERVAL / 2;
	
	init_normal();
	StopPWM();
}

void buttons_init(){ BUTTON_CONTROL &= ~((1<<TOP_PIN)|(1<<BOTTOM_RIGHT_PIN)|(1<<BOTTOM_LEFT_PIN)); }
void lights_init(){ LIGHT_CONTROL |= (1<<FAN_LIGHT_PORT) | (1<<MUSIC_LIGHT_PORT); }

int press(int pinid, int initcd){
	int pressed = !(BUTTON_PIN & (1<<pinid));
	if(initcd) pressed = !cd && pressed;
	if(pressed&&initcd) cd = COUNT_DOWN_DELAY;
	return pressed;
}
void change_light_switch(){
	if(fan_on)LIGHT_PORT |= 1<<FAN_LIGHT_PORT;
	else LIGHT_PORT &= ~(1<<FAN_LIGHT_PORT);
	if(music_on)LIGHT_PORT |= 1<<MUSIC_LIGHT_PORT;
	else LIGHT_PORT &= ~(1<<MUSIC_LIGHT_PORT);
}