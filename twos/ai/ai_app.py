from openai import OpenAI
from twos.settings import Settings

class AIChatApp:
    def __init__(self, config: Settings):
        self.model_name = config.ai_model_name
        self.client = OpenAI(api_key=config.ai_api_key, base_url=config.ai_base_url)
        self.system_prompt = """You are a helpfull assistand running on a mecahnical typewriter.
Output only in plain text, without any markdown or images.
Make a short response."""

    def __call__(self, params):
        if params is None or params == " ": return ""
        completion = self.client.chat.completions.create(
            model=self.model_name,
            messages=[
                {"role": "system", "content": self.system_prompt},
                {"role": "user", "content": params},
            ],
            stream=False,
            temperature=0
        )
        print(completion.choices[0].message)
        return completion.choices[0].message.content
