import serial
from csv import writer

arduino_port = "COM11" #serial port of Arduino
baud = 115200 #arduino uno runs at 9600 baud
fileName="analog-data.csv" #name of the CSV file generated


ser = serial.Serial(arduino_port, baud)
print("Connected to Arduino port:" + arduino_port)
file = open(fileName, "a")
print("Created file")


#display the data to the terminal
getData=str(ser.readline())
data=getData[0:][:-1]
print(data)

#add the data to the file
file = open(fileName, "a") #append the data to the file
file.write(data + "\\n") #write data with a newline

#close out the file
file.close()