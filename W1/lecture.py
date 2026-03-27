
"""
Gusfield'z Z Algorithm

Preprocessing algorithm

Output: outputs output = [Zi-values] where len(output) = len(input)
Z[i] = length of the longest substring starting at position i, where the substring is some prefix in str[1..i]


at some interation i, define l, r

keep track of right most end point of a z-boxes at or before i
define l, the start point of the z box defined at r


endpoint = startpoint + L(substring) - 1
Base case
compute z2 by doing explicit left-to-right comparison of characters str[2...] and str[1...]

set Z2 > 0:
    set r to 2 + z2-1
    set l to 2
else:
    set both l and r to 0

    
Iterative case:
    Case 1:
        k > r 
    Case 2:
        K <= R
        K exists within the Zl-box


        Then that means that it is within the z box described by [l,r], where str[l,r] describes the prefix [1, r-l+1]
        Since it is within that box, we can define its symmetry witht he prefix [1,r-l+1] to say that k is the same as k-l+1
        Thus, z[k] = |Z[k-l+1]|

        special case:
            when z[k-l+1] > tje bounding z box of where k is, then u bound the z[k-l+1] box, and then extend the box until mismatch
"""


def gusfield_z(input: str) -> list[str]:
    output = [0] * len(str)
    l = 1
    r = 1
    mismatch = False
    i = 0
 

        
