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

//nawiniete sa juz 3 warstwy o grubosci 0.7 na walku o srednicy 30
def LRolObrot(n):
    return ((3+n)*0.7+30)*3.1415926

def R_mm2step(mm):
    n = 0:
    while n < 14:
        obwod = LRolObrot(n)
        if mm < obwod:
            return 12000*n + int(12000*mm/obwod)
        mm = mm - obwod
        n += 1
    return 0

def compare(out, bytes):
    
    if len(out) != len(bytes):
        return False

    for i in range(len(out)):
        if ord(out[i]) != bytes[i]:
            return False

    return True

def toint(b1, b2, b3, b4):
    return ord(b1) << 24 + ord(b2) << 16 + ord(b3) << 8 + ord(b4)


def send(bytes):
    
    sdata = [ord(b) for b in bytes]
    
    str = ''
    for s in sdata:
        str += '%2x ' % s
    print("Send [", str, "]")
    ser.write(sdata)
    

def welcome(bytes):
    
    send(bytes)
    out = ''
    # let's wait one second before reading output (let's give device time to answer)
    i = 0
    time.sleep(1)
    
    while True:
        while ser.inWaiting() > 0:
            out += ser.read(1)
        
        if len(out) == 0 :
            continue 

        if len(out) < 17: 
            print("ERROR:", out, ' '.join(['%02x' % ord(o) for o in out]))
            return;

        if not compare(out, [0x2f, 0x4b, 0x4f, 0x4e, 0x54, 0x52, 0x4f, 0x4c, 0x45, 0x52, 0x57, 0x49, 0x41, 0x54, 0x52, 0x32, 0xdc]):
            print("ERROR:", out, ' '.join(['%02x' % ord(o) for o in out]))
            return
        
        print("welcome - OK")
        return

def radio(bytes):
    
    send(bytes)
    out = ''
    # let's wait one second before reading output (let's give device time to answer)
    i = 0
    time.sleep(1)
    
    while True:
        while ser.inWaiting() > 0:
            out += ser.read(1)
        
        if len(out) == 0 :
            continue 

        print(out)
        return


def sendPos(bytes):
    send(bytes)
    out = ''
    # let's wait one second before reading output (let's give device time to answer)
    i = 0
    
    idx = 1
    while True:
        time.sleep(1)
        while ser.inWaiting() > 0:
            out += ser.read(1)
        
        if len(out) == 0 :
            continue 

        if len(out) >= 3 and idx == 1:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x61, 0x73, 0xbe]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            idx = 2
            print("Start pozycjonowania")
            continue

        if len(out) >= 3 and idx == 2:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x61, 0x63, 0xe3]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            idx = 3
            print("Start pozycjonowania osi X")
            continue

        if len(out) >= 11 and idx == 3 and out[1] == 'P':
            sout = out[0:11]
            out = out[11:]
            idx = 4
            print("Koniec pozycjonowania osi X ", toint(sout[2],sout[3],sout[4],sout[5]), toint(sout[6],sout[7],sout[8],sout[9]))
            continue

        if len(out) >= 3 and idx == 4:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x61, 0x64, 0xdb]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            idx = 5
            print("Poczatek pozycjonowania osi Y")
            continue    

        if len(out) >= 11 and idx == 5 and out == 'G':
            sout = out[0:11]
            out = out[11:]
            idx = 6
            print("Koniec pozycjonowania osi Y ", toint(sout[2],sout[3],sout[4],sout[5]), toint(sout[6],sout[7],sout[8],sout[9]))
            continue    

        if len(out) >= 3 and idx == 6:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x61, 0x4b, 0x16]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            print("Koniec pozycjonowania")
            return

        if len(out) > 1:
            print("ERROR:", out, ' '.join(['%02x' % ord(o) for o in out]))
            return

def homePos(bytes):
    send(bytes)
    out = ''
    # let's wait one second before reading output (let's give device time to answer)
    i = 0
    
    idx = 1
    while True:
        time.sleep(1)
        while ser.inWaiting() > 0:
            out += ser.read(1)
        
        if len(out) == 0 :
            continue 

        if len(out) >= 3 and idx == 1:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x81, 0x73, 0xfd]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            idx = 2
            print("Start bazowania")
            continue

        if len(out) >= 3 and idx == 2:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x81, 0x63, 0x8d]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            idx = 3
            print("Start bazowania osi X")
             continue

        if len(out) >= 7 and idx == 3 and out[1] == 'P':
            sout = out[0:7]
            out = out[7:]
            idx = 4
            print("Koniec bazowania osi X ", toint(sout[2],sout[3],sout[4],sout[5]))
            continue

        if len(out) >= 3 and idx == 4:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x81, 0x64, 0x98]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            idx = 5
            print("Poczatek bazowania osi Y")
            continue    

        if len(out) >= 7 and idx == 5 and out[1] == 'G':
            idx = 6
            sout = out[0:7]
            out = out[7:]
            print("Koniec bazowania osi Y ", toint(sout[2],sout[3],sout[4],sout[5]))
            continue    

        if len(out) >= 3 and idx == 6:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x81, 0x4b, 0x55]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            print("Koniec bazowanania")
            return

        if len(out) > 1:
            print("ERROR:", out, ' '.join(['%02x' % ord(o) for o in out]))
            return

