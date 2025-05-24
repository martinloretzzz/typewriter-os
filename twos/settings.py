from functools import lru_cache
from pydantic_settings import BaseSettings, SettingsConfigDict

class Settings(BaseSettings):
    timezone: str
    long: float
    lat: float
    document_path: str
    telegram_token: str
    ai_base_url: str
    ai_api_key: str
    ai_model_name: str
    xfiles_ghost_mode: bool = False
    xfiles_ghost_interval: int = 10 * 60 # s
    model_config = SettingsConfigDict(env_file=".env")

@lru_cache
def get_settings():
    return Settings()
