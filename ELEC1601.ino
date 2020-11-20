
// MAZE (WHISKER ONLY) ATTEMPTED SOLUTION


// Include servo library and declare servos
#include <Servo.h>                     
Servo servoLeft;
Servo servoRight;

void setup() {                          // Built-in initialization block
    servoLeft.attach(13);               // Attach left signal to pin 13
    servoRight.attach(12);              // Attach right signal to pin 12
    pinMode(5, INPUT);                  // Set left whisker pin to input 
    pinMode(7, INPUT);                  // Set right whisker pin to input
}

void loop() {
    // WHEN WE ENTER THE MAZE THE ROBOT JUST BE POINTED SLIGHTLY TO THE OUTER EDGE

    
    
    while (1) {
        byte wLeft = digitalRead(5);  
        byte wRight = digitalRead(7);
        int wall;

        // left = 1700, Right = 1300 means the robot goes straight forward

        if (wRight == 0 && wLeft == 1) {   // If right whisker contact
            servoLeft.writeMicroseconds(1300);
            servoRight.writeMicroseconds(1700);
            delay(250);
            servoLeft.writeMicroseconds(1300);
            servoRight.writeMicroseconds(1500); // Pivot outward left slightly
            delay(100);

            wall = 1;                  
        }
        else if (wRight == 1 && wLeft == 0) {   // If left whisker contact
            servoLeft.writeMicroseconds(1300);
            servoRight.writeMicroseconds(1700);
            delay(250);
            servoLeft.writeMicroseconds(1500);
            servoRight.writeMicroseconds(1700); // Pivot outward right slightly
            delay(100);

            wall = 0;
        }
        else if (wRight == 0 && wLeft == 0) {   // If both whiskers contact
            if (wall == 0) { // If the wall was on the left before, do a turn to the right
                servoLeft.writeMicroseconds(1300);
                servoRight.writeMicroseconds(1700);
                delay(1000);
                // 90 turn to the right
                servoLeft.writeMicroseconds(1500);
                servoRight.writeMicroseconds(1700); 
                delay(1000);
            }
            else if (wall == 1) { // If the wall was on the right before, do a turn to the left
                servoLeft.writeMicroseconds(1300);
                servoRight.writeMicroseconds(1700);
                delay(1000);
                // 90 turn to the left
                servoLeft.writeMicroseconds(1300);
                servoRight.writeMicroseconds(1500);
                delay(1000);
            }
        }

        // Reset movement to go straight forward
        servoLeft.writeMicroseconds(1700);
        servoRight.writeMicroseconds(1300);

        // Unfortunately there is no detection as to when we reach the end.
    }
}


// ______________________________________________________________________________________________________________________________

// ATTEMPTED MEMORY SOLUTION - SECONDARY AND TERTIARY TASKS


int counter = 0; // Counter for number of commands that are used

int state = 0; // The state determines what sequential code to run when we click the button
// 0 = manual navigation, 1 = automatic maze/line pathfinding, 2 = secondary navigation automation
// 3 = automatic maze/line pathfinding, 4 = reverse navigation automation
// EACH TIME WE PRESS THE BUTTON THE STATE INCREMENTS BY 1


// memoryList stores a list of sequential movements for the manual navigation
// in pairs of two int elements (direction, length of time)
// Only use static initialisation - dynamic memory allocation is unfeasible on microcontrollers
int memoryList[50]; // we assume a max of 25 movements (+ time)

int moveDirection = 2; // Save last movement direction in manual navigation
bool moveChanged = false; // Boolean set to true every time a move is being made
// 0 = left, 1 = right, 2 = forward, 3 = backward
int lastTime = 0; // Save the last time 


