
*NextionHMI_Test.ino* uses serial commands to communicate with the Nextion HMI. \
Remember each command must be finalized with three bytes 0xFF. 

If you need no controller-specific commands then porting the program to another controller is very easy. 

For simplicity I use here a timer interrupt to build the measuring period. This is specific to 8-bit AVR and not good for portability. But, for most other controller there will be a simple timer which can be used instead of this timer interrupt. So this should be no big issue.

The used DHT11 Library is specific for Arduino but compatible for the Arduino family including ESPxxx etc. Therefore, no additional issues are to be expected here.
