"""
Implicit suffix tree:
tree without teerminal character

$ constraints that no.leaves = string length
Suffix tree constructed on a prefix of the substring
becomes explicity when the prefix == len(string)


For a string[1..n]


ImplicitySTi = implicit tree of all suffixes of str[1..i]

ImplicitSti+1 is built using info from ImplicitSTi

How:
    Basically traverse from 1..i-1 to i, then extend


Rule 1:
from some 1 < j < i, extend a leaf node when the path from j -> i has i as a leaf node. Occurs when alpha is not observed in the previous search/string
Rule 2:
    When creating a new leaf, the leaf points to the phase number 
Rule 2 regular:
For some 1 <= j <= i, the substring [j..i] has been observed before. therefore, we create a new inernal node, and create a new branch for i+1
Rule 3:
    alpha[str[i+1]] alr exist



Suffix Links
    When traversing some path j -> i, you wouldve reached some node u where u apply a rule. then the remaning substring is k->i
    Let alpha' = j..u
    suppose there was a path from root -> v where dist = [j+1...k-1]
"""

from typing import TypeVar as T

class node:
    def __init__(self, start: int = 0, end: int = 0):
        self.start = start
        self.end = end
        self.children = []

    


class suffix_tree:

    def __init__(self, string: str):
