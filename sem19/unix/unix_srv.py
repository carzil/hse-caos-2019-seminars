import socket

listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
listener.bind(("127.0.0.1", 9091))
listener.listen(1)

while True:
    client, addr = listener.accept()
    print("got client:", addr)

    while True:
        data = client.recv(4096)
        if len(data) == 0:
            print("got eof from client")
            break

        print("got data {} from {}".format(data, addr))

    print("listening for a new client")
