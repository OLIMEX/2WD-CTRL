#include <Arduino.h>
#include <avr/io.h>
#include <NewPing.h>

#include "OLIMEX_2WD.h"


/****************************************************************************/
/*                                 Automatic Strategy                       */
/****************************************************************************/

Automatic::Automatic() {
}

STRATEGY_type Automatic::getType() {
	return AUTOMATIC;
}

const char* Automatic::getName() {
	return "Automatic";
}

int Automatic::calcSpeed(int distance) {

	int speed=0;
	if (distance > MAX_DIST || distance == 0) {
			distance = MAX_DIST;
		} else if (distance < MIN_DIST) {
			distance = MIN_DIST;
		}
	
	if (distance >= REV_DIST && distance <= STOP_DIST) {
			speed = 0;
		} else {
			speed = (distance - REV_DIST);
			if (speed > 0) {
				speed += MIN_SPEED;
			} else {
				speed -= MIN_SPEED;
			}
		}
	
	return speed;
}

int Automatic::updateSpeed(CARSTATE& state) {
	int l_speed=0;
	int r_speed=0;
	int delay = 0;

	if (abs(state.l_distance - state.r_distance) > REV_DIST) {
		delta_distance = state.l_distance - state.r_distance;
	}

	l_speed = calcSpeed(state.r_distance);
	r_speed = calcSpeed(state.l_distance);
	
	if (l_speed == 0 && r_speed == 0) {
		if (delta_distance > 0) {
			l_speed = -MIN_SPEED;
			r_speed = MIN_SPEED;
		} else {
			l_speed = MIN_SPEED;
			r_speed = -MIN_SPEED;
		}
		delay = 500;
	}
	
	state.l_speed = l_speed == 0 ?
		l_speed
		: 
		state.l_speed + (l_speed - state.l_speed) * 0.7
	;
	
	state.r_speed = r_speed == 0 ?
		r_speed
		:
		state.r_speed + (r_speed - state.r_speed) * 0.7
	;

	return delay;
}

/****************************************************************************/
/*                                Remote Strategy                           */
/****************************************************************************/

Remote::Remote() : overall_speed(0) {
}

void Remote::incSpeed(float& speed) {
	if (speed + SPEED_STEP < MAX_SPEED - MIN_SPEED ) {
		speed += SPEED_STEP;
	}
}

void Remote::decSpeed(float& speed) {
	if (speed - SPEED_STEP > -MAX_SPEED + MIN_SPEED) {
		speed -= SPEED_STEP;
	}
}

STRATEGY_type Remote::getType() {
	return REMOTE;
}

const char* Remote::getName() {
	return "Remote";
}

int Remote::updateSpeed(CARSTATE& state) {
	switch (state.ir_command) {

		case MOVE_FWD: 
			if(overall_speed<0) overall_speed = 0;
			incSpeed(overall_speed);
			state.l_speed = overall_speed;
			state.r_speed = overall_speed;
		break;	

		case MOVE_BCKWD:
			if(overall_speed>0) overall_speed = 0;
			decSpeed(overall_speed);
			state.l_speed = overall_speed;
			state.r_speed = overall_speed;
		break;

		case TURN_LEFT:

				if(overall_speed>0) {
					if(state.r_speed<state.l_speed) state.r_speed = state.l_speed;
					decSpeed(state.l_speed);
					incSpeed(state.r_speed);
				}				
 				if(overall_speed == 0) overall_speed = MIN_SPEED;
				if(overall_speed<0) {
				if(state.l_speed<state.r_speed) state.l_speed = state.r_speed;
				decSpeed(state.r_speed);
				incSpeed(state.l_speed);
				}
				
			
		break;

		case TURN_RIGHT:
				
				if(overall_speed>0) {
					if(state.l_speed<state.r_speed) state.l_speed = state.r_speed;
					decSpeed(state.r_speed);
					incSpeed(state.l_speed);
				}				
 				if(overall_speed == 0) overall_speed = MIN_SPEED;
				if(overall_speed<0) {
				if(state.r_speed<state.l_speed) state.r_speed = state.l_speed;
				decSpeed(state.l_speed);
				incSpeed(state.r_speed);
				}
 

		break;

		case STOP:
			overall_speed=0;
			state.l_speed=0;
			state.r_speed=0;
		break;

			
		default: 
		break;
	}
	
	return 0;
}

/****************************************************************************/
/*                                 Infrared                               */
/****************************************************************************/

Infra::Infra(IRrecv& rcv, decode_results& res) : i_rcv(rcv), i_result(res){
	last_cmd = NO_CMD;
}

