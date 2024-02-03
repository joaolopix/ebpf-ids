
import ctypes

max_tree_depth = 3

class TreeNode(ctypes.Structure):
    _fields_ = [("feature", ctypes.c_int),
                ("value", ctypes.c_int),
                ("left", ctypes.c_int),
                ("right", ctypes.c_int)]
rf_nodes = [
	TreeNode(3, 630, 1, 2),
	TreeNode(0, 5000, 6, 5),
	TreeNode(3, 1125, 3, 4),
	TreeNode(4, 1161660, 5, 5),
	TreeNode(2, 15, 5, 5),
	TreeNode(-1, 1, -1, -1),
	TreeNode(1, 115, 7, 7),
	TreeNode(-1, 0, -1, -1),
	TreeNode(3, 630, 9, 10),
	TreeNode(0, 5000, 12, 5),
	TreeNode(4, 1161660, 11, 5),
	TreeNode(3, 1125, 5, 5),
	TreeNode(3, 515, 7, 7),
	TreeNode(4, 1111660, 14, 15),
	TreeNode(2, 15, 16, 5),
	TreeNode(2, 15, 17, 18),
	TreeNode(3, 630, 7, 5),
	TreeNode(4, 1211115, 5, 5),
	TreeNode(2, 25, 5, 5),
	TreeNode(3, 635, 20, 21),
	TreeNode(3, 515, 7, 22),
	TreeNode(4, 1161660, 23, 5),
	TreeNode(4, 1111660, 7, 7),
	TreeNode(1, 115, 5, 5),
	TreeNode(3, 630, 25, 26),
	TreeNode(1, 115, 27, 28),
	TreeNode(4, 1161660, 29, 5),
	TreeNode(4, 1111660, 7, 7),
	TreeNode(3, 555, 7, 7),
	TreeNode(4, 1112160, 5, 7),
	TreeNode(4, 1111660, 31, 32),
	TreeNode(3, 665, 33, 34),
	TreeNode(4, 1211115, 35, 5),
	TreeNode(1, 115, 7, 7),
	TreeNode(3, 1065, 5, 5),
	TreeNode(0, 15000, 5, 7),
	TreeNode(3, 635, 37, 38),
	TreeNode(0, 5000, 41, 5),
	TreeNode(1, 115, 39, 40),
	TreeNode(0, 5000, 5, 5),
	TreeNode(2, 15, 5, 5),
	TreeNode(3, 515, 7, 7),
	TreeNode(0, 5000, 43, 44),
	TreeNode(2, 15, 45, 46),
	TreeNode(3, 1140, 47, 5),
	TreeNode(1, 115, 7, 7),
	TreeNode(3, 1160, 5, 5),
	TreeNode(3, 1110, 5, 7),
	TreeNode(2, 15, 49, 50),
	TreeNode(1, 115, 51, 52),
	TreeNode(3, 1140, 53, 5),
	TreeNode(4, 1111660, 7, 5),
	TreeNode(0, 10000, 7, 5),
	TreeNode(4, 1161660, 7, 5),
	TreeNode(4, 1111660, 55, 56),
	TreeNode(2, 15, 59, 5),
	TreeNode(3, 545, 57, 58),
	TreeNode(4, 1211115, 7, 5),
	TreeNode(0, 5000, 5, 5),
	TreeNode(0, 10000, 7, 5),
	TreeNode(2, 15, 61, 62),
	TreeNode(3, 635, 63, 64),
	TreeNode(3, 1140, 65, 5),
	TreeNode(4, 1111660, 7, 7),
	TreeNode(1, 115, 5, 5),
	TreeNode(3, 1110, 5, 7),
]
rf_tree_roots = [0,8,13,19,24,30,36,42,48,54,60]
