import socket
import sys


s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM, 0)
s.connect(sys.argv[1])

while True:
    usr_data = input().encode("utf-8")
    s.sendall(usr_data)
    data = s.recv(len(usr_data))
    if data != usr_data:
        print("failed:", data, "!=", usr_data)
        break

s.close()
