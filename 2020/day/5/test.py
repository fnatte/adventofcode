import unittest
from main import get_pos_from_bsp_code, get_seat_id_from_row_col

class Tests(unittest.TestCase):
    def test_get_pos_from_bsp_code_1(self):
        (row, col) = get_pos_from_bsp_code("BFFFBBFRRR")
        self.assertEqual(row, 70)
        self.assertEqual(col, 7)

    def test_get_pos_from_bsp_code_2(self):
        (row, col) = get_pos_from_bsp_code("FFFBBBFRRR")
        self.assertEqual(row, 14)
        self.assertEqual(col, 7)

    def test_get_pos_from_bsp_code_3(self):
        (row, col) = get_pos_from_bsp_code("BBFFBBFRLL")
        self.assertEqual(row, 102)
        self.assertEqual(col, 4)

    def test_get_seat_id_from_row_col_1(self):
        id = get_seat_id_from_row_col(70, 7)
        self.assertEqual(id, 567)

    def test_get_seat_id_from_row_col_2(self):
        id = get_seat_id_from_row_col(14, 7)
        self.assertEqual(id, 119)

    def test_get_seat_id_from_row_col_3(self):
        id = get_seat_id_from_row_col(102, 4)
        self.assertEqual(id, 820)

if __name__ == '__main__':
    unittest.main()
