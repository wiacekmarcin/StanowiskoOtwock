import sys
import crc8

def S(st, i):
    if i >= len(st):
        return 0
    print(st[i])
    return ord(st[i])


hash = crc8.crc8()

s = "%c%c" % (0x41, 0x1)
print(s)
crc = hash.update(s)
c = hash.hexdigest()
s += chr(int(c,16))
print(c)
print("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x" % (
    S(s,0),S(s,1),S(s,2),S(s,3),S(s,4),S(s,5),S(s,6),S(s,7),S(s,8),S(s,9),S(s,10),S(s,11),S(s,12),S(s,13),S(s,14),S(s,15),S(s,16)
))
"""
while True:
    i = raw_input('Podaj komende\n\t1. Welcone\n\t2. Set Param\n\t3. Position\n\t4. Echo\n\t5. MoveHome\n\t6. Reset\n'
    '\t7. Echo\n\n\t0 - Wyjscie\n[1-5]:')
    hash = crc8.crc8()
    if i =='0':
        sys.exit(0)
    if i == '1':
        hash.update("%c" % 0x10)
        c = hash.hexdigest()
        print ("10%s" % c)
    if i == '2':
        hash = crc8.crc8()
        step = int(raw_input("Podaj maksymalna ilosc krokow: "))
        mls = int(raw_input("Podaj czas impulsu: "));
        rev = raw_input("Podaj maske odwroconego kierunku w postaci np HHHHL: ")
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
        hash.update("%c%c" % (0x71,0x00))
        c = hash.hexdigest()
        print ("7100%s" % c)
    if i == '5':
        dozo = int(raw_input("Podaj nr dozownika: "))
        hash.update("%c%c" % (0x91, dozo))
        c = hash.hexdigest()
        print ("91%02x%s" % (dozo, c))
    if i == '6':
        hash.update("%c" % 0xb0)
        c = hash.hexdigest()
        print ("b0%s" % c)
    if i == '7':
        hash.update("%c%c" % (0x71,0x01))
        c = hash.hexdigest()
        print ("7101%s" % c)
"""