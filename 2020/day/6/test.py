import unittest
from main import sum_of_questions_per_group, MODE_ANYONE, MODE_EVERYONE

class Tests(unittest.TestCase):
    def test_sum_of_questions_per_group_anyone(self):
        with open('./test_input1.txt') as f:
            res = sum_of_questions_per_group(f, MODE_ANYONE)

        self.assertEqual(res, 11)

    def test_sum_of_questions_per_group_everyone(self):
        with open('./test_input1.txt') as f:
            res = sum_of_questions_per_group(f, MODE_EVERYONE)

        self.assertEqual(res, 6)

if __name__ == '__main__':
    unittest.main()
