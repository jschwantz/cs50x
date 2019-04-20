from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # breakout lines by return
    aline = a.split('\n')
    bline = b.split('\n')
    comp = []

    # for each line in a, check if it's in b
    # if it is, check if its already in our comparison list before adding
    for line in aline:
        if line in bline:
            if line not in comp:
                comp.append(line)

    return comp


def sentences(a, b):
    """Return sentences in both a and b"""

    asent = sent_tokenize(a, language='english')
    bsent = sent_tokenize(b, language='english')
    comp = []

    for sent in asent:
        if sent in bsent:
            if sent not in comp:
                comp.append(sent)

    return comp


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    asub = []
    bsub = []
    comp = []

    # from start to finish, get all the different combos of length n
    for i in range(0, len(a) - (n-1)):
        asub.append(a[i:i+n])

    for i in range(0, len(b) - (n-1)):
        bsub.append(b[i:i+n])

    for sub in asub:
        if sub in bsub:
            if sub not in comp:
                comp.append(sub)

    return comp
