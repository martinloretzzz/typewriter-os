def get_document():
    with open("./data/document.txt", "r+") as f:
        text = f.read()
    return text
