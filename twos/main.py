from typing import Union
from fastapi import FastAPI
from twos.document.document_app import get_document
from twos.ai.ai_app import AIChatApp
from twos.settings import get_settings
from twos.time.time_app import get_time
from twos.weather.weather import get_weather

app = FastAPI()
config = get_settings()

apps = {
    "time": lambda p: get_time(),
    "weather": lambda p: get_weather(long=config.long, lat=config.lat),
    "document": lambda p: get_document(),
}

@app.get("/")
async def read_root():
    return {"Hello": "World"}

@app.get("/command/{command}")
async def run_app(command: str, q: Union[str, None] = None):
    app_name, parameters = command, None
    if " " in command:
        app_name, parameters = command.split(" ", 1)
    return apps[app_name](parameters)
