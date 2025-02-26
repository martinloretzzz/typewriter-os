import requests
import time
import sys

def print_stram(text, delay=0.04):
    text = text.encode().decode('unicode_escape')
    for char in text:
        sys.stdout.write(char)
        sys.stdout.flush()
        time.sleep(delay)  

if __name__ == "__main__":
    while True:
        command = input().strip()
        r = requests.get(f"http://127.0.0.1:8000/command/{command}")
        answer = str(r.text)
        answer = answer[1:-1] if answer.startswith('"') and answer.endswith('"') else answer
        print_stram(f"{answer} ")
