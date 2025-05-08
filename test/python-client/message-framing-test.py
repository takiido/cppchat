import json
import socket
import struct
import os
import time

from dotenv import load_dotenv


def send_message(s, _sender, _receiver, _content, _timestamp):
    msg = {
        "sender": _sender,
        "receiver": _receiver,
        "content": _content,
        "timestamp": _timestamp
    }

    message_bytes = json.dumps(msg).encode('utf-8')

    length = struct.pack('!I', len(message_bytes))
    s.sendall(length + message_bytes)


if __name__ == '__main__':
    load_dotenv(dotenv_path="../../.env.dev")

    sender = "user_1"
    receiver = "user_2"
    timestamp = time.time()

    HOST = os.getenv("HOST")
    PORT = int(os.getenv("PORT"))

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    try:
        while True:
            content = input("Enter message to send (or '!q' to exit): ")
            if content.lower() == '!q':
                break
            send_message(sock, sender, receiver, content, timestamp)
    finally:
        sock.detach()
