from datetime import datetime
import pytz
from twos.settings import Settings

class TimeApp:
    def __init__(self, config: Settings):
        self.timezone = config.timezone

    def __call__(self, params):
        current_time = datetime.now(pytz.timezone(self.timezone))
        return f"{str(current_time.hour).zfill(2)}:{str(current_time.minute).zfill(2)}"
