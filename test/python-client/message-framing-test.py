import socket
import struct
import os
from dotenv import load_dotenv


def send_message(s, message):
    message_bytes = message.encode('utf-8')
    length = len(message_bytes)

    length_pref = struct.pack('!I', length)

    s.sendall(length_pref + message_bytes)


if __name__ == '__main__':
    load_dotenv(dotenv_path="../../.env.dev")

    HOST = os.getenv("HOST")
    PORT = int(os.getenv("PORT"))

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    try:
        while True:
            msg = input("Enter message to send (or '!q' to exit): ")
            if msg.lower() == '!q':
                break
            send_message(sock, msg)
    finally:
        sock.detach()
