import unittest
from main import (validate_passport_byr, validate_passport_iyr,
    validate_passport_eyr, validate_passport_hgt, validate_passport_hcl,
    validate_passport_ecl, validate_passport_pid)

class TestValidationMethods(unittest.TestCase):

    def test_validate_passport_byr_valid(self):
        self.assertTrue(validate_passport_byr({ 'byr': '2002' }))

    def test_validate_passport_byr_invalid(self):
        self.assertFalse(validate_passport_byr({ 'byr': '2003' }))

    def test_validate_passport_hgt_valid_in(self):
        self.assertTrue(validate_passport_hgt({ 'hgt': '60in' }))

    def test_validate_passport_hgt_valid_cm(self):
        self.assertTrue(validate_passport_hgt({ 'hgt': '190cm' }))

    def test_validate_passport_hgt_invalid_in(self):
        self.assertFalse(validate_passport_hgt({ 'hgt': '190in' }))

    def test_validate_passport_hgt_invalid(self):
        self.assertFalse(validate_passport_hgt({ 'hgt': '190' }))

    def test_validate_passport_hcl_valid(self):
        self.assertTrue(validate_passport_hcl({ 'hcl': '#123abc' }))

    def test_validate_passport_hcl_invalid_char(self):
        self.assertFalse(validate_passport_hcl({ 'hcl': '#123abz' }))

    def test_validate_passport_hcl_invalid_missing_square(self):
        self.assertFalse(validate_passport_hcl({ 'hcl': '123abc' }))

    def test_validate_passport_ecl_valid(self):
        self.assertTrue(validate_passport_ecl({ 'ecl': 'brn' }))

    def test_validate_passport_ecl_invalid(self):
        self.assertFalse(validate_passport_ecl({ 'ecl': 'wat' }))

    def test_validate_passport_pid_valid(self):
        self.assertTrue(validate_passport_pid({ 'pid': '000000001' }))

    def test_validate_passport_pid_invalid(self):
        self.assertFalse(validate_passport_pid({ 'pid': '0123456789' }))

if __name__ == '__main__':
    unittest.main()
