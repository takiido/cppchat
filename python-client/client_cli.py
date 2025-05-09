from datetime import datetime
import json
import socket
import struct
import os
import time
import threading
import sys
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

def connect_to_server(s, host, port, retry_interval=3):
    print(f"Connecting to server on {host}:{port}")
    while True:
        try:
            s.connect((host, port))
            print(f"Connected to {host}:{port}")
            authorize(s, sender)
            return
        except ConnectionRefusedError:
            print(f"Connection refused. Retrying in {retry_interval} seconds...")
            time.sleep(retry_interval)
        except Exception as e:
            print(f"Connection failed: {e}. Retrying in {retry_interval} seconds...")
            time.sleep(retry_interval)

def listen_for_messages(s):
    while True:
        try:
            msg = receive_message(s)[0]
            if msg is None:
                print("\n🔌 Connection closed by the server.")
                break
            dt_str = datetime.fromtimestamp(msg['timestamp']).strftime('%Y-%m-%d %H:%M:%S')
            sys.stdout.write('\r' + ' ' * 100 + '\r')  # Clear input line
            print(f"[{dt_str}] @{msg['sender']}: {msg['content']}")
            sys.stdout.write("Enter message to send (or '!q' to exit): ")
            sys.stdout.flush()
        except Exception as e:
            print(f"\nError receiving message: {e}")
            break

def authorize(s, username):
    username_msg = {"username": username}
    msg_bytes = json.dumps(username_msg).encode('utf-8')
    length = struct.pack('!I', len(msg_bytes))
    s.sendall(length + msg_bytes)

if __name__ == '__main__':
    load_dotenv(dotenv_path="../.env.dev")

    sender = input("Enter your username: ")
    receiver = input("Enter receiver username: ")

    HOST = os.getenv("HOST")
    PORT = int(os.getenv("PORT"))

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connect_to_server(sock, HOST, PORT)

    listener_thread = threading.Thread(target=listen_for_messages, args=(sock,), daemon=True)
    listener_thread.start()

    try:
        while True:
            sys.stdout.write("Enter message to send (or '!q' to exit): ")
            sys.stdout.flush()
            content = sys.stdin.readline().strip()

            if content.lower() == '!q':
                break

            timestamp = int(time.time())
            send_message(sock, sender, receiver, content, timestamp)

            # Clear previous line (prompt) and print your own message
            sys.stdout.write('\033[F\033[K')
            date_str = datetime.fromtimestamp(timestamp).strftime('%Y-%m-%d %H:%M:%S')
            print(f"[{date_str}] @$: {content}")
    finally:
        sock.close()
