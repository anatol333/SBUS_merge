This sketch merge signsls from two input SBUS lines into one output. Board used is Arduino Mega2560
 * At boot it sends an output packet with default values, about 500 for second line related channels and 302 for channel #2 (count from zero) for line #1 channels,
 * so connection to servos and transmitting of output signals can be checked without signals on input lines
 * Then, after 3 seconds controller starts to listen to input lines. When a valid packet comes on input 1 - an output packet is sent, channels 0...7 of it 
 * repeating data in input #1 packet. Channels 8...15 contain data from the last received packet on input #2 or, if nothing was received - default values.
 * pins used on Arduino Mega 2560: input #1 - 19, input #2 - 17, output - 14, LED1 - 31, LED2 - 33.
 * Sketch uses library  Bold Flight Systems SBUS by Brian Taylor
 * by now library files are placed into sketch's folder so should be no need to install in Arduino IDE
 * debuggiing signals sent by usb interface (115200 bod): at boot: version of software, on receiving a valid packet on line #1: "rx1", on receiving a valid packet on line #2: "rx2"
 * LEDs flash on boot to check it's cercuits