const char* Infra::cmdStr(IR_command cmd){

	switch (cmd) {
		case NO_CMD         : return "No Command";      break;
		case MOVE_FWD       : return "Move Forward";    break;
		case MOVE_BCKWD     : return "Move Backward";   break;
		case TURN_LEFT      : return "Turn Left";       break;
		case TURN_RIGHT     : return "Turn Right";      break;
		case STOP          : return "STOP";            break;
		case STRATEGY_TOGGLE: return "Strategy Toggle"; break;
	}
}

void Infra::init() {
		
	unsigned long temp = 0;
	unsigned char value[] = {0,0,0,0};
	
	for(unsigned char i=1; i<MAX_CMD; i++)
	{
		for(unsigned char j=0; j<4;j++)
		{
			value[j] = EEPROM.read((unsigned char) (4 * i + j));
	 
		}
		
		
		temp = (unsigned long) (value[0]);
		temp = temp<<8;
		temp += (unsigned long) (value[1]);
		temp = temp<<8;
		temp += (unsigned long) (value[2]);
		temp = temp<<8;
		temp += (unsigned long) (value[3]);
		
		ir_cmd_set[i] = temp;		
		temp = 0;

	}
	
}

void Infra::wireCmd(unsigned int i){

	unsigned long temp = 0;
	unsigned char value[] = {0,0,0,0};
	
	
		while(temp==0 || temp == 0xFFFFFFFF) temp = read();			  	
			
		
			value[0] = (unsigned char) (temp>>24);
			value[1] = (unsigned char) (temp>>16);
			value[2] = (unsigned char) (temp>>8);
			value[3] = (unsigned char) (temp);

		for (int j=0; j<4;j++)
		{
 
         
			EEPROM.write(i*4 + j, value[j]);
		}		
		
		ir_cmd_set[i] = (unsigned long) temp;
		
}

void Olimex2WD::simonSays(unsigned int i){

	switch(i) 
	{
	
		case 1: delay(3000); l_motor.speed(60); r_motor.speed(60); delay(500); l_motor.speed(0); r_motor.speed(0);	break;
		case 2: l_motor.speed(-60); r_motor.speed(-60); delay(500); l_motor.speed(0); r_motor.speed(0);	break;
		case 4: l_motor.speed(60); r_motor.speed(-60); delay(500); l_motor.speed(0); r_motor.speed(0);	break;
		case 3: l_motor.speed(-60); r_motor.speed(60); delay(500); l_motor.speed(0); r_motor.speed(0);	break;
		case 5: break;
		case 6: l_motor.speed(60); r_motor.speed(0); delay(500); l_motor.speed(0); r_motor.speed(60);delay(500);
				l_motor.speed(60); r_motor.speed(60); delay(500);
				l_motor.speed(-60); r_motor.speed(-60); delay(500); l_motor.speed(0); r_motor.speed(0);	break;
	
		default: break;
	}
}

unsigned long Infra::read() {
	
	if (i_rcv.decode(&i_result)) {
			if(i_result.value >= 0x800 && i_result.value < 0x900) i_result.value -=800;
		
		i_rcv.resume(); // Receive the next value
		return i_result.value;
  	}
		
	return 0;

}


IR_command Infra::getCmd() {
	
	unsigned long raw=0;
	unsigned char cmd=0;

	raw = read();
	
	if (raw == 0) {
		return NO_CMD;
	}
	
	if (raw == 0xFFFFFFFF) {
		return last_cmd;
	}

	for (int i=1;i<MAX_CMD;i++)
	{
		if (raw == ir_cmd_set[i]) {
			last_cmd = (IR_command) i;
			return last_cmd;
		}
	}
	
	return NO_CMD;
}

/****************************************************************************/
/*                                 Timer/PWM                                */
/****************************************************************************/

struct PWMMODULE pwm_modules[MAX_MODULES] = {
	{
		.set_enable = 0x80,
		.set_duty = 0x88
	},
	{
		.set_enable = 0x20,
		.set_duty = 0x8A
	},
	{
		.set_enable = 0x08,
		.set_duty = 0x08
	}
};

MotorTimer::MotorTimer(void){ //Configure Timer1 as FastPWM 8bits

	TCCR1A = 1;
	TCCR1B = 0x09;
	TCCR1C = 0;

}

void MotorTimer::setPwm(int pin, int speed){		//pin: 0-A 1-B 2-C | speed: 0-255

	_SFR_MEM16(pwm_modules[pin].set_duty) = speed;	// Set PWM value
	if (speed == 0) DDRB &= ~(1 << (5 + pin));
	else DDRB |= 1 << (5 + pin);					// Set Output Mode
	TCCR1A |= pwm_modules[pin].set_enable;			// Activate channel


}

/****************************************************************************/
/*                                 MotorDriver                              */
/****************************************************************************/

MotorDriver::MotorDriver(){
	
}

