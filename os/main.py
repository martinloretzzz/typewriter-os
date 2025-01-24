from typing import Union

from fastapi import FastAPI
from weather import get_weather

app = FastAPI()


@app.get("/")
async def read_root():
    return {"Hello": "World"}

@app.get("/weather/")
async def read_item():
    return get_weather(long=48.2081, lat=16.3713)


@app.get("/items/{item_id}")
async def read_item(item_id: int, q: Union[str, None] = None):
    return {"item_id": item_id, "q": q}
