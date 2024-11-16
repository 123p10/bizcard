import serial
ser = serial.Serial('/dev/ttyACM0', 230400, timeout=2)
print("Setup Serial")
ser.write(bytearray([0x77]))
print("Sent Text")
res = ser.readline()
print("Response: " + str(res))
print("ACK matched: " + str(res == b'HI'))
