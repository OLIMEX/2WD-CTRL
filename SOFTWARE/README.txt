
I. INTRODUCTION 

This library is designed to be used with Olimex's product "2WD-CTRL". It 
allows you to easily setup and control the car. This library supports 
two modes, which we call "strategies": 
- Automatic: In this mode the car will use the Ultrasonic sensors and
will move autonomously.
- Remote: In this mode, you can setup an InfraRed remote control and
use it to control the car. (You should notice, that using Remote mode,
you should have clear visibility and should be no more than 2-3 meters
away from the car. 

Before using the provided library and it's example, check t he SOFTWARE 
and HARDWARE requirements. 

To install this library just copy the OLIMEX_2WD folder with all it's 
subfolders and files in your Arduino Library directory. Check this link 
for more information about Including Libraries to Arduino. 

https://www.arduino.cc/en/guide/libraries 

II. SOFTWARE REQUIREMENTS 

1. Infrared library. We used this one: 
http://z3t0.github.io/Arduino-IRremote/ 

(Tested with version 2.0) 

2. Arduino IDE software. (tested with version 1.8.1) 

3. Add the content from folder "libraries" to your Arduino Libraries 
directory 

III. HARDWARE REQUIREMENTS: 

1. Olimex 2WD-CTRL 
2. MicroUSB cable for programming the product 
3. US-015 Ultrasonic Rangefinder modules 
4. 3.7 V Li-Po /Li-ion battery !!! Note that you can't program or run 
the 2WD-CTRL product without attaching 3.7V Li-ion/Li-Po battery 
5. Infrared Remote Control. You can use almost any remote control, 
including Remote Controls used for TVs, radios, DVDs, 

IV. LIBRARY INTRODUCTION: 

This library contains several C++ classes, most important and the ones 
that need in need to setup and control this product are: 

###MotorTimer:#### 

description: 

Configures Timer1 as FastPWM 8bit. You can use any or all of Timer1 
outputs to generate PWM square waves. 

usage: 

MotorTimer <object_name>; 

###MotorHardware:#### 

description: Creates Motor object (associated with phisical motor) 

usage: 

MotorHardware <object_name> = MotorHardware(int _pA, int _pB, int 
_timer_module, MotorTimer& _timer); 

where: _pA and _pB are the motor's drive direction pins according the 
Arduino Leonardo pinouts _timer_module is integer which value could be 
0,1,2 representing timer1's modules (A,B,C). According the hardware 
setup, you should call this constructor as showed in the example. _timer 
is the object which initializes Timer1. Sonic <object_name> = Sonic(int 
_trigger_pin,int _echo_pin); where: _triger_pin is the 2WD-Ctrl trigger 
pin according Arduino Leonardo pinout. _echo_pin is the 2WD-Ctrl echo 
pin according Arduino Leonardo pinout. 

!!!NOTICE that this product supports 2 US-015 sensors, so there are 2 
trigger pins and two echo pins 

###IRrecv### 

description: This class creates infrared object. 

usage: 

IRrecv <object_name>(recv_pin); 

where: recv_pin is the pin associated with the infrared sensor on the 
2WD-CTRL board according Arduino Leonardo pinouts 

###decode_results### 

description: 

creates object which will do all the calculation data for getting IR 
commands code and sotre it. 

usage: 

decode_results <object_name>; 

Infra ir_sensor = Infra(irrecv, results); 

where: irrecv is the IRrecv object which should be created first results 
is the decode_results object 

###Olimex2WD### 

description: 

This is the abstracion class which you need to initialize the Car 
hardware. It's constructor gets as refference the objects that you 
already created: 

MotorHardware left_motor MotorHardware right_motor Sonic left_sensor 
Sonic right_sensor Infra _infrared_object int _standby_pin - according 
Arduino Leonardo's pinout and 2WD-CTRL setup. 

usage: 

Olimex2WD <object> = Olimex2WD( MotorHardware& _left_m, MotorHardware& 
_right_m, Sonic& _left_s, Sonic& _right_s, Infra& _infrared, int _stby 
); 

where: _left_m is the left motor object _right_m is the right motor 
object _left_s is the left sensor object _right_s is the right sensor 
object _infrared is the IR object _stby is the Standby pin according 
Arduino Leonardo' pinouts 

!!!NOTICE: Standby pin's value is constant and is equal to 5, according 
the 2WD-CTRL hardware setup 

###Olimex2WD::go()### 

description: 

this method is used to do all the neccesary operations including: 
reading sensors, calculations, strategy toggling, updating motor speeds 
and etc. 

usage: 

call this method in the loop function of the Arduino sketch as follows: 

void loop() { <Olimex2WD_object_name>.go(); 

} 

V. USAGE EXAMPLE: 

You can use the Example in this library to test the 2WD-CTRL car 
device/module To do so: 

1. Launch Arduino IDE (this library should be moved in the Arduino 
Libraries folder before launching) 2. Go to "File" menu ---> choose 
"Examples" submenu ---> scroll down until you see OLIMEX_2WD ---> Choose 
OLIMEX_2WD submenu 3. Click on the Compile button. 4. 4.1 Plug the 
MicroUSB cable in the 2WD-CTRL board, 4.2 Connect the 3.7V Li-ion/Li-Po 
battery, 4.3 Turn the switch ON 5. Click the Upload button of Arduino 
IDE 6. Wait untill you see "Upload Done" message 7. You are ready. 

Read bellow, to see how to use 2WD-CTRL module: 

VI. EXAMPLE OF USING 2WD-CTRL MODULE AND ARDUINO EXAMPLE SKETCH 

If you did everything right, your Car should be ready to be controled by 
you. 

1. The first think that you have to do is to press the HWD button on the 
2WD-CTRL board and hold it pressed for 1 second. Then you have to 
release the button and after about 2 seconds the car will run forward. 
This indicates that you need to press the button of your remote control 
that you want to be assigned as Move Forward button. 

2. If the command is assigned with the pressed button, the car will run 
backwards. That indicates that now you have to pass the second command 
which is move backward. Press the desired button of your IR Remote 
Control and wait until the car moves left. 

3. If the last command is accepted, the car will run left, which 
indicates that you have to assign button with the command Turn Right 

4. When you press the command for Turn Right command, THE CAR WILL DO 
NOTHING. This indicates that it waits a new button to be assigned with 
STOP command. Please press the desired button to be associated with STOP 
command. 

5. If everything is successfull until now, the car will run 
forward,backward,turn left, turn right. This shows that the last button 
to be assigned is the one for switching Car control modes. (If you 
remember from above, there are two modes/strategies - Automatic and 
Remote) Press the desired button of your Remote Control to be associated 
with StrategyToggle command. 

6. If you managed to successfuly assign your remote control to the 
OLIMEX-2WD car's movement control commands you cand try: 6.1 move 
forward, pressing the assigned key for that command 6.2 move backward, 
pressing the assigned key for that command 6.3 turn left, pressing the 
assigned key for that command 6.4 turn right, pressing the assigned key 
for that command 6.5 stop, pressing the assigned key for that command 
6.6 Switch to automatic mode (NOTE that the Ultrasonic sensors should be 
installed before powering up the car) by pressing the 
StrategyToggling/Mode changing button. 

VII. IMPORTANT NOTES: 

Note that the MAXIMUM distance that infrared control will work is about 
2-3 meters. Note that you need to have clear visibility Note that if you 
press a button several times, let's say Move Forward button, it will 
increase the speed of moving forward. Same thing for move backward, turn 
left, turn right. Note that holding a button is equal to press it 
several time. So as much you hold it, the much it will increase it's 
movement speed Note that if you hold StrategyToggle button it will 
continuesly change strategies. So you need to press it shortly. 

