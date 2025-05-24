import requests
import time
import sys
import threading

msg_queue = []

def print_stream(text, delay=0.04):
    text = text[1:-1] if text.startswith('"') and text.endswith('"') else text
    if text == "": return
    text = text.encode().decode('unicode_escape')
    for char in text:
        sys.stdout.write(char)
        sys.stdout.flush()
        time.sleep(delay)

def read_messages():
    while True:
        r = requests.get("http://127.0.0.1:8000/msg/")
        msg_queue.append(r.text)
        time.sleep(5)

def handle_input():
    while True:
        command = input().strip()
        r = requests.post(f"http://127.0.0.1:8000/command/", json={"command": command})
        msg_queue.append(r.text)

def print_output():
    while True:
        if msg_queue:
            text = msg_queue.pop(0)
            print_stream(text)
        time.sleep(1)

if __name__ == "__main__":
    msg_thread = threading.Thread(target=read_messages, daemon=True)
    msg_thread.start()

    input_thread = threading.Thread(target=handle_input, daemon=True)
    input_thread.start()

    print_output()
