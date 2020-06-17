import serial
import time
import json

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1);
if(ser.isOpen() == False):
    ser.open()
ser.flushInput()

##dataFile = open('dataFile.txt', 'w')

while (1):
  user = input()
  arduinoData = ser.readline()
  ser.write(str.encode(user))
  time.sleep(1)
