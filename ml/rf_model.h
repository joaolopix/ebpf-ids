
#define MAX_TREE_DEPTH 3

typedef struct tree_node {
    int feature;
    int value;
    int left;
    int right;
} tree_node;

static __always_inline int predict_tree(int current_node, int* features) {
	tree_node rf_nodes[68] = {
	  { 1, 15, 1, 2 },
	  { 2, 55, 6, 7 },
	  { 4, 515, 3, 4 },
	  { 2, 4545, 8, 8 },
	  { 3, 15, 5, 5 },
	  { -1, 1, -1, -1 },
	  { 4, 3025, 5, 5 },
	  { 2, 105, 8, 8 },
	  { -1, 0, -1, -1 },
	  { 4, 515, 10, 11 },
	  { 4, 430, 14, 8 },
	  { 3, 15, 12, 13 },
	  { 2, 327855, 5, 5 },
	  { 5, 1161660, 5, 5 },
	  { 2, 4545, 8, 8 },
	  { 2, 245, 16, 17 },
	  { 5, 1216110, 20, 5 },
	  { 0, 5000, 18, 19 },
	  { 4, 515, 8, 5 },
	  { 4, 1130, 5, 5 },
	  { 2, 35, 5, 8 },
	  { 2, 245, 22, 23 },
	  { 2, 35, 26, 27 },
	  { 5, 1111660, 24, 25 },
	  { 5, 1111115, 5, 8 },
	  { 3, 25, 5, 5 },
	  { 4, 700, 8, 5 },
	  { 5, 1216110, 8, 5 },
	  { 4, 515, 29, 30 },
	  { 4, 430, 33, 8 },
	  { 2, 245, 31, 32 },
	  { 4, 920, 8, 5 },
	  { 4, 600, 5, 5 },
	  { 1, 90, 8, 8 },
	  { 4, 515, 35, 36 },
	  { 1, 90, 8, 39 },
	  { 5, 1111660, 37, 38 },
	  { 1, 115, 8, 5 },
	  { 3, 15, 5, 5 },
	  { 4, 430, 8, 8 },
	  { 5, 1111660, 41, 42 },
	  { 5, 1111115, 43, 44 },
	  { 0, 85000, 45, 46 },
	  { 2, 510, 8, 5 },
	  { 2, 4435, 5, 8 },
	  { 2, 795, 8, 5 },
	  { 5, 1161660, 5, 5 },
	  { 2, 245, 48, 49 },
	  { 0, 5000, 52, 53 },
	  { 4, 515, 50, 51 },
	  { 4, 430, 8, 8 },
	  { 2, 327855, 5, 5 },
	  { 4, 660, 8, 5 },
	  { 3, 25, 8, 5 },
	  { 5, 1111660, 55, 56 },
	  { 4, 600, 57, 58 },
	  { 2, 750, 59, 60 },
	  { 1, 40, 8, 8 },
	  { 2, 175, 8, 5 },
	  { 4, 1440, 8, 5 },
	  { 0, 85000, 5, 5 },
	  { 3, 15, 62, 63 },
	  { 5, 1111660, 64, 65 },
	  { 2, 175, 66, 67 },
	  { 4, 600, 8, 5 },
	  { 2, 795, 8, 5 },
	  { 0, 9929995, 5, 8 },
	  { 4, 1140, 5, 5 } 
	};

    tree_node node;

    for (int depth = 0; depth < MAX_TREE_DEPTH; depth++){
        if (current_node >= 0 && current_node < sizeof(rf_nodes)/sizeof(tree_node)) {
            node = rf_nodes[current_node];
            if (node.feature == -1) {
                return node.value;
            }
            if (node.feature >= 0 && node.feature < sizeof(features)/sizeof(int)) {
                if (features[node.feature]*10 < node.value) {
                    current_node = node.left;
                }
                else {
                    current_node = node.right;
                }
            }
            else
                continue;
        }
        else
            continue;
    }
    return 0;

}

static __always_inline int predict(int* features){

    int result = 0;
    	result += predict_tree(0,features);
	result += predict_tree(9,features);
	result += predict_tree(15,features);
	result += predict_tree(21,features);
	result += predict_tree(28,features);
	result += predict_tree(34,features);
	result += predict_tree(40,features);
	result += predict_tree(47,features);
	result += predict_tree(54,features);
	result += predict_tree(61,features);

	return result;
}