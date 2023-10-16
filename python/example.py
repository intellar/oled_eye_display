import display_control
import serial
import time

arduino = serial.Serial("COM3", 115200, timeout=.1)
time.sleep(0.5)

for i in range(8):    
    display_control.sendAnimToDisplay(arduino,i)
    time.sleep(2)
