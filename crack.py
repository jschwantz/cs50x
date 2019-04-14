import sys
import crypt
import string


def main():

    if len(sys.argv) != 2:
        print("Usage: ./python crack.py hash")
        return 1

    shash = sys.argv[1]
    salt = shash[0:2]

    # try the single character words
    for i in "AaIi":
        password = crypt.crypt(i, salt)
        if password == shash:
            return i

    for i in range(1, 5):
        test2 = list('aaaaa')
        testarg = list('aaaaa')
        # index 4
        for j in string.ascii_letters:
            test2[4] = j
            # index 3
            for k in string.ascii_letters:
                test2[3] = k
                # index 2
                for l in string.ascii_letters:
                    test2[2] = l
                    # index 1
                    for m in string.ascii_letters:
                        test2[1] = m
                        # index 0
                        for n in string.ascii_letters:
                            test2[0] = n
                            # if i == 1:
                            # testarg[0] = test2[0]
                            # testarg[1] = test2[1]
                            # else:
                            testarg = test2[0:i+1]

                            # join our list to pass into crypt as a string
                            password = crypt.crypt(''.join(testarg), salt)

                            if password == shash:
                                print(''.join(testarg[0:i+1]))
                                return 0
                        test2[0] = 'a'
                    test2[1] = 'a'
                    if i < 2:
                        break
                test2[2] = 'a'
                if i < 3:
                    break
            test2[3] = 'a'
            if i < 4:
                break

    return 0


if __name__ == "__main__":
    main()