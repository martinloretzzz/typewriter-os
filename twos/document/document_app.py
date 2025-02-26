from twos.settings import Settings

class DocumentApp:
    def __init__(self, config: Settings):
        self.document_path = config.document_path

    def __call__(self, params):
        with open(self.document_path, "r+") as f:
            text = f.read()
        return text
