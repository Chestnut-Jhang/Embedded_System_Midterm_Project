import serial
import time
import matplotlib.pyplot as plt

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

s.write("+++")
char = s.read(2)
print("Enter AT mode.")
print(char)

s.write("ATMY 0x47\r\n")
char = s.read(3)
print("Set MY 0x47.")
print(char)

s.write("ATDL 0x48\r\n")
char = s.read(3)
print("Set DL 0x48.")
print(char)

s.write("ATWR\r\n")
char = s.read(3)
print("Write config.")
print(char)

s.write("ATMY\r\n")
char = s.read(3)
print("MY :")
print(char)

s.write("ATDL\r\n")
char = s.read(3)
print("DL : ")
print(char)

s.write("ATCN\r\n")
char = s.read(3)
print("Exit AT mode.")
print(char)

# i = 0
# line = [0] * 50
# s.read(1)
# send to remote
# s.write("abcd")
# while True:
#     line[i] = s.read(1)
#     if line[i] != '\r' and line[i] != '\n':
#         i = i + 1
#     else:
#         print(str(line))
#         i = 0
i = 0
red = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]  ##
times = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
fp = open("RGB.txt", "w")
while i < 5:
    color = s.readline() ##
    red[i] = int(color)
    line = s.readline()
    # fp.write(red[i])
    fp.write(line)
    i = i + 1
fp.close()
plt.plot(times, red) ##
plt.xlabel('Times')
plt.ylabel('Red Color')
plt.show() ##

s.close()