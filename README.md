# Pong_CoreProyect
## Context
The propose of this proyect was a didactic way to learn about hardware manage with a microcontroller and distract yourselfs. 
In this document I am going to explain all the elements that I used and the code that I wrote.

## element's list
The elements that are used for this proyect are:
  1. 8x8 led matrix (the used matrix: [1088AS](https://pdf1.alldatasheet.com/datasheet-pdf/view/1179268/ETC/1088AS.html)(optional))
  3. [74HC595 x2](https://pdf1.alldatasheet.com/datasheet-pdf/view/12198/ONSEMI/74HC595.html)
  4. A 4 digit 7 segment display (the used display: [SH5461AS](https://www.datasheet-pdf.info/attach/1/2118861884.pdf))
  5. Potentiometer (joystick) x2
  6. 1k Ohm resistor x2
  7. Bread Board x2
  8. Arduino Uno R3 microcontroller

## hardware connections
Fristly, the matrix an the chips are put on one of the boards, then you connect the pin according to the led matrix datasheet, the connection depends of the 
matrix that it choose although the first pin is the left part of the serial number. Also, the joysticks can be placed there, the pins that are used for the
joystick are: VCC(5V),GND and VRx or VRy (we only move the paddles in only one direction). Additionally, the 74HC595 is formed by a flip-flop register and 
latch register. That means the information it loads when the clock change on  a high clock cycle from a serial input, and when the latch clock is high the 
second register load the elements on parallel, and when the clock signal is low, it does not change nothing.

In addition of this, in the other Bread Board is placed the display with two resistors, the resistors are conected to the display select to enable the digit.
In this case the second and the third digit are employed, that there are the eighth and ninth pin, be careful, the display enable works with a low signal, and
the segments works in high level, they are diodes, that means that the display work when the circuit is close and they have more voltage in the cathode than
in the anode and the diode threshold, in other works, it woks when the circuit has current and 3.3V. 

Lastly, we conect on the arduino board:
  1. Serial data inputs of the two registers in the second and the fifth pin
  2. Clocks in the forth and sixth pin
  3. Lactch in the third pin
  4. Potentiometers in the Analog 0 (A0) and Analog 1 (A1) pin
  5. 7 segments of the display in the seventh, eighth, ninth, tenth and eleventh pin
  6. enable displays are in the twelfth, thirteenth pin

## Code explanation
The code is made up of:
  1. ***Structures:***

     1. t_DISPLAY defines:
        1. The enable display pin
        2. score in that display   
     3. t_PADDLE defines:
        1. The position on the matrix with paddle_In ('1' send Voltage) and paddle_Out ('0' close the circuit)
        2. The position of the paddle in the program (virtual position)
        3. The value of the joystick.
     4. t_BALL defines:
        1. The position on the matrix with position_X('1' send Voltage) adn position_Y ('0' close the circuit)
        2. The direction that the ball follows
        3. The position of the ball in the program (virtual position)
  3. ***Constants:***
     1. leds_7seg define the pin used for give the information to the display
     
  5. ***Setup funtion:***
     Open all the ports that are going to used 
     
  7. ***Loop funtion:***
     1. pick up the information of the joystick (analogRead())
     2. move the paddles on the led matrix (movePaddle())
     3. print the paddle on the matrix (printBoard())
     4. move the ball on the matrix (moveBall())
     5. confirm if the ball collide with a wall or a paddle, and in that case the direction is changed (collide())
     6. print the ball in the matrix  (printBoard())
     7. update the score if it need it (updateScore())
     8. confirm if any player win the math (isWin())
     9. Finally ends the program (exit(0))
    
  9. ***Complement funtions:***
     1. reset(): restart with the predefined values
     2. printBoard(byte paddle_in, byte paddle_out): load the flip-flop register and activate the latch to draw on the matrix
     3. movePaddle(t_PADDLE paddle): check the value of the joystick and shift the physical and virtual position
     4. moveBall(t_BALL *ball): move physical and virtual position, be careful, you have to reverse the position_Y in bytes to operate corretly
     5. collide(): change the direction of the ball when the ball collide with a wall or a paddle
     6. collideWall(): check if in the position of the ball where you put on the voltage is the end of the matrix
     7. collidePaddle(): check if the ball is in fornt of the paddle and is in its rage
     8. updateScore(): check if the ball is in one of the limits of the matrix in the Y edge, in that case the score is updated and the display printed, else
        print the display.
     9. printScore(int score): prints the score in the display, its only print cero, one, two, three
     10.isWin(): returns true if any score is 3 or false if anyones is 3

##Conclusion
To conclude the objetive of this proyect is teach about programming in a microcontroller like Arduino although the code is deficiency and it can be 
improvable. Despite the code can be better, the proyect has hardware complexity that is interesting to analise.
