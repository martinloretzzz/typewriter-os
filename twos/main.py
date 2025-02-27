from fastapi import FastAPI
from pydantic import BaseModel
import textwrap
from twos.document.document_app import DocumentApp
from twos.ai.ai_app import AIChatApp
from twos.hackernews.hackernews_app import HackernewsApp
from twos.settings import get_settings
from twos.telegram.telegram_app import TelegramApp
from twos.time.time_app import TimeApp
from twos.weather.weather_app import WeatherApp

config = get_settings()

apps = {
    "time": TimeApp(config),
    "weather": WeatherApp(config),
    "document": DocumentApp(config),
    "ai": AIChatApp(config),
    "telegram": TelegramApp(config),
    "tg": TelegramApp(config),
    "hackernews": HackernewsApp(config),
    "hn": HackernewsApp(config)
}

class CommandMsg(BaseModel):
    command: str

def format_output(text):
    return "\n".join("\n".join(textwrap.wrap(line, 64, replace_whitespace=False))
                         if line else "" for line in (text + "\n").split("\n"))

app = FastAPI()

@app.get("/")
async def read_root():
    return {"Hello": "World"}

@app.post("/command/")
async def run_app(body: CommandMsg):
    command = body.command
    app_name, params = command, None
    if " " in command:
        app_name, params = command.split(" ", 1)
    if not app_name in apps:
        return f"na\n"
    print(f"App: {app_name}, Params: {params}")
    response = format_output(apps[app_name](params))
    print(response)
    return response
