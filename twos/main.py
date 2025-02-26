from typing import Union
from fastapi import FastAPI
from pydantic import BaseModel
from twos.document.document_app import get_document
from twos.ai.ai_app import AIChatApp
from twos.settings import get_settings
from twos.telegram.telegram_app import TelegramApp
from twos.time.time_app import get_time
from twos.weather.weather import get_weather

app = FastAPI()
config = get_settings()

class CommandMsg(BaseModel):
    command: str

apps = {
    "time": lambda p: get_time(),
    "weather": lambda p: get_weather(long=config.long, lat=config.lat),
    "document": lambda p: get_document(),
    "ai": AIChatApp(config),
    "telegram": TelegramApp(config)
}

@app.get("/")
async def read_root():
    return {"Hello": "World"}

@app.post("/command/")
async def run_app(body: CommandMsg):
    command = body.command
    app_name, parameters = command, None
    if " " in command:
        app_name, parameters = command.split(" ", 1)
    if not app_name in apps:
        return f"App {app_name} not found.\n"
    return apps[app_name](parameters) + "\n"
