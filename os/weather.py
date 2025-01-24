import requests


def get_weather(long, lat):
    params = {
        "latitude": long,
        "longitude": lat,
        "forecast_days": 1,
        "current": ["temperature_2m", "precipitation", "weather_code"],
        "daily": ["weather_code", "temperature_2m_max", "temperature_2m_min", "precipitation_sum"],
    }
    r = requests.get(f"https://api.open-meteo.com/v1/forecast", params=params)
    weather = r.json()

    temp_unit = weather['current_units']['temperature_2m']
    rain_unit = weather['current_units']['precipitation']

    weather_desc_now = wmo_codes[weather['current']['weather_code']]
    weather_desc_today = wmo_codes[weather["daily"]['weather_code'][0]]
    rain = 2 # weather["daily"]['precipitation_sum'][0]

    return f"{weather_desc_now} {round(weather['current']['temperature_2m'])}{temp_unit} " + \
            f"{round(weather["daily"]['temperature_2m_max'][0])}/{round(weather["daily"]['temperature_2m_min'][0])}{temp_unit}" + \
            (f" {rain}{rain_unit}" if rain > 0 else "")

wmo_codes = {
    0: "Sunny",
    1: "Mainly Sunny",
    2: "Partly Cloudy",
    3: "Cloudy",
    45: "Foggy",
    48: "Rime Fog",
    51: "Light Drizzle",
    53: "Drizzle",
    55: "Heavy Drizzle",
    56: "Light Freezing Drizzle",
    57: "Freezing Drizzle",
    61: "Light Rain",
    63: "Rain",
    65: "Heavy Rain",
    66: "Light Freezing Rain",
    67: "Freezing Rain",
    71: "Light Snow",
    73: "Snow",
    75: "Heavy Snow",
    77: "Snow Grains",
    80: "Light Showers",
    81: "Showers",
    82: "Heavy Showers",
    85: "Light Snow Showers",
    86: "Snow Showers",
    95: "Thunderstorm",
    96: "Light Thunderstorms With Hail",
    99: "Thunderstorm With Hail"
}