// Automatic memory solution for Manual Navigation
while (1) {
    if (state == 0) {
        // After every time we make a movement and stop it saves a record of that movement and
        // the time it took
        if (not moving) {
            // Essentially when the joystick is being pressed, moveChanged is set to true
            // and the moveDirection is being set to the current direction
            // Only after we stop movement does this statement activate, saving
            // a record before setting the boolean to false again so it doesn't
            // re-activate until movement occurs again
            if (moveChanged == true) {
                int difference = millis() - lastTime;
        
                memoryList[counter] = moveDirection;
                memoryList[counter+1] = difference;

                counter += 2;
                moveChanged = false;
            }
            // Only when movement is not happening is the lastTime set to the current
            // time. Otherwise when moving this is suspended and when movement finishes
            // the length of time can be captured from the last time it was not moving
            lastTime = millis();
        }
    }

    else if (state == 1 || state == 3) {

        // Pathfinding

    }

    else if (state == 2) {
        // Standard Repeat
        int size = sizeof(memoryList)/(sizeof(array[0]));
        for (int i = 0; i < size; i += 2) {
        if (memoryList[i] == 0) { // Turn left
            servoLeft.writeMicroseconds(1300);
            servoRight.writeMicroseconds(1500);
        }
        else if (memoryList[i] == 1) { // Turn Right
            servoLeft.writeMicroseconds(1500);
            servoRight.writeMicroseconds(1700);
        }
        else if (memoryList[i] == 2) { // Go Forward
            servoLeft.writeMicroseconds(1700);
            servoRight.writeMicroseconds(1300);
        }
        else if (memoryList[i] == 3) { // Go Backward
            servoLeft.writeMicroseconds(1300);
            servoRight.writeMicroseconds(1700);
        }
        delay(memoryList[i+1]);
        delay(100);
    }

    else if (state == 4) {
        // Reverse Traversal
        int size = counter+1; // Final Element
        // We only reverse the turns
        for (int i = size; i > 1; i -= 2) {
            if (memoryList[i-1] == 0) { // Turn right (REVERSED)
                servoLeft.writeMicroseconds(1500);
                servoRight.writeMicroseconds(1700);
            }
            else if (memoryList[i-1] == 1) { // Turn left (REVERSED)
                servoLeft.writeMicroseconds(1300);
                servoRight.writeMicroseconds(1500);
            }
            else if (memoryList[i-1] == 2) { // Go Forward (NOT REVERSED)
                servoLeft.writeMicroseconds(1700);
                servoRight.writeMicroseconds(1300);
            }
            else if (memoryList[i-1] == 3) { // Go Backward (NOT REVERSED)
                servoLeft.writeMicroseconds(1300);
                servoRight.writeMicroseconds(1700);
            }
            delay(memoryList[i]);
            delay(100);
        }   
    }

}

// When the servo writeMicroseconds is above 1500, it goes anticlockwise, otherwise it goes anticlockwise.
// 1300 -> maximum speed CLOCKWISE, 1700 -> maximum speed of ANTICLOCK
// 1500 = no movement

// WHISKERS: 1 -> NO contact, 0 -> Contact

// Caution 1: Pins 6 and 7 are used for the software serial connection between the Arduino and the Bluetooth shield. 
// Pin A1 is used to monitor the connection status of the Bluetooth module. If you connect anything to these three pins 
// (6, 7, A1) you will create a conflict and the Bluetooth connection will not work correctly.

// Caution 2: Pins 0 and 1 are used for the serial connection between the Arduino and the computer. If you connect anything 
// to these two pins (0, 1) you will create a conflict and the serial connection to the computer 
// (and thus your Serial Monitor window) will not work correctly.

// ______________________________________________________________________________________________________________________________

// REFERENCES

// https://learn.parallax.com/tutorials/robot/shield-bot/robotics-board-education-shield-arduino/chapter-5-tactile-navigation-8
// (Whisker values)
// https://canvas.sydney.edu.au/courses/25935/pages/11-dot-3-4-robot-vision (IR Sensor setup)
// https://playground.arduino.cc/Code/PrintingNumbers/ (itoa() function)
// https://www.arduino.cc/reference/en/language/variables/data-types/array/ (Arduino arrays)
// https://forum.arduino.cc/index.php?topic=527619.0 (Determine array sizes)
// https://www.arduino.cc/reference/en/language/functions/time/millis/ (millis() function)
// https://forum.arduino.cc/index.php?topic=461241.0 (Static vs Dynamic allocation of memory)
