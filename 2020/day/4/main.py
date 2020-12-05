import sys
import re

p = re.compile('(\w+):([^\s]+)')
hgt_p = re.compile('^(\d+)(cm|in)$')
hcl_p = re.compile('^#[0-9a-f]{6}$')
pid_p = re.compile('^\d{9}$')

required_passport_fields = [
    "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
]

def is_int_between(input, min, max):
    try:
        val = int(input)
        return val >= min and val <= max
    except ValueError:
        return False

def validate_passport_required_fields(obj):
    for field in required_passport_fields:
        if not field in obj:
            return False
    return True

def validate_passport_byr(obj):
    return is_int_between(obj['byr'], 1920, 2002)

def validate_passport_iyr(obj):
    return is_int_between(obj['iyr'], 2010, 2020)

def validate_passport_eyr(obj):
    return is_int_between(obj['eyr'], 2020, 2030)

def validate_passport_hgt(obj):
    m = hgt_p.match(obj['hgt'])
    if m:
        val, unit = m.group(1, 2)
        if unit == 'cm':
            return is_int_between(val, 150, 193)
        if unit == 'in':
            return is_int_between(val, 59, 76)
    return False

def validate_passport_hcl(obj):
    return hcl_p.match(obj['hcl']) is not None

def validate_passport_ecl(obj):
    return obj['ecl'] in ['amb', 'blu', 'brn', 'gry', 'grn', 'hzl', 'oth']

def validate_passport_pid(obj):
    return pid_p.match(obj['pid']) is not None


def validate_passport_part1(obj):
    return validate_passport_required_fields(obj)

def validate_passport_part2(obj):
    return (
        validate_passport_required_fields(obj) and
        validate_passport_byr(obj) and
        validate_passport_iyr(obj) and
        validate_passport_eyr(obj) and
        validate_passport_hgt(obj) and
        validate_passport_hcl(obj) and
        validate_passport_ecl(obj) and
        validate_passport_pid(obj)
    )

def main(validate_passport):
    num_of_valid_passports = 0
    obj = {}
    for line in sys.stdin:
        if not line.strip():
            if validate_passport(obj):
                num_of_valid_passports = num_of_valid_passports + 1
            obj = {}
            continue

        for m in p.finditer(line):
            (grp, val) = m.group(1, 2)
            obj[grp] = val

    print("Valid passports:", file=sys.stderr)
    print(num_of_valid_passports)

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == "part1":
        main(validate_passport_part1)
    elif len(sys.argv) > 1 and sys.argv[1] == "part2":
        main(validate_passport_part2)
    else:
        print("Expected part1 or part2 as first argument", file=sys.stderr)
        sys.exit(1)
