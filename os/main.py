from typing import Union

from fastapi import FastAPI
from document_app import get_document
from settings import get_settings
from time_app import get_time
from weather import get_weather

app = FastAPI()
config = get_settings()

@app.get("/")
async def read_root():
    return {"Hello": "World"}

apps = {
    "time": lambda: get_time(),
    "weather": lambda: get_weather(long=config.long, lat=config.lat),
}

@app.get("/command/{app_name}")
async def run_app(app_name: str, q: Union[str, None] = None):
    return apps[app_name]()
