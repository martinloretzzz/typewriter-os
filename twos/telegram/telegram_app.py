from twos.settings import Settings
import requests

class TelegramApp:
    def __init__(self, config: Settings):
        self.telegram_token = config.telegram_token
        self.chat_ids = set()
        self.read_updates = set()

    def __call__(self, parameters):
        messages = self.get_updates()
        chat_ids = [msg['chat']['id'] for msg in messages]
        self.chat_ids.update(chat_ids)

        if parameters is not None and parameters != " ":
            self.send_message(parameters)

        msg_texts = [msg['text'] for msg in messages]

        if len(msg_texts) > 0:
            return "\n".join(msg_texts)

        return "-"

    def get_updates(self):
        response = requests.get(f"https://api.telegram.org/bot{self.telegram_token}/getUpdates")
        updates = response.json()['result']
        update_ids = [r['update_id'] for r in updates]
        unread_messages = [r['message'] for r in updates if not r['update_id'] in self.read_updates]
        self.read_updates.update(update_ids)
        return unread_messages

    def send_message(self, message):
        for chat_id in self.chat_ids:
            url = f"https://api.telegram.org/bot{self.telegram_token}/sendMessage"
            data = {"chat_id": chat_id, "text": message}
            response = requests.post(url, data=data)
            # print(response.json())