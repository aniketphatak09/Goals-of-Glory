# Preface:

->Link to the original documentation along with all details:
http://socialledge.com/sjsu/index.php/F18:_Goals_of_Glory

->Demo Video:
https://www.youtube.com/watch?v=zId_HAEDWms


- The folder LED_Matrix contains device driver for the 64x64 LED Matrix which includes the animation and game logic that is utilized between the display and 3 SJ-One Microcontrollers.

- The Folders LPC_GYRO_player and LPC_goalkeeper consists of MPU 6050 device driver logic along with Nordic Wireless Implementation for the game to work synchronously.

# GAME Description and Statistics:

-2-Player Penalty shootout mode

-3 sec timer for each player to shoot

-Press switch on the main SJOne board to start

-Move the Goalkeeper SJone board left<->right to have a corresponding movement on LED Matrix

-Control the ball movement via player SJone board to give direction i.e. left,extreme left,right, extreme right or straight.

-Highest player to score wins and score is displayed on the result screen

# Issues, Challenges and solutions to get past them

1. RGB LED Matrix Display driver:


a) Writing the driver function on our own was a challenge. We were able to easily glow a single row in the matrix. However, the challenge was in glowing a single pixel value at a specified location. And lack of datasheet made it even more difficult. Once we got the control over glowing a single LED (check updateDisplay function ), we were able to implement various functions to draw lines, digits, text, ball, and goalkeeper.

b) Matrix flickering problem in which pixels on the screen weren't consistently bright when a particular animation was going on:

To solve this, we stopped clearing the whole display multiple times and cleared the specific pixels which mattered. Also, we were using a separate task, in which we were sending the data through the queue to control game task which was causing the task to wait on the queue till the data arrived which was causing flickering.So we removed this task and read the wireless data packet in controlGoalkeeper task and controlGame task separately

c) Ball animation to achieve swing:

We wrote an algorithm to find the path of the ball from the start point to endpoint (goalpost) to appear as if it were swinging

d) Handling screen refresh issue in which when we light up an LED, that would fade away in a few milliseconds:

The solution to this was creating an array of size 64x64 and a dedicated task to update the LED matrix.

e) The ungraceful transition of the ball from the one half of the LED matrix to the other half:

The solution to this calculated using the new X value and new color in drawPixel function. Here we are calling calculateMatrixposition fucntion which will calculate and return new x value and new color value.



2. Designing the Gyroscope and Accelerometer driver for goalkeeper and player movements:

a) Loss of connection on MPU 6050

The solution to this was hard wiring the MPU 6050 sensor to a general purpose PCB so that connection issues do not occur. Once this was done, we were able to successfully get the tilt.

b) Precision for getting accurate direction

Since we have calculated five directions for ball movement, finding accurate gyroscope and accelerometer values(threshold) with proper mapping was critical to our application.

3. Designing the Wireless driver 
a) System freeze when no data sent

We introduced a NACK to indicate that no data sent hence whenever this condition occurs it waits for the new data to be sent.

4. Dimensions of power pins for the PCB design

a) Accommodate the exact dimensions of the 5V Power pin on the PCB design.

Since there is no datasheet available for the 64x64 RGB LED Matrix, hence it was a difficult task. We used a basic approximation to determine the pin dimensions that would fit inside the PCB design such that it works just fine.
