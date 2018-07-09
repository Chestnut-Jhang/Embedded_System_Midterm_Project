# Chuan-Jia Jhang's Midterm Project: Color-detecting Machine

![](http://gitlab.larc-nthu.net/105061224/midterm_project/raw/master/Pitcures/P_20180502_133507.jpg)

## 1　Project Introduction

My project is designed to detect the color of mixture in test tube. Due to my participation in iGEM

competition, we have to make a machine to detect the concentration of bacteria. Therefore, I make a

prototype for the future. This prototype can detect the color of all mixture with two continuous servos,

and detect the temparature in avoid of too high temparature. After detecting, it can transmit the color

value to the computer and draw a line chart to show the result. The above is roughly the procedure of the machine.

## 2　Project Description

### 2.1　Preparation

(1) Prepare a GitLab working directory and and a new Mbed project.

　`git clone <URL>`

URL is the GitLab project URL: git@gitlab.larc-nthu.net:105061224/midterm_project.git

　`mbed new Color-detecting_Machine`

(2) Add "4DGL-uLCD-SE" library to the current project

　`mbed add https://os.mbed.com/users/4180_1/code/4DGL-uLCD-SE/`

(3) Add "parallax" library to the current project

　`mbed add http://gitlab.larc-nthu.net/embedded_lab/parallax`

(4) Add "TCS3200" library to the current project

　`mbed add http://gitlab.larc-nthu.net/105061224/midterm_project/tree/master/TCS3200`

　Because there is some unknown problem to include **TCS3200.cpp** in **"midterm_project/TCS3200/TCS3200.cpp"**

　like **#include "TCS3200.cpp"**, I directly add the code of **TCS3200.cpp** in the **main.cpp**.

(5) Prepare some equipments and materials for the machine.

![](http://gitlab.larc-nthu.net/105061224/midterm_project/raw/master/Pitcures/%E5%AE%8C%E6%95%B4%E6%A9%9F%E6%A7%8B%E5%9C%96.PNG)

Equipment|Quantity
---------|--------
K64F board|1
continuous servo|2
optical encoder|1
rack and gear|1
simulated circular test tube rack|1
color sensor (TCS3200)|1
temparature sensor (TMP102)|1
uLCD|1
ZigBee|2
wires|many
corrugated board|4

　The image file of simulated cicular test tube rack is at **"midterm_project/Pictures/simulated test tube rack.pdf"**.

　You can assemble the machine by the above image. The detail dimension is up to you, but the wires have to be long

　enough to connect the up and down equipments.

### 2.2　Line Configuration

Equipment|Ground|Vcc|Digital Pin
---------|------|---|-----------
servo for test tube rack|ground|6V (battery)|D13
servo for color sensor|ground|6V (battery)|D12
optical encoder|ground|5V|D8
temparature sensor|ground|3.3V|SDA->D14, SDL->D15
uLCD|ground|5V|RX->D0, TX->D1, RES->D2
XBee|ground|5V|TX->D9, RX->D7
color sensor|ground|5V|S0->D11, S1->D6, S2->D5, S3->D3

### 2.3　Control the Equipments to Run the Machine

(1) Go into the Mbed project **Color-detecting_Machine**.

　`cd ~/midterm_project/Color-detecting_Machine`

(2) Start VS code to edit **main.cpp**.

　`code main.cpp &`

(3) Copy the codes in **"midterm_project/main.cpp"**.

　The codes is written to control all the equipments in the machine.

(4) Compile the program.

　`sudo mbed compile -m K64F -t GCC_ARM -f`

(5) Push the reset button on the microcontroller.
 
### 2.4 Receive the color values from the microcontroller.
 
(1) Start VS code to edit **xbee_PC.py**.

　`code xbee_PC.py &`

(2) Copy the codes in **"midterm_project/xbee_PC.py"**.

　The codes is written to receive the color values from the microcontroller.

　After receiving, the PC will store the values in a file and draw a line chart to show the result immediately.
 
(3) Compile the program.

　`sudo python xbee_PC.py`

(4) Connect the power for the machine.

(5) During the operation, you can see that the python program outputs a text file RGB.txt about the color values.

　The example is at **"midterm_project/RGB.txt"**. At the same time, the screen shows the line chart for red values.
 
　You can change the amount of data　PC receives by yourself in xbee_PC.py (default amount is 5).

### 2.5　Demo Video

Video link: `https://drive.google.com/open?id=1uUDxHxeA1woBVR4LT1X3X2kpR5-k2iUs`

The video displays the operation of the machine with all things mentioned above.

## 3　Difficulties for Creating Color-detecting Machine

### 3.1　TCS3200

The difficulty for TCS3200 is its great sensitivity to the surrounding light. I have to alomost block all the 

interference to get the correct color value. It is not easy to do this in all different environment.

### 3.2　Assembling the Machine

Because it is the first time I use corrugated board, I spent much time to know how to cut a perfect shape I want.

Another difficulty is the configuration for all equipments, like sensors, boards, microcontroller, wires, etc.

I thought many ways to connect all of them to get the proper size and great effects.

## 4　Reference for TCS3200

(1) `https://www.ncnynl.com/archives/201606/101.html`

(2) `https://howtomechatronics.com/tutorials/arduino/arduino-color-sensing-tutorial-tcs230-tcs3200-color-sensor`