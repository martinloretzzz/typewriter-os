import requests
from bs4 import BeautifulSoup
from twos.ai.ai_app import AIChatApp
from twos.settings import Settings

class HackernewsApp:
    def __init__(self, config: Settings):
        system_prompt = "Extract the main content from this website and generate a concise summary in one sentences. Focus on key points, removing ads, navigation elements, and irrelevant details."
        self.ai_summarizer = AIChatApp(config, system_prompt=system_prompt)

    def __call__(self, params):
        if params is not None:
            if params.isdigit():
                return self.hn_individual_story(int(params))
        return self.hn_top_stories(count=8 if params == "a" else 4)

    def hn_top_stories(self, count=4):
        top_stories = self.get_top_stories(list(range(count)))
        titles = [f"{i} " + story["title"] for i, story in enumerate(top_stories)]
        return "\n".join(titles)
    
    def hn_individual_story(self, index):
        story = self.get_top_stories([index])[0]
        print(story)
        website_content = self.get_site_text(story["url"]) if "url" in story else story["text"]
        summary = self.ai_summarizer(website_content) if website_content != "" else ""
        url_info = (story["url"]+"\n") if "url" in story else "No Summary"
        return f"{story["title"]} by {story["by"]} {story["score"]}U\n{url_info}{summary}"

    def get_top_stories(self, indices):
        top_ids = requests.get("https://hacker-news.firebaseio.com/v0/topstories.json").json()
        top_stories = []
        for i in indices:
            id = top_ids[i]
            story = requests.get(f"https://hacker-news.firebaseio.com/v0/item/{id}.json").json()
            if story is not None:
                top_stories.append(story)
        return top_stories

    def get_site_text(self, url):
        response = requests.get(url)
        if response.status_code != 200:
            print(f"Failed to fetch: {response.status_code}")
            return ""
        soup = BeautifulSoup(response.text, "html.parser")
        text = soup.get_text()
        return text.strip()