void MotorDriver::speed(int speed){

	if (speed == 0) {
		stop();
	} else if (speed > 0) {
		fwd(MIN_SPEED + speed);
	} else {
		bckwd(MIN_SPEED - speed);
	}
}

/****************************************************************************/
/*                                MotorHardware                               */
/****************************************************************************/

MotorHardware::MotorHardware(int _pA, int _pB, int _timer_module, MotorTimer& _timer) : 
MotorDriver(), pinA(_pA), pinB(_pB), timer(_timer) {

	pinMode(_pA, OUTPUT);
	pinMode(_pB, OUTPUT);
	digitalWrite(_pA, LOW);
	digitalWrite(_pB, LOW);
    timer_module = _timer_module;     
	
}

void MotorHardware::stop(){

	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);

}

void MotorHardware::fwd(int _speed){

	digitalWrite(pinA, HIGH);
	digitalWrite(pinB, LOW);
	timer.setPwm(timer_module, _speed);
}

void MotorHardware::bckwd(int _speed){

	digitalWrite(pinA, LOW);
	digitalWrite(pinB, HIGH);
	timer.setPwm(timer_module, _speed);
}

/****************************************************************************/
/*                                 Sonic                                    */
/****************************************************************************/

Sonic::Sonic(int _trigger_pin, int _echo_pin) : trigger_pin(_trigger_pin), echo_pin(_echo_pin){

	pinMode(trigger_pin, OUTPUT);
	pinMode(echo_pin, INPUT);
	digitalWrite(trigger_pin,LOW);
}

unsigned int Sonic::getDistance(){

	unsigned long time = 0;
	unsigned int dist = 0;
	
	digitalWrite(trigger_pin, HIGH);
	delayMicroseconds(50);
	digitalWrite(trigger_pin, LOW);
  
	time = pulseIn(echo_pin, HIGH);
    if(time<60000 && time>1) dist = ((time *34	) / 2) / 1000;
	
	return (unsigned int) dist;
}

/****************************************************************************/
/*                                 Olimex2WD                               */
/****************************************************************************/

Olimex2WD::Olimex2WD(
	MotorHardware& _left_m, 
	MotorHardware& _right_m, 
	Sonic& _left_s, 
	Sonic& _right_s,
	Infra& _infrared, 
	int _stby,
	int _ir_en
) : 
	l_motor(_left_m), 
	r_motor(_right_m), 
	l_sensor(_left_s), 
	r_sensor(_right_s), 
	infrared(_infrared)
{
	
	state.l_distance = 0;
	state.r_distance = 0;
	state.ir_command = NO_CMD;
	state.l_speed = 0;
	state.r_speed = 0;
	hwd_butt = 1;
	begin=1	;
	current_strategy = REMOTE;

	pinMode(_stby,OUTPUT);
	digitalWrite(_stby,HIGH);
	pinMode(_ir_en, OUTPUT);
	digitalWrite(_ir_en, LOW);
	strategies[AUTOMATIC] = new Automatic();
	strategies[REMOTE] = new Remote();
}

int Olimex2WD::motorSpeedCalc(int _distance){

	int speed=0;
	if (_distance > MAX_DIST || _distance == 0) {
		_distance = MAX_DIST;
	} else if (_distance < MIN_DIST) {
		_distance = MIN_DIST;
	}
	
	if (_distance >= REV_DIST && _distance <= STOP_DIST) {
		speed = 0;
	} else {
		speed = (_distance - REV_DIST);
	}
	
	return speed;
}

void Olimex2WD::addRemoteControl(){

	for (int i=1;i<MAX_CMD;i++)
	{
		simonSays(i);
		infrared.wireCmd(i);
	}
}


void Olimex2WD::go(void){
		
	unsigned int temp1=0;
	unsigned int temp2=0;

	delay(30);	

	temp1 = l_sensor.getDistance();

	delay(30);

	temp2 = r_sensor.getDistance();

	if(temp1) state.l_distance = temp1;
	if(temp2) state.r_distance = temp2;
		

	state.ir_command =  infrared.getCmd();
	hwd_butt = (PINE & 0x04);
	if(!hwd_butt) addRemoteControl();
	if(begin)
	{
		begin=0;
		infrared.init();
	}
		
	if (state.ir_command == STRATEGY_TOGGLE) {
		
		switch (current_strategy) {
			case AUTOMATIC: 
				
				state.l_speed=0;
				state.r_speed=0;
				current_strategy = REMOTE;
				
			break;

			case REMOTE:
				
				state.l_speed=0;
				state.r_speed=0;
				current_strategy = AUTOMATIC;			
			break;
		}
	}
	
	int d = strategies[current_strategy]->updateSpeed(state);
	
	l_motor.speed(state.l_speed);
	r_motor.speed(state.r_speed);
	
	if (delay > 0) {
		delay(d);
	}	

}
