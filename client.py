import binascii
import socket
import ssl
import time


HOST = '10.157.9.133'
PORT = 4433
counter = 1

fail = 0
while True:
    with open("t55664.dat", "rb") as f:
        while (stored_hash :=f.read(32)) :
            if not stored_hash:
                f.close()
                print("Error")
            else :
                print("[+] Hash From database:", stored_hash)
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                sock.setblocking(1);
                #print("Starting Connection")
                sock.connect((HOST, PORT))
                context = ssl.SSLContext(ssl.PROTOCOL_TLSv1_2)
                context.load_cert_chain(certfile="client.pem", keyfile="client.key")
                if ssl.HAS_SNI:
                    secure_sock = context.wrap_socket(sock, server_side=False, server_hostname=HOST)
                else:
                    secure_sock = context.wrap_socket(sock, server_side=False)
                cert = secure_sock.getpeercert()
                #print("Sending")
                secure_sock.write(b'a2bc886')
                #print("Receiving")
                data = secure_sock.read(32)
                print("[+] Hash from MCU:", data)
                print("[+] Receiving hash of block (",counter,"/ 56)")
                counter = counter + 1
                if stored_hash == bytes(data):
                    print("[+] Comparison successful")
                    secure_sock.write(b'A')
                else:
                    print("[+] Comparison Failed! possible attack!")
                    secure_sock.write(b'A')
                    f.seek(0)
                    counter = 0
                    time.sleep(5)
            secure_sock.close()
            sock.close()


