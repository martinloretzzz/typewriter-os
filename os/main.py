from typing import Union

from fastapi import FastAPI
from time_app import get_time
from weather import get_weather

app = FastAPI()


@app.get("/")
async def read_root():
    return {"Hello": "World"}

apps = {
    "time": lambda: get_time(),
    "weather": lambda: get_weather(long=48.2081, lat=16.3713)
}

@app.get("/command/{app_name}")
async def run_app(app_name: str, q: Union[str, None] = None):
    return apps[app_name]()
