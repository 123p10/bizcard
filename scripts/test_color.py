import serial
import time

x_y_map = [0, 1, 2, 3, 4, 5,
           11, 10, 9, 8, 7, 6,
           12, 13, 14, 15, 16, 17,
           23, 22, 21, 20, 19, 18,
           24, 25, 26, 27, 28, 29,
           35, 34, 33, 32, 31, 30]

ser = serial.Serial('/dev/ttyACM0', 230400, timeout=2)
print("Setup Serial")
ser.write(bytearray([0x77]))
print("Sent Text")
res = ser.readline()
print("Response: " + str(res))
print("ACK matched: " + str(res == b'HI'))

NUM_LEDS = 36
BY = 3
t = 2
for a in range(20):

    arr = []
    for i in range(t):
        arr.append(bytearray())
    for j in range(t):
        for i in range(NUM_LEDS):
            rem = a % 6
            if x_y_map[i] % 6 == rem:
                arr[j] = arr[j] + bytearray([50, 0, 0])
            else:
                arr[j] = arr[j] + bytearray([00, 0, 10])

    for j in range(t):
        for i in range(4):
            buf = bytearray([0x55, j, i * 16, 16])
            buf = buf + bytearray(arr[j][i*16*3:(i+1)*16*3-1])
            ser.write(buf)
            print("Sent: "  + str(j) + ", " + str(i))
            print("Buff: " + str(buf.hex('_')))
    time.sleep(0.1)
