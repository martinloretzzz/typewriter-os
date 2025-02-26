from functools import lru_cache
from pydantic_settings import BaseSettings, SettingsConfigDict

class Settings(BaseSettings):
    long: float
    lat: float
    telegram_token: str
    ai_base_url: str
    ai_api_key: str
    ai_model_name: str
    model_config = SettingsConfigDict(env_file=".env")

@lru_cache
def get_settings():
    return Settings()
