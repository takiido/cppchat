import json
import socket
import struct
import os
import time
import threading

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

def receive_message(s):
    length_bytes = s.recv(4)
    if not length_bytes:
        return None

    length = struct.unpack('!I', length_bytes)[0]

    data = b''
    while len(data) < length:
        more = s.recv(length - len(data))
        if not more:
            return None
        data += more

    message_json = data.decode('utf-8')
    return json.loads(message_json)

def listen_for_messages(s):
    while True:
        try:
            msg = receive_message(s)
            if msg is None:
                print("Connection closed be the server.")
            print(f"\n[RECEIVED] From {msg['sender']}: {msg['content']}")
        except Exception as e:
            print(f"Error receiving message: {e}")
            break


if __name__ == '__main__':
    load_dotenv(dotenv_path="../../.env.dev")

    sender = input("Enter your username: ")
    receiver = input("Enter receiver username: ")
    timestamp = time.time()

    HOST = os.getenv("HOST")
    PORT = int(os.getenv("PORT"))

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    listener_thread = threading.Thread(target=listen_for_messages, args=(sock,), daemon=True)
    listener_thread.start()

    try:
        while True:
            content = input("Enter message to send (or '!q' to exit): ")
            if content.lower() == '!q':
                break
            send_message(sock, sender, receiver, content, timestamp)
    finally:
        sock.detach()
