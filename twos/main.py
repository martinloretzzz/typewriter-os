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
from twos.xfiles.xfiles_app import XFilesApp

config = get_settings()

apps = {
    "time": TimeApp(config),
    "weather": WeatherApp(config),
    "document": DocumentApp(config),
    "ai": AIChatApp(config),
    "telegram": TelegramApp(config),
    "tg": TelegramApp(config),
    "hackernews": HackernewsApp(config),
    "hn": HackernewsApp(config),
    "x": XFilesApp(config),
}

class CommandMsg(BaseModel):
    command: str

def format_output(text):
    return "\n".join("\n".join(textwrap.wrap(line, 64, replace_whitespace=False))
                         if line else "" for line in (text + "\n").split("\n"))

def remap_broken_keys(text):
    return text.replace("+", "l").replace("ü", "k")


app = FastAPI()

@app.get("/")
async def read_root():
    return {"Hello": "World"}

@app.post("/command/")
async def run_app(body: CommandMsg):
    command = body.command
    app_name, params = command, None
    if command == "": return "\n"
    if " " in command:
        app_name, params = command.split(" ", 1)
        # l isn't working on the typewriter, so use + as an alternative
        params = remap_broken_keys(params)
    if not app_name in apps:
        return f"na\n"
    print(f"App: {app_name}, Params: {params}")
    response = format_output(apps[app_name](params))
    print(response)
    return response

@app.get("/msg/")
async def run_app():
    output = []
    # telegram_messages = apps["tg"](None)
    # if telegram_messages != "-":
    #     output.append(telegram_messages)
    xfiles_output = apps["x"].msg()
    if xfiles_output is not None:
        output.append(xfiles_output)
    if len(output) == 0: return ""
    return format_output("\n".join(output))
