#include <Arduino.h>
#include <avr/io.h>
#include <stdbool.h>
#include <IRremote.h>
#include <EEPROM.h>

#define MAX_MODULES 3
#define MAX_DIST 400
#define MIN_DIST 11
#define REV_DIST 13
#define STOP_DIST 18
#define MIN_SPEED 50
#define MAX_SPEED 250	
#define SPEED_STEP 10

//PWM modules adresses for enable and setting duty cycle.
struct PWMMODULE {
	int set_enable;
	int set_duty;
};

class MotorTimer {
	struct PWMMODULE timer1_modules[MAX_MODULES];

	public:
		MotorTimer(void);              
		void setPwm(int _module, int _speed);

};


class MotorDriver {
	public:
		MotorDriver();
		void speed(int _speed);

	protected:
		virtual void stop();
		virtual void fwd(int _speed);
		virtual void bckwd(int _speed);
};

class MotorHardware : public MotorDriver {
	public:
		MotorHardware(int _pA, int _pB, int _timer_module, MotorTimer& _timer);

	protected:
		void stop();
		void fwd(int _speed);
		void bckwd(int _speed);

	private:	
		int pinA;
		int pinB;
		int timer_module;
		MotorTimer& timer;
};

typedef enum _IR_command {
	NO_CMD,
	MOVE_FWD,
	MOVE_BCKWD,
	TURN_LEFT,
	TURN_RIGHT,
	STOP,
	STRATEGY_TOGGLE,
	
	MAX_CMD
} IR_command;

typedef struct _carstate_ {

	unsigned long l_distance;
	unsigned long r_distance;
	IR_command ir_command;
	
	float l_speed;
	float r_speed;

} CARSTATE;

typedef enum _strategy_type_ {
	AUTOMATIC,
	REMOTE,
	
	MAX_STRATEGY
} STRATEGY_type;

class CarStrategy {
	public:
		CarStrategy() {};
		virtual STRATEGY_type getType() {};
		virtual const char* getName() {};
		virtual int updateSpeed(CARSTATE& _state) {};
};

class Automatic : public CarStrategy {
	
	protected:
		int delta_distance;
		int calcSpeed(int _distance);
	
	public:
		Automatic();	
		STRATEGY_type getType();
		const char* getName();
		int updateSpeed(CARSTATE& _state);
};

class Remote : public CarStrategy {

	protected: 
		float overall_speed;
		void incSpeed(float& _speed);
		void decSpeed(float& _speed);

	public:
		Remote();
		STRATEGY_type getType();
		const char* getName();
		int updateSpeed(CARSTATE& _state);
			
};

class Infra {

	public:
		Infra(IRrecv& _rcv, decode_results& _res);
		void init();
		void wireCmd(unsigned int _i);
		unsigned long read();
		IR_command getCmd();
		const char* cmdStr(IR_command _cmd);
		
	protected:
		unsigned long ir_cmd_set[MAX_CMD];
		IR_command last_cmd;
		IRrecv& i_rcv;
		decode_results& i_result;
};


class Sonic {

	public:
		Sonic(int _trigger_pin,int _echo_pin);	
		unsigned int getDistance();
		
	protected:
		unsigned int trigger_pin;
		unsigned int echo_pin;


};

class Olimex2WD {

	public:
		Olimex2WD(
			MotorHardware& _left_m, 
			MotorHardware& _right_m, 
			Sonic& _left_s, 
			Sonic& _right_s,
			Infra& _infrared, 
			int _stby,
			int _ir_en
		);
		
		void go(void);

	private:
		MotorHardware& l_motor;
		MotorHardware& r_motor;
		Sonic& l_sensor;
		Sonic& r_sensor;
		Infra& infrared;
		byte hwd_butt;
		CarStrategy* strategies[MAX_STRATEGY];
		
		STRATEGY_type current_strategy;

		CARSTATE state;
		bool begin;
		int motorSpeedCalc(int _distance); 
		void simonSays(unsigned int _i);
		void addRemoteControl();
};
