def main():

    # do a loop here to get them to give an appropriate number
    while True:
        # use error handling with try in case they type in text
        try:
            height = int(input("Height:"))
            if height <= 8 and height >= 1:
                break

        except ValueError:
            continue

    base = "#  #"

    # decrement from the height down
    for i in range(height, 0, -1):
        # start with our base, and pad with spaces to the left to center our blocks
        print('{:>{n}}'.format(base, n=len(base)+i-1))
        # go ahead and set our new base for next time, adding 2 additional blocks
        base = '{:#^{n}}'.format(base, n=len(base)+2)


if __name__ == "__main__":
    main()