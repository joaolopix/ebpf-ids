
import ctypes

max_tree_depth = 3

class TreeNode(ctypes.Structure):
    _fields_ = [("feature", ctypes.c_int),
                ("value", ctypes.c_int),
                ("left", ctypes.c_int),
                ("right", ctypes.c_int)]
rf_nodes = [
	TreeNode(4, 1161660, 1, 2),
	TreeNode(4, 1111165, 3, 4),
	TreeNode(4, 1211115, 7, 8),
	TreeNode(4, 1111115, 5, 6),
	TreeNode(4, 1111660, 10, 10),
	TreeNode(-1, 1, -1, -1),
	TreeNode(-1, 2, -1, -1),
	TreeNode(2, 15, 9, 8),
	TreeNode(-1, 0, -1, -1),
	TreeNode(-1, 4, -1, -1),
	TreeNode(-1, 3, -1, -1),
	TreeNode(4, 1161660, 12, 13),
	TreeNode(3, 560, 14, 15),
	TreeNode(4, 1211115, 16, 8),
	TreeNode(3, 515, 5, 6),
	TreeNode(3, 1125, 10, 8),
	TreeNode(0, 5000, 9, 8),
	TreeNode(1, 115, 18, 19),
	TreeNode(2, 15, 20, 21),
	TreeNode(2, 15, 22, 8),
	TreeNode(0, 5000, 9, 8),
	TreeNode(4, 1161660, 10, 8),
	TreeNode(3, 675, 5, 8),
	TreeNode(4, 1161660, 24, 25),
	TreeNode(4, 1111165, 26, 27),
	TreeNode(2, 15, 28, 8),
	TreeNode(1, 115, 6, 5),
	TreeNode(3, 560, 8, 10),
	TreeNode(0, 5000, 9, 8),
	TreeNode(4, 1161660, 30, 31),
	TreeNode(2, 15, 33, 34),
	TreeNode(3, 545, 32, 8),
	TreeNode(4, 1211115, 9, 8),
	TreeNode(4, 1111165, 6, 10),
	TreeNode(1, 115, 10, 8),
	TreeNode(4, 1161660, 36, 37),
	TreeNode(3, 560, 38, 39),
	TreeNode(3, 545, 40, 8),
	TreeNode(3, 515, 5, 6),
	TreeNode(1, 115, 10, 8),
	TreeNode(4, 1211115, 9, 8),
	TreeNode(1, 115, 42, 43),
	TreeNode(4, 1161660, 44, 45),
	TreeNode(3, 675, 46, 47),
	TreeNode(3, 560, 6, 10),
	TreeNode(2, 15, 9, 8),
	TreeNode(3, 640, 5, 5),
	TreeNode(2, 15, 8, 8),
	TreeNode(4, 1161660, 49, 50),
	TreeNode(2, 15, 52, 53),
	TreeNode(3, 545, 51, 8),
	TreeNode(0, 10000, 9, 8),
	TreeNode(4, 1111165, 6, 10),
	TreeNode(3, 1200, 10, 8),
	TreeNode(4, 1161660, 55, 56),
	TreeNode(3, 560, 57, 58),
	TreeNode(4, 1211115, 59, 8),
	TreeNode(4, 1111115, 5, 6),
	TreeNode(2, 15, 10, 8),
	TreeNode(3, 545, 9, 8),
	TreeNode(3, 545, 61, 62),
	TreeNode(0, 5000, 65, 8),
	TreeNode(3, 600, 63, 64),
	TreeNode(4, 1161160, 10, 8),
	TreeNode(0, 115000, 8, 8),
	TreeNode(3, 515, 5, 9),
	TreeNode(4, 1161660, 67, 68),
	TreeNode(3, 560, 69, 70),
	TreeNode(0, 5000, 71, 8),
	TreeNode(3, 515, 5, 6),
	TreeNode(4, 1111160, 8, 10),
	TreeNode(2, 15, 9, 8),
]
rf_tree_roots = [0,11,17,23,29,35,41,48,54,60,66]
