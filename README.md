# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

# Overview

# Rubric Points

## Compilation

### Your code should compile.
It compiles on my ubuntu 16.04 laptop without error.

## Implementation

### The PID procedure follows what was taught in the lessons.
The PID implementation is in the file [src/PID.cpp](https://github.com/sunpochin/pid-control/blob/master/src/PID.cpp) :

* `PID::UpdateError` updates proportional, integral and derivative errors.
* `PID::TotalError` calculates the total error using 3 coefficients for P, I, D components and decide new steering angle.

## Reflection

### Describe the effect each of the P, I, D components had in your implementation.
* Proportional component reduces the cross track error(CTE) by steering the car to center. However it could make the car overshooting if used without Differential component.

* Derivative component reduces the overshooting of P component and make the car stay in track.

* Integral component is supposed to reduce system bias. In this project it seems to help the car stay near center when passing sharp curve.


### Describe how the final hyperparameters were chosen.
I started with setting hypterparameters ```Kp, Ki, Kd``` to ```(1, 0, 1)``` and multiple ```Kp``` and ```Kd``` with powers of 1.1 . With ```Kp, Ki, Kd``` = ```1.0, 0.0, 4.59``` , the car can compelete a lap with large swinging. <br>
So I tried tuning down hyperparameters by 1 magnitude, using powers of 1.1 * 0.1 . Finally I settled with ```Kp``` = ```0.214  ( (1.1 ^ 8) * 0.1 )``` , ```Kd``` = ```7.2890 ( (1.1 ^ 45)  * 0.1 )```. <br>
```Ki``` seems doesn't affect the result much, 0 or 0.01 both worked.


## Simulation

### The vehicle must successfully drive a lap around the track.
Result video youtube: [link](https://youtu.be/O9IUDq2UJTE)


## Installation
Installation instruction from Udacity repository: [link](https://github.com/udacity/CarND-PID-Control-Project/blob/master/README.md)

