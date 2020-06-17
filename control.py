import serial
import time
import asyncio
import json

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=2);
if(ser.isOpen() == False):
    ser.open()

# dataFile = open('dataFile.txt', 'w')

#############
#  Set Points
#############

heat_min_runtime = 30
heat_cooldown = 10
time_last_heat = 0
setpoint_temp_max = 26.0
setpoint_temp_min = 20.0

#############
#  Functions
#############

def getdata(str):
    data = ser.readline()
    arduinodata = json.loads(data)
    value = arduinodata[str]
    return value

def heaton():
    print('Turning Heat On')
    ser.write("A\r".encode())

def heatoff():
    ser.write("a\r".encode())

###########
# Main Function
###########
def main():

  global heat_cooldown
  global time_last_heat

  while True:
    # This gets the latest temp data from the PI
    temp_in = float(getdata('TempOut'))

    if(temp_in < setpoint_temp_max and time.time() > (time_last_heat + heat_cooldown)):
        print(f'Temp is {temp_in}')
        heaton()
        time_last_heat = time.time()

    elif(temp_in > setpoint_temp_max and time.time() > (time_last_heat + heat_min_runtime)):
        print(f'Temp is {temp_in}')
        print("Heat off")
        heatoff()
        print(f'Temp={temp_in}| heat_cooldown rem={time.time() - (time_last_heat + heat_cooldown)}')
        print(f'Temp={temp_in}| heat_min_runtime={time.time() - (time_last_heat + heat_min_runtime)}')
#        print(time.time())
#        print(time_last_heat)
#        print(time_last_heat + heat_cooldown)
    else:
        print(f'Temp={temp_in}| heat_cooldown rem={time.time() - (time_last_heat + heat_cooldown)}')
        print(f'Temp={temp_in}| heat_min_runtime={time.time() - (time_last_heat + heat_min_runtime)}')

#########
# Main Initiator
#########

main()

#	dataFile.write(arduinoData)
#	dataFile.close()
