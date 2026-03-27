from W1.gusfield_z import gusfield_z

"""

Boyer Moore:
right to left scanning until mismatch (like in naive, but right to left)
Start at end of pattern to mismatch:
if x in tdxt is a mismatch then call it a bad character in the iteration
let k be the index of the iteration. then pat[k+1...m] = good suffix

big idea
1 (perprocess) - create data scturues
    - 1 for bad character shifting
    - 2 good suffix shifting
    - 3 match prefix shifting
2 search phase:
align pat[1...m] to txt[1...n]
1 if matched use 3
if bad, use best of (1, 2)

BC dt:
 for R(x) = rightmost appearance/index of letter x in pat
 if mismatch found at letter x in txt:
    check R(x)
    if R(x) > K : naive shift shift by 1
    else: shift by k- R(x) : basically shift such that. x in R(X is undernearth the mismatched
    )

"""


"""
R[i][x] = right_most appearance of x left of index i
R[i][x] = R[i-1][x] if str[i-1] != x else i-1

"""

class Node:
    def __init__(self, item = None):
        self.item = item
        self.next = None

class Linked_List:
    start: Node
    def __init__(self, items)


class bad_suffix_optimised:

    last_rightmost_pointer: list
    bad_suffix_table: list
    def __init__(string, alphabet):
        for i in range(len(string)):
            





def extended_bad_character_processing(string, alphabet: list[chr]) -> list[list[int]]:
    res =[[-1 for _ in range(len(alphabet))] for _ in range(len(string))]
    for i in range(1, len(string)):
        for j in range(len(alphabet)):
            res[i][j] = max(res[i-1][j], i-1 if string[i-1] == alphabet[j] else -1 )
    return res

def preprocess_goodsuffix(string) -> list[int]:
    res = [-1 for _ in range(len(string) + 1)]
    z_suffix = gusfield_z(''.join(reversed(string)))
    z_suffix.reverse()
    for i in range(len(string)):
        if z_suffix[i] > 0:
            j = len(string) - z_suffix[i] 
            res[j] = i
    for i in range(len(string-1), -1, -1):
        if str[i] != str[len(string)- 1]:
            res[-1] = i
    return res

def preprocess_mp(string) -> list[int]:
    res = [-1 for i in range (len(string) + 1)]
    z_suffix = gusfield_z(''.join(reversed(string)))
    z_suffix.reverse()
    for i in range(len(res) - 1):
        res[i] = z_suffix[i] if z_suffix[i] == i + 1 else -1
    res[-1] = 0
    return res

"""
Assume english alphabet
"""

def booyer_more(string, pattern) ->list[int]:
    extended_bad_character = extended_bad_character_processing(pattern)
    good_suffix = preprocess_goodsuffix(pattern)
    match_prefix = preprocess_mp(pattern)
    k = len(pattern) - 1
    while k < len(string):
        i = len(pattern) - 1
        j = k
        while string[j] == pattern[i] and k >= 0:
            i -= 1
            j -= 1
        if i < 0:
        else:
            bc_shift = i - extended_bad_character[i][ord(string[j].lower()) - 97]
            suffix_shift = max(good_suffix[i+1], match_prefix[i])
            k = k + max(bc_shift, suffix_shift)


if __name__ == "__main__":
    alphabet = [chr(i) for i in range(ord('a'), ord('z') + 1)]
    res = extended_bad_character_processing("tbapxab", alphabet)
    for i in res:
        print(i)