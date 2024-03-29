# Microcontroller
## Abstract

this project aims to implement all the possible control structures (descrete time) in microntrollers throw a serires of C++ library and Python scripts. In order to be available for more microcontrollers possible it is tested on Arduino IDE.

## Structure

This project can be divided in tree main parts:

1. **Identification** of the system which will be controlled or used for other similar purpose like observing the real process. Throw identification tecniques (i.e. Areas, Relay feedback, Least square ...).
2. **Controller building** a series of cntrol structures (i.e. PID, Smith predictor ...) which you can choose and tune properly in order to reach the desired results.
3. **Controller implementation** on a micro controller.
![implementation](https://raw.githubusercontent.com/campregher99/Microcontroller/main/images/microcontroller.png)

## Code structure
* **System identification**
 + Arduino: the [Estimator](https://github.com/campregher99/Microcontroller/blob/main/arduino/estimator/estimator.ino) program provides the hardware part of the identification procedure. You have to define the input, output and reference functins, which will have the following signture:

	`void output(float out);`	`float input(void);`	`float reference(void);`
 + Python: [Estimator](https://github.com/campregher99/Microcontroller/blob/main/python/Estimator.py) script provides the user interface and save the values estimated by the micro controller.

* **Controller building**
 + Python: [Controler_design](https://github.com/campregher99/Microcontroller/blob/main/python/Controller_design.py) permits to choice the desired controll structure and perform the calculations in order to build the control law. Finally permit to save the generated law.

* **Controller implementation**
 + Arduino: [Controller](https://github.com/campregher99/Microcontroller/blob/main/arduino/Controller/Controller.ino) implements the actual code which will control the process. You have to define the input and output functins, as before.
 + Python: [micro_setter](https://github.com/campregher99/Microcontroller/blob/main/python/micro_setter.py) uploads the desired control law on micro controller.

All the Arduino programs allow to enable the DEBUG and MONITOR modalities placed in [SetUp](https://github.com/campregher99/Microcontroller/blob/main/arduino/library/SetUp.h) file:
* *DEBUG* is used for the develop phase, must be disabled when you use the python scripts.
* *MONITOR* is used to stream the data in order to visualize them on the Serial Plotter of Arduino IDE, must be disabled when you use the python scripts.

## Current implementation
**Identification methods:**

* *Areas method* used for FOPDT systems returns time constant, delay and dc gain of the process and standard deviation of initial condition and at steady state. It is implemented only for positive system output. The system is assumed in a stable initial condition. Can be detected process that have a time constant less than thirty seconds otherwise the procedure maight fail.
* *Relay feedback method* used for FOPDT systems returns time constant, delay and dc gain of the process and standard deviation at steady state. It is implemented only for positive system output.

**Controller structures:**

* **PID**
	* *Ziegler and Nichols* is a method used only for load disturbance rejection task. It aims to make the system dacay ratio to 0.25, which implies a damping factor of 0.22 and a consequent overshoot of 50%.
	* *Chien-Hrones-Reswick* it is an improovement of the Ziegler and Nichols method that implement different law for load disturbance rejection task and set point following task, morover permit to choose the desired overshoot percentage (this version only 0% or 20%).
	* *Haalman* is an analytical method that set the desired open-loop function as:
$$L(s)=C(s)P(s)=\frac{2}{3Ls}e^{-sL}$$
* **Cancellation controller** basic structure only for system which has low disturbs, since it is based on pole-zero cancelation, it permits to choose the settling time according to the actuator saturation.
* **Anti-windup** implemented from 0% to 100%, always activated.

**Microcontroller tested:**
* **ESP-WROOM-32** [Az-delivery version](https://www.az-delivery.de/it/products/esp32-developmentboard)
**Warning:** the adc of this board is non linear (from 0V to 0.1V is always encoded with 0), it is not suitable for accurate tasks.

## User Guide
1. *Add* the [library](https://github.com/campregher99/Microcontroller/tree/main/arduino/library) folder to the Arduino librries.
2. *Define* the input/output/reference functions as shown in the Code  structure paragraph and insert them in [Estimator](https://github.com/campregher99/Microcontroller/blob/main/arduino/estimator/estimator.ino) and [Controller](https://github.com/campregher99/Microcontroller/blob/main/arduino/Controller/Controller.ino). You have to insert definition under the `loop()` function, prototype ahead `main()` and pass the pointers at `tuning.begin_()` and `controller.begin_()` method call.
2. *Upload* [Estimator](https://github.com/campregher99/Microcontroller/blob/main/arduino/estimator/estimator.ino) on the Arduino boards.
3. *Open* [Estimator](https://github.com/campregher99/Microcontroller/blob/main/python/Estimator.py) on your computer and make sure that the Arduino serial monitor or plotter are closed  (if the program stops after you select the board try to restart [Estimator](https://github.com/campregher99/Microcontroller/blob/main/python/Estimator.py)).
4. *Follow* the scripts instruction.
5. *Save* the output data.
6. *Open*  [Controler_design](https://github.com/campregher99/Microcontroller/blob/main/python/Controller_design.py).
7. *Follow* the scripts instruction.
8. *Save* the output controller law.
9. *Upload* [eraser](https://github.com/campregher99/Microcontroller/blob/main/arduino/eraser/eraser.ino) that erases the micro controler eeprom.
10. *Upload* [Controller](https://github.com/campregher99/Microcontroller/blob/main/arduino/Controller/Controller.ino).
11. *Open* [micro_setter](https://github.com/campregher99/Microcontroller/blob/main/python/micro_setter.py).
12. *Follow* the scripts instruction.
13. *Ready* to control the systems

## Example

### System

The system used for the example is a RC circuit that emulate the behaviour of a mass-damper mechanical system, which is a FOPDT system, considering the delay time introduced by the component.
> **Scheme**
>![RC](https://raw.githubusercontent.com/campregher99/Microcontroller/main/images/RC.jpg)
> **Real circuit**
>![realRC](https://raw.githubusercontent.com/campregher99/Microcontroller/main/images/photoRC.png)
> **Transform function**
> $$G(s)=\frac{R_{2}}{R_{1}R_{2}Cs+R_{1}+R_{2}}$$

**Values:**
* R1 = 1012 Hom
* R2 = 8352 Hom
* C = 404.7 uF
* tau = 365.3 ms

### I/O/R functions
#### Input
```
float input()
{
  return (float)analogRead(4);
}
```
#### Output
```
void output(float _output)
{
  ledcWrite(CHANNEL_PWM, _output * (pow(2, RESOLUTION_PWM) - 1)/100);
}
```
#### Reference
```
float reference()
{
  static float ref = 0;
  if(Serial.available())
  {
    ref = Serial.readStringUntil(ENDER).toFloat();
	clear_in_buffer();
  }
  return ref;
}
```
### Video explanetion
* [Estimation](https://www.youtube.com/watch?v=6kdfCB7v6HE&t=8s)
* [Control designing](https://www.youtube.com/watch?v=_sXwOQpxr8A)
* [Control implementatio](https://www.youtube.com/watch?v=COWamDWwHf8)