def sendRoleta(bytes):
    send(bytes)
    out = ''
    # let's wait one second before reading output (let's give device time to answer)
    i = 0
    
    idx = 1
    while True:
        time.sleep(1)
        while ser.inWaiting() > 0:
            out += ser.read(1)
        
        if len(out) == 0 :
            continue 

        if len(out) >= 3 and idx == 1:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x61, 0x53, 0x5e]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            idx = 2
            print("Start pozycjonowania rolety")
            continue

        if len(out) >= 11 and idx == 2 and out[1] == 'R':
            sout = out[0:11]
            out = out[11:]
            print("Koniec pozycjonowania rolety ", toint(sout[2],sout[3],sout[4],sout[5]), toint(sout[6],sout[7],sout[8],sout[9]))
            return
        
        if len(out) > 1:
            print("ERROR:", out, ' '.join(['%02x' % ord(o) for o in out]))
            return

def homeRol(bytes):
    send(bytes)
    out = ''
    # let's wait one second before reading output (let's give device time to answer)
    i = 0
    
    idx = 1
    while True:
        time.sleep(1)
        while ser.inWaiting() > 0:
            out += ser.read(1)
        
        if len(out) == 0 :
            continue 

        if len(out) >= 3 and idx == 1:
            sout = out[0:3]
            out = out[3:]
            if not compare(sout, [0x81, 0x52, 0x1a]):
                print("ERROR:", sout, ' '.join(['%02x' % ord(o) for o in sout]))
                return
            idx = 2
            print("Start bazowania rolety")
            continue

        if len(out) >= 7 and idx == 2 and out[1] == 'R':
            sout = out[0:7]
            out = out[7:]
            print("Koniec bazowania rolety ", toint(sout[2],sout[3],sout[4],sout[5]))
            return

        if len(out) > 1:
            print("ERROR:", out, ' '.join(['%02x' % ord(o) for o in out]))
            return    


while True:
    i = raw_input('Podaj komende\n\t1. Welcone\n\t2. Set Param\n\t3. Position\n\t4. MoveHome\n\t5. Roleta move\n'
    '\t6. Roleta home\n\t7. Pozycja rolety w mm. \n\t8.Radio\n\t0 - Wyjscie\n[1-5]:')
    hash = crc8.crc8()
    if i =='0':
        ser.close()
        sys.exit(0)
    if i == '1':
        hash.update("%c" % 0x10)
        c = hash.hexdigest()
        welcome("%c%c" % (0x10, int(c, 16)))
        continue
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
        continue
    if i == '3':
        hash = crc8.crc8()
        stepX = int(raw_input("Podaj ilosc krokow w plaszczyznie X: "))
        stepY = int(raw_input("Podaj ilosc krokow w plaszczyznie Y: "))
        s = "%cP%c%c%c%c%c%c%c%c" % (0x59, stepX >> 24, (stepX >> 16) & 0xff, (stepX >> 8) & 0xff, stepX & 0xff,
                                           stepY >> 24, (stepY >> 16) & 0xff, (stepY >> 8) & 0xff, stepY & 0xff)
        crc = hash.update(s)
        c = hash.hexdigest()
        sendPos(s + '%c' % int(c, 16))
    if i == '4':
        hash.update("%cP" % (0x71))
        c = hash.hexdigest()
        #send("%cP%c" % (0x71, int(c, 16)), 6)
        #print("%02x %02x %02x " % (0x71, 'P', int(c, 16))) 
        homePos("%cP%c" % (0x71, int(c, 16)))
    if i == '5':
        step = int(raw_input("Podaj ilosc krokow: "))
        hash.update("%cR%c%c%c%c" % (0x55, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff))
        c = hash.hexdigest()
        sendRoleta("%cR%c%c%c%c%c" % (0x55, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff, int(c, 16)))
        
    if i == '6':
        hash.update("%cR" % (0x71))
        c = hash.hexdigest()
        homeRol("%cR%c" % (0x71, int(c, 16)))

    if i == '7':
        mm = int(raw_input("Podaj ilosc mm rolety: "))
        step = R_mm2step(mm)
        print("step", step)
        hash.update("%cR%c%c%c%c" % (0x55, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff))
        c = hash.hexdigest()
        sendRoleta("%cR%c%c%c%c%c" % (0x55, step >> 24, (step >> 16) & 0xff, (step >> 8) & 0xff, step & 0xff, int(c, 16)))
        
    if i == '8':
        hash.update("%c" % (0x90))
        c = hash.hexdigest()
        radio("%c%c" % (0x90, int(c, 16)))

ser.close()
