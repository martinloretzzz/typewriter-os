import random
import time
from twos.ai.ai_app import AIChatApp
from twos.settings import Settings

x_files_keywords = [
    "FBI",
    "X-Files",
    "UFO",
    "Alien",
    "goverment",
    "secret base",
    "feldkirch",
    "schattenburg",
    "hacker",
    "cucumber",
    "Angela Merkel",
    "Christoph Blank",
    "hackathon",
    "DNA",
    "symbols",
    "Area 51",
    "Electron",
    "Uranium-235",
    "Digitale Initiativen",
    "Cryptography",
    "4d chess"
]

class XFilesApp:
    def __init__(self, config: Settings, system_prompt=None):
        self.prompt = "Write a 100-word X-Files story featuring Mulder and Scully investigating a mysterious case. Naturally integrate these keywords to drive the investigation's mystery: "
        self.ai_app = AIChatApp(config)
        self.ghost_mode = config.xfiles_ghost_mode
        self.update_interval = config.xfiles_ghost_interval
        self.next_update_time = self.get_next_update_time()

    def __call__(self, params):
        if params is None or params == " ": return ""
        return self.ai_app(self.prompt + params.replace(" ", ", "))

    def msg(self):
        if time.time() >= self.next_update_time and self.ghost_mode:
            self.next_update_time = self.get_next_update_time()
            keywords = random.sample(x_files_keywords, 3)
            return self.__call__(", ".join(keywords))
        return None
    
    def get_next_update_time(self):
        return time.time() + random.randint(int(0.2 *  self.update_interval), self.update_interval)
