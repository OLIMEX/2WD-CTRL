#include <IRremote.h>
#include <OLIMEX_2WD.h>
#include <avr/io.h>

/*
Please don't change the pins bellow
Notice that all the pin numbers are according Arduino Leonardo's pinout and 2WD-CTRL PCB wiring
*/

MotorTimer timer1; //This object initializes Timer1 hardware module

MotorHardware left_wheel = MotorHardware(A3,A2,0,timer1); //Create object that controls left wheel
MotorHardware right_wheel = MotorHardware(A0,A1,1,timer1); //Create object that controls right wheel

Sonic left_sensor = Sonic(4,13); //Creates object for reading Left Ultrasonic Sensor
Sonic right_sensor = Sonic(6,7); //Creates object for reading Right Ultrasonic Sensor
int recv_pin = A5; //The pin that Infrared Detector is associated to, according Arduino Leonardo's pinout
IRrecv irrecv(recv_pin); // Creates object that reads IR module signals
decode_results results; // Creates object that converts the signal into data and store the Command value

Infra ir_sensor = Infra(irrecv, results); // Creates object of Abstraction class for IR control

Olimex2WD car = Olimex2WD(
                        left_wheel, // Pass the left motor object as refference
                        right_wheel, // Pass the right motor object as refference
                        left_sensor, // Pass the left Ultrasonic sensor object as refference
                        right_sensor,  // Pass the right Ultrasonic sensor object as refference
                        ir_sensor, // Pass the Infrared Abstraction object as refference
                        A4, // this is the Standby pin. It is a constant so you should leave it as it is.
                        5 // this is the Infrared enable pin
                        );
                        
  
void setup() {
  timer1=MotorTimer (); //initializes Timer1 module  
  irrecv.enableIRIn(); // Starts the receiver
  //Enable IR reciver
  DDRE &= ~(1<< 2); //Enables the IR module
  
}

void loop() {

  car.go(); //Starts Olimex2WD car
 
}
