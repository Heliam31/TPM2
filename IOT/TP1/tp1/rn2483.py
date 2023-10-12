#!/usr/bin/python
# -*- encoding: utf-8 -*-
# -*- Rahim KACIMI 2023 -*-
# -*- kacimi@irit.fr -*-

import time
import serial
from subprocess import PIPE, Popen



writeconfig = 1

sp = serial.Serial()
sp.port = '/dev/ttyACM0'
sp.baudrate = 57600
sp.parity = serial.PARITY_NONE
sp.bytesize = serial.EIGHTBITS
sp.stopbits = serial.STOPBITS_ONE
sp.dtr = 0
sp.open()

appkey = "0123456789ABCDEF0123456789ABCDEF"
appeui = "DEAD25DEAD25DEAD"
deveui = "DEADDEAD00090001"

# Data rate values [0-5] = SF[12-7]
dr = 0

def send(data):
   sp.write((data+"\x0d\x0a").encode())
   data.rstrip()
   time.sleep(2)
   rdata=sp.read_all()
   rdata=rdata.decode("utf-8")
   return rdata

print(sp.read_all().decode("utf-8"))


print("Resetting device : ")
print(send("sys reset"))

print("Setting the appkey : " + appkey)
print(send("mac set appkey " + appkey))

print("Setting the appeui : " + appeui)
print(send("mac set appeui " + appeui))

print("Setting the deveui : " + deveui)
print(send("mac set deveui " + deveui))

print("Setting the data-rate : " + str(dr))
print(send("mac set dr " + str(dr)))

print("Saving mac settings")
print(send("mac save"))

print("Preparing to join the network")
print(send("mac join otaa"))

period = 4
while True:
   msg="hello-01" 
   print(sp.read_all().decode("utf-8"))
   print("Sending message: " + str(msg.encode('utf-8').hex()) + " to broker")
   print(send("mac tx cnf 220 " + msg.encode('utf-8').hex()))
   print("Waiting for " + str(period) + "s before sending again")
   time.sleep(period)
