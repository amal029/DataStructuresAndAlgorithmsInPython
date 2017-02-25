#!/usr/bin/env python

import enchant


def permute(first, rest, d, anagrams):
    frest = first + rest
    while True:
        for i in range(len(rest)-1):
            rest = rest[0:i] + rest[i+1] + rest[i] + rest[i+2:len(rest)]
            rsplit = (first+rest).strip().split(' ')
            ts = [d.check(w) for w in rsplit if w != '']
            if all(ts):
                anagrams += [first+rest]
        if (first+rest) == frest:
            break


def main(s):
    d = enchant.Dict('en_US')
    anagrams = []
    for i in range(len(s)):
        if i == 0:
            rs = s[i+1:len(s)]
        elif i == len(s)-1:
            rs = s[0:i]
        else:
            rs = s[0:i] + s[i+1:len(s)]
        permute(s[i], rs, d, anagrams)
    print(list(set(anagrams)))


if __name__ == '__main__':
    main('Hello World')
    main('MANY VOTED BUSH RETIRED')
    main('RAM')
