import time
import hashlib
F=''
# 16KB = 1024
# 8KB = 512
# 2KB = 56
Choose_size = int(input("Choose the size of blocks (For 2KB Enter 128, 8KB Choose 512 and 16KB Enter 1024):"))


Blocks = 7168/Choose_size
counter = 0
counter2 =0
fo = open("Hex_Dump_2KB.txt", "r+") #from "arm-none-eabi-readelf -x <elfname> > input.txt")
for _ in range(2):
        next(fo)
strb = ''
m = hashlib.sha256()

while True:

        str = fo.readline()
        str = str[12:48]
        str = str.replace(' ', '')
        A = (bytes.fromhex(str))
        m.update(A)
        counter = counter + 1
        if counter2<=Blocks: #this counter to read 7 blocks each is 16KB
            if counter == Choose_size : #Counter that hashes 16KB and stores it in output.dat
                hash = m.digest()
                f = open("t556y64.dat", "ab+")
                f.write(bytes(hash))
                print("[+] Hash extracted and written to database",hash)
                counter=0
                counter2=counter2+1
                if counter2 == (Blocks):
                    f.close()
                    exit()
                #time.sleep(5)
                m = hashlib.sha256()
