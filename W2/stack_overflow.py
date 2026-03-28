from random import randrange


def random_list():
    return [0] * randrange(10, 500)


#  dp[k] = 1 + dp[1:]
def mystery_list_counter(lst):
    if lst == []:
        return 0

    return 1 + mystery_list_counter(lst[1:])

if __name__ == "__main__":
    k : list = random_list()
    # length of list[0...k] = 1 + list[1...k]
    print(mystery_list_counter(k))
    print(len(k))

    