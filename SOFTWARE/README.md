# I. INTRODUCTION

This library is designed to be used with Olimex's product [2WD-CTRL](..).
It allows you to easily setup and control the vehicle.
This library supports two modes, which we call "strategies":

- **Automatic**: In this mode the car will use the Ultrasonic sensors and moves autonomously
- **Remote**: In this mode, you can set up an InfraRed remote control and use it to control the car.

**NOTE**: When using Remote mode, you should have clear visibility and should be no more than 2-3 meters from the vehicle.

Before using the provided library and it's example, check the **SOFTWARE** and **HARDWARE** requirements. 

To install this library, copy the [OLIMEX_2WD](..) folder with all its subfolders and files into your Arduino Library directory.
For more information about Including Libraries to Arduino, click [here](https://www.arduino.cc/en/guide/libraries).

# II. SOFTWARE REQUIREMENTS 

1. Infrared library (tested with version 2.0).
We used [this one](http://z3t0.github.io/Arduino-IRremote/).



2. Arduino IDE software (tested with version 1.8.1)

3. Add the content from folder "libraries" to your Arduino Libraries directory

# III. HARDWARE REQUIREMENTS

1. Olimex 2WD-CTRL 
2. MicroUSB cable for programming the product 
3. US-015 Ultrasonic Rangefinder modules 
4. 3.7 V Li-Po /Li-ion battery
- **NOTE**: You cannot program or run 2WD-CTRL without attaching a 3.7V Li-ion/Li-Po battery!
5. Infrared Remote Control.
You can use almost any remote control, including Remote Controls used for TVs, radios, and DVDs.

# IV. LIBRARY INTRODUCTION

This library contains several C++ classes.
Libraries required to setup and control this product are:

### MotorTimer

#### Description

Configures Timer1 as FastPWM 8bit.
You can use any or all of Timer1 outputs to generate PWM square waves.

#### Usage

    MotorTimer <object_name>; 

### MotorHardware 

#### Description

Creates a Motor object (associated with a physical motor)

#### Usage

    MotorHardware <object_name> = MotorHardware(int _pA, int _pB, int _timer_module, MotorTimer& _timer); 

...where `_pA` and `_pB` are the motor drive direction pins, according to the Arduino Leonardo pinouts.
`_timer_module` is an integer with values of `0,1,2`, representing `timer1`'s modules `(A,B,C)`.
According to the hardware setup, you should call this constructor as showed in the example.
`_timer` is the object which initializes `Timer1`.

    Sonic <object_name> = Sonic(int _trigger_pin,int _echo_pin);

...where `_triger_pin` is the `2WD-Ctrl` trigger pin according to Arduino Leonardo pinout.
`_echo_pin` is the `2WD-Ctrl` echo pin according Arduino Leonardo pinout. 

**NOTICE**: This product supports 2 US-015 sensors, so there are 2 trigger pins and two echo pins.

### IRrecv 

#### Description

This class creates infrared object. 

#### Usage

    IRrecv <object_name>(recv_pin); 

...where `recv_pin` is the pin associated with the infrared sensor on the `2WD-CTRL` board, according Arduino Leonardo pinouts.

### decode_results 

#### Description

Creates an object which will do all the calculation data for getting IR commands code and store it. 

#### Usage

    decode_results <object_name>; 

    Infra ir_sensor = Infra(irrecv, results); 

...where `irrecv` is the `IRrecv` object which should be created first results is the decode_results object.

### Olimex2WD

#### Description

This is the abstracion class which you need to initialize the Car hardware.
It's constructor gets as reference the objects that you have already created:

- MotorHardware left_motor
- MotorHardware right_motor
- Sonic left_sensor
- Sonic right_sensor
- Infra _infrared_object
- int _standby_pin (according to Arduino Leonardo's pinout)
- 2WD-CTRL setup

#### Usage

    Olimex2WD <object> = Olimex2WD( MotorHardware& _left_m, MotorHardware& _right_m, Sonic& _left_s, Sonic& _right_s, Infra& _infrared, int _stby); 

...where:

- `_left_m`: left motor object
- `_right_m`: right motor
- `object _left_s`: left sensor object
- `_right_s`: right sensor 
- `object _infrared`: IR object
- `_stby`: Standby pin (according to Arduino Leonardo pinout)

**NOTICE**: Standby pin's value is constant and is equal to 5, according to the 2WD-CTRL hardware setup.

### Olimex2WD::go() 

#### Description

This method is used to do all the neccesary operations including:

- reading sensors
- calculations
- strategy toggling
- updating motor speeds
- etc. 

#### Usage

Call this method in the loop function of the Arduino sketch as follows:

```bash
void loop() { <Olimex2WD_object_name>.go(); 

}
```

# V. USAGE EXAMPLE

You can use the Example in this library to test the `2WD-CTRL` vehicle device/module.
To do so: 

1. Launch Arduino IDE (this library should be moved in the Arduino Libraries folder before launching)
2. Go to "File" menu

- choose `Examples` submenu
- scroll down until you see `OLIMEX_2WD`
- Choose `OLIMEX_2WD` submenu

3. Click on the Compile button
4.

4.1 Plug the MicroUSB cable in the 2WD-CTRL board
4.2 Connect the 3.7V Li-ion/Li-Po battery
4.3 Turn the switch ON 5.
Click the Upload button of Arduino IDE 6.
Wait untill you see "Upload Done" message 7.

You are now ready.
Read below to see how to use `2WD-CTRL` module:

# VI. EXAMPLE OF USING 2WD-CTRL MODULE AND ARDUINO EXAMPLE SKETCH 

If you did everything right, your vehicle should be ready to be controlled.

1. The first thing that you have to do is to press and hold the `HWD` button on the `2WD-CTRL` board for 1 second.
Release the button; after about 2 seconds the car will move forward.
This indicates that you need to press the button of your remote control that you want to be assigned as `Move Forward` button. 

2. If the command is assigned with the pressed button, the car will move backwards.
That indicates that now you have to pass the second command which is move backward.
Press the desired button of your IR Remote Control and wait until the car moves left.

3. If the last command is accepted, the car will run left, which indicates that you have to assign button with the command `Turn Right`

4. When you press the command for Turn Right command, **THE CAR WILL DO NOTHING**.
This indicates that it waits a new button to be assigned with `STOP` command.
Please press the desired button to be associated with `STOP` command.

5. If everything is successfull until now, the car will move forward,backward,turn left, turn right.
This shows that the last button to be assigned is the one for switching Car control modes.
(Remember that there are two modes/strategies: **Automatic** and **Remote**)
Press the desired button of your Remote Control to be associated with `StrategyToggle` command. 

6. If you managed to successfuly assign your remote control to the `OLIMEX-2WD` car's movement control commands you cand try:

6.1 move forward, pressing the assigned key for that command
6.2 move backward, pressing the assigned key for that command
6.3 turn left, pressing the assigned key for that command
6.4 turn right, pressing the assigned key for that command
6.5 stop, pressing the assigned key for that command
6.6 Switch to automatic mode (NOTE that the Ultrasonic sensors should be installed before powering up the car) by pressing the 
StrategyToggling/Mode changing button

# VII. IMPORTANT NOTES

The MAXIMUM distance that infrared control will work is about 2-3 meters.
Note that you need to have clear visibility Note that if you press a button several times, say "Move Forward" button, it will 
increase the speed of forward movement.
The same thing applies for moving backward, turning left, and turning right.

Holding a button down is equal to pressing it several times in succession.
The vehicle will increase its movement speed as long as you hold it down.

Holding down the "StrategyToggle" button will cause the vehicle to continuesly change strategies.
To not do this, press the button but do not hold it down.
