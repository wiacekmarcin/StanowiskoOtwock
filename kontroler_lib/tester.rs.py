import time
import serial
import crc8
import sys

ser = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=115200,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)
if ser.isOpen():
    ser.close()
ser.open()
ser.isOpen()

def send(bytes, num_read):
    
    sdata = [ord(b) for b in bytes]
    str = ''
    for s in sdata:
        str += '%2x ' % s
    print(str)
    ser.write(sdata)
    out = ''
    # let's wait one second before reading output (let's give device time to answer)
    i = 0
    while i < num_read:

        time.sleep(1)
        while ser.inWaiting() > 0:
            out += ser.read(1)
        if len(out) == 0 :
            continue    
        i = i + 1
        print(">>" + out)
        s = ''
        for o in out:
            s += '%2x ' % int(ord(o))
        print(s)



while True:
    i = raw_input('Podaj komende\n\t1. Welcone\n\t2. Set Param\n\t3. Position\n\t4. MoveHome\n\t5. Roleta move\n'
    '\t6. Roleta home\n\n\t0 - Wyjscie\n[1-5]:')
    hash = crc8.crc8()
    if i =='0':
        ser.close()
        sys.exit(0)
    if i == '1':
        hash.update("%c" % 0x10)
        c = hash.hexdigest()
        send("%c%c" % (0x10, int(c, 16)), 1)
    if i == '2':
        hash = crc8.crc8()
        stepX = int(raw_input("Podaj maksymalna ilosc krokow w plaszczyznie X: "))
        stepY = int(raw_input("Podaj maksymalna ilosc krokow w plaszczyznie Y: "))
        stepR = int(raw_input("Podaj maksymalna ilosc krokow w plaszczyznie Y: "))
        rev = raw_input("Podaj maske odwroconego kierunku w postaci np HHH: ")
        brev = 0
        for i in range(5): 
            brev += (0x1 << i) if rev[i]== 'H' else 0
        print(brev)
        s = "%c%c%c%c%c%c%c%c" % (0x37, brev, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, 
                step & 0xff, (mls >> 8) & 0xff, mls & 0xff)
        print(s)
        crc = hash.update(s)
        c = hash.hexdigest()
        print ("37%02x%02x%02x%02x%02x%02x%02x%s" % (brev, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, 
            step & 0xff, (mls >> 8) & 0xff, mls & 0xff, c))
    if i == '3':
        hash = crc8.crc8()
        dozo = int(raw_input("Podaj nr dozownika: "))

        step = int(raw_input("Podaj ilosc krokow: "))
        s = "%c%c%c%c%c%c" % (0x55, dozo, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff)
        crc = hash.update(s)
        c = hash.hexdigest()
        print ("55%02x%02x%02x%02x%02x%s" % (dozo, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff, c))
    if i == '4':
        hash.update("%cP" % (0x71))
        c = hash.hexdigest()
        #send("%cP%c" % (0x71, int(c, 16)), 6)
        print("%02x %02x %02x " % (0x71, 'P', int(c, 16))) 
    if i == '5':
        step = int(raw_input("Podaj ilosc krokow: "))
        hash.update("%cR%c%c%c%c" % (0x55, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff))
        c = hash.hexdigest()
        send("%cR%c%c%c%c%c" % (0x55, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff, int(c, 16)), 2)
        
    if i == '6':
        hash.update("%cR" % (0x71))
        c = hash.hexdigest()
        send("%cR%c" % (0x71, int(c, 16)), 2)
        #print("%02x %02x %02x " % (0x71, int'R', int(c, 16))) 


ser.close()
