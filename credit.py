def main():

    # do a loop here to get them to give an appropriate number
    while True:
        # use error handling with try in case they type in text
        try:
            # Confirm its a number up front, let error handling catch non-integers
            cardnum = int(input("Number:"))
            # only strings have the len method
            if len(str(cardnum)) not in (13, 15, 16):
                print("INVALID")
                return
            elif len(str(cardnum)) in (13, 15, 16):
                break

        except ValueError:
            continue

    # store this to save some typing
    scardnum = str(cardnum)
    length = len(scardnum)
    sums = 0

    # increment down from second to last digit back
    for i in range(length - 1, 0, -2):
        x = int(scardnum[i-1])
        if 2*x > 9:
            # can assume the first digit is 1 here..
            sums += (2*x % 10) + 1
        else:
            sums += 2*x

    for i in range(length, 0, -2):
        x = int(scardnum[i-1])
        sums += x

    # card validation checks
    if sums % 10 == 0:
        if length == 15 and int(scardnum[0]) == 3 and int(scardnum[1]) in (4, 7):
            print("AMEX")
        elif length in (16, 13) and int(scardnum[0]) == 4:
            print("VISA")
        elif length == 16 and int(scardnum[0]) == 5 and int(scardnum[1]) in (1, 2, 3, 4, 5):
            print("MASTERCARD")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()