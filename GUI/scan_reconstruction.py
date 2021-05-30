import serial
import time

output_file = open("coordinates.xyz", "w")

serialPort = serial.Serial(
    port="COM5", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE
)
serialString = ""  # Used to hold data coming over UART
try:
    while 1:
        # Wait until there is data waiting in the serial buffer
        if serialPort.in_waiting > 0:
        
            # Read data out of the buffer until a carraige return / new line is found
            serialString = serialPort.readline()

            # Print the contents of the serial data
            try:
                line=serialString.decode("utf-8").rstrip()
                print(line)
                output_file.write(line + '\n')
            except:
                pass
    time.sleep(0.001)
except:
    print("Communication closed")
finally: 
    output_file.close()
