from datetime import datetime
import pytz

def get_time(timezone='Europe/Vienna'):
    current_time = datetime.now(pytz.timezone(timezone))
    return f"{str(current_time.hour).zfill(2)}:{str(current_time.minute).zfill(2)}"
