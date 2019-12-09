# PID Controller

## Introduction

The goal of the project is to control the vehicle steering using the PID procedure. The sole criteria of the project is that the car should be able to drive one complete lap.


## Steps

### Running the program
The implementation uses PID procedure to control the steering of the vehicle. 
1. The implementation can be compiled in test mode to run twiddle or without it. 
2. To run in test mode, compile time flag "RUN_TWIDDLE"  should be provided. 
3. In test mode, the program takes 3 arguments to print error over a period with N = 100.
4. In test mode, a 4 the argument should be provided to run the vehicle all along the track
5. In release mode no arguments are neeeded.

### Determining KP, KI, KD
1. Implementation uses the procedure defined in [here](https://www.thorlabs.com/tutorials.cfm?tabID=5dfca308-d07e-46c9-baa0-4defc5c40c3e)
2. The parameters are generated manually for test.
3. Initially, KI and KD are kept as zero.
4. Ku is adjusted and different values of KP are tried to minimise the error.
5. After KP is finalised, Pu is adjusted to generated KI keeping KD as 0.
6. Once KP and KI are finalised, Pu is fine tuned to get KD.
7. After final tests following value is obtained KP = 2.1, KI = 0.007, KD = 157.5.
8. The values generated manually to test the application is provided in attached file evaluate.xlsx

