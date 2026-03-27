from typing import Callable


def gusfield_z(string: str) -> list[int]:
    output = [0] * len(string)
    l = 0
    r = 0
    output[0] = len(string)
    if len(string) < 2: return output
    k = 1
    i = 1
    while k < len(string) and string[k] == string[k-1]:
        output[1] += 1
        k += 1
    if output[1] > 0:
        l = 1
        r = output[1]
    i += 1
    while i < len(string):
        if i > r:
            k = i
            while k < len(string) and string[k] == string[k-i]:
                output[i] += 1
                k += 1
            if output[i] > 0:
                r = output[i] - 1 + i
                l = i
            i += 1
        # i is contained within an existing z box
        else:
            if output[i-l] < r-i:
                output[i] = output[i-l]
            else:
                k = r + 1
                while k < len(string) and string[k] == string[k-i]:
                    output[i] += 1
                    k += 1
                output[i] += r-i + 1
                l = i
                r = output[i] - 1 + i
            i += 1
    return output


def gusfield_linear_pattern_match(pattern: str, string: str):
    input_string = pattern + "$" + string
    z_array = gusfield_z(input_string)
    return len(pattern) in z_array

def question_7(T: str, S: str):
    concat = T + "$" + S
    z_array = gusfield_z(concat)
    mx = 0
    for i in range(len(T) + 1, len(concat)):
        if z_array[i] + i == len(concat):
            mx = max(mx, z_array[i])
    return mx



if __name__ == "__main__":
    test_string = "yahyaiscool"
    pattern = "isc"
    print(gusfield_linear_pattern_match(pattern, test_string))