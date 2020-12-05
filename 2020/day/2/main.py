import sys
from collections.abc import Callable

Policy = tuple[int, int, str]
IsValidPassword = Callable[[str, Policy], bool]

def is_valid_password_part1(password: str, policy: Policy):
    count = 0
    for ch in password:
        if ch == policy[2]:
            count = count + 1
            if count > policy[1]:
                return False
    return count >= policy[0]

def is_valid_password_part2(password: str, policy: Policy):
    count = 0
    index1 = policy[0] - 1
    if index1 < len(password) and password[index1] == policy[2]:
        count = count + 1

    index2 = policy[1] - 1
    if index2 < len(password) and password[index2] == policy[2]:
        count = count + 1
    return count == 1

def main(is_valid_password: IsValidPassword):
    print("Reading from stdin", file=sys.stderr)

    num_of_valid_passwords = 0

    for line in sys.stdin:
        policy_min_ch, _, rest = line.partition("-")
        policy_max_ch, _, rest = rest.partition(" ")
        policy_char, _, rest = rest.partition(":")
        password = rest.strip()

        try:
            policy = (int(policy_min_ch), int(policy_max_ch), policy_char)
        except ValueError:
            continue

        print(policy[0], policy[1], policy[2])

        if is_valid_password(password, policy):
            num_of_valid_passwords = num_of_valid_passwords + 1

    print("Num of valid passwords: ", file=sys.stderr)
    print(num_of_valid_passwords)

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "part1":
        main(is_valid_password_part1)
    elif len(sys.argv) > 1 and sys.argv[1] == "part2":
        main(is_valid_password_part2)
    else:
        print("Expected part1 or part2 as first argument", file=sys.stderr)
        sys.exit(1)
