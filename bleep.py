import sys


def main():

    if len(sys.argv) != 2:
        print("Usage: ./python bleep.py dictionary")
        sys.exit(1)

    words = set()

    message = input("What message wouuld you like to censor?\n")

    # Load dictionary into memory
    file = open(sys.argv[1], "r")
    for line in file:
        words.add(line.rstrip("\n").lower())
    file.close()

    sensor = ''

    # split out message into individual words to check
    for text in message.split():
        if text.lower() in words:
            # replace word with asterisks
            sensor += '{:*<{n}}'.format("", n=len(text)) + " "
        else:
            sensor += text + " "

    # strip off any trailing spaces
    print(sensor.strip())
    return 0


if __name__ == "__main__":
    main()
