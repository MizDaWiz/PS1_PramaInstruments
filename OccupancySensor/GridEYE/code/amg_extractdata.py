# amg_test.py Basic test of AMG8833 sensor

# Released under the MIT licence.
# Copyright (c) Peter Hinch 2019

import machine
import utime
import math
#from setuptools import ncsv
from amg88xx import AMG88XX



#To create an 2d list of size rows and columns
i2c = machine.I2C(1)
sensor = AMG88XX(i2c)
while True:
    my_data=[]
    utime.sleep(3)
    sensor.refresh()
    file = open("train.csv","w")
    sum=0
    sum2=0
    for row in range(8):
        #print()
      
        for col in range(8):
            #print('{:4d}'.format(sensor[row, col]), end='')
            lite=int('{:4d}'.format(sensor[row, col]) )
            #print(lite)
            
            sum=sum+lite
            sum2=sum2+(lite*lite)
            my_data.append(lite)
            #print(my_data)
            
            #for i in range(8)
                #sum= sum + my_data[i]
    #print(my_data)
    stdev=math.sqrt((sum2/64)-(sum*sum/4096))
    #print(stdev)
    #print(my_data)
    for col in range(64):
            #print(my_data[col])
            #print('{:4d}'.format(sensor[row, col]), end='')
            my_data[col]=(my_data[col]-(sum/64))/stdev
#             print(my_data[col])
            print('\n')
    for row in range(64):
        
        print('{:4f}'.format(my_data[row]), end=' ')
        if (row+1)%8==0 and row is not 0:
           print('\n')

   
    file = open("train.csv","w")
    file.write(str(my_data)+",")
    file.close()
            
        