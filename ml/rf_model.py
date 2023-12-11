
import ctypes

max_tree_depth = 3

class TreeNode(ctypes.Structure):
    _fields_ = [("feature", ctypes.c_int),
                ("value", ctypes.c_int),
                ("left", ctypes.c_int),
                ("right", ctypes.c_int)]
rf_nodes = [
	TreeNode(3, 630, 1, 2),
	TreeNode(1, 115, 3, 4),
	TreeNode(3, 1125, 6, 7),
	TreeNode(3, 575, 5, 5),
	TreeNode(3, 555, 5, 5),
	TreeNode(-1, 0, -1, -1),
	TreeNode(0, 5000, 8, 8),
	TreeNode(1, 115, 8, 8),
	TreeNode(-1, 1, -1, -1),
	TreeNode(3, 630, 10, 11),
	TreeNode(0, 5000, 13, 8),
	TreeNode(4, 1161660, 12, 8),
	TreeNode(3, 1125, 8, 8),
	TreeNode(3, 515, 5, 5),
	TreeNode(4, 1111660, 15, 16),
	TreeNode(2, 15, 17, 8),
	TreeNode(2, 15, 18, 19),
	TreeNode(3, 630, 5, 8),
	TreeNode(4, 1211115, 8, 8),
	TreeNode(2, 25, 8, 8),
	TreeNode(3, 630, 21, 22),
	TreeNode(3, 515, 5, 23),
	TreeNode(4, 1161660, 24, 8),
	TreeNode(4, 1111660, 5, 5),
	TreeNode(1, 115, 8, 8),
	TreeNode(3, 630, 26, 27),
	TreeNode(1, 115, 28, 29),
	TreeNode(4, 1161660, 30, 8),
	TreeNode(4, 1111660, 5, 5),
	TreeNode(3, 555, 5, 5),
	TreeNode(4, 1112160, 8, 5),
	TreeNode(4, 1111660, 32, 33),
	TreeNode(3, 675, 34, 35),
	TreeNode(4, 1211115, 36, 8),
	TreeNode(1, 115, 5, 5),
	TreeNode(3, 1065, 8, 8),
	TreeNode(0, 5000, 8, 5),
	TreeNode(3, 630, 38, 39),
	TreeNode(0, 5000, 42, 8),
	TreeNode(1, 115, 40, 41),
	TreeNode(0, 5000, 8, 8),
	TreeNode(2, 15, 8, 8),
	TreeNode(3, 515, 5, 5),
	TreeNode(0, 5000, 44, 45),
	TreeNode(2, 15, 46, 47),
	TreeNode(3, 1140, 48, 8),
	TreeNode(1, 115, 5, 5),
	TreeNode(3, 1160, 8, 8),
	TreeNode(3, 1110, 8, 5),
	TreeNode(2, 15, 50, 51),
	TreeNode(1, 115, 52, 53),
	TreeNode(3, 1140, 54, 8),
	TreeNode(4, 1111660, 5, 8),
	TreeNode(0, 10000, 5, 8),
	TreeNode(4, 1161660, 5, 8),
	TreeNode(4, 1111660, 56, 57),
	TreeNode(2, 15, 60, 8),
	TreeNode(3, 545, 58, 59),
	TreeNode(4, 1211115, 5, 8),
	TreeNode(0, 5000, 8, 8),
	TreeNode(0, 10000, 5, 8),
	TreeNode(2, 15, 62, 63),
	TreeNode(3, 640, 64, 65),
	TreeNode(3, 1140, 66, 8),
	TreeNode(4, 1111660, 5, 5),
	TreeNode(1, 115, 8, 8),
	TreeNode(3, 1110, 8, 5),
]
rf_tree_roots = [0,9,14,20,25,31,37,43,49,55,61]
