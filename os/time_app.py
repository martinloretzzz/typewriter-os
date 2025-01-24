from datetime import datetime
import pytz

def get_time(timezone='Europe/Vienna'):
    current_time = datetime.now(pytz.timezone(timezone))
    return f"{current_time.hour}:{current_time.minute}"
