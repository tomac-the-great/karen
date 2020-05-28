import serial
import time
import json

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=5);
time.sleep(2)
#dataFile = open('dataFile.txt', 'w')

while (1):

        arduinoData = ser.readline()
        print(arduinoData)
	temp = json.loads(arduinoData)
	print temp['TempOut']


#	dataFile.write(arduinoData)
#	dataFile.close()
