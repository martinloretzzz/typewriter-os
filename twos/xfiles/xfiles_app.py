from openai import OpenAI
from twos.ai.ai_app import AIChatApp
from twos.settings import Settings

class XFilesApp:
    def __init__(self, config: Settings, system_prompt=None):
        self.prompt = "Write a 100-word X-Files story featuring Mulder and Scully investigating a mysterious case. Naturally integrate these keywords to drive the investigation's mystery: "
        self.ai_app = AIChatApp(config)

    def __call__(self, params):
        if params is None or params == " ": return ""
        return self.ai_app(self.prompt + params.replace(" ", ", "))
