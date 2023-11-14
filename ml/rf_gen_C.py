import re

def extract_and_copy(source_file, dest_file, start_pattern_nodes, end_pattern_nodes, start_pattern_roots, end_pattern_roots, max_tree_depth):
    with open(source_file, 'r') as src:
        lines = src.readlines()

    roots_values = []

    # Construct the content string
    content = f"""
#define MAX_TREE_DEPTH {max_tree_depth}

typedef struct tree_node {{
    int feature;
    int value;
    int left;
    int right;
}} tree_node;

static __always_inline int predict_tree(int current_node, int* features) {{
"""
    content += "\r"
    found_nodes_section = False
    for line in lines:
        if start_pattern_nodes in line:
            found_nodes_section = True
        if found_nodes_section:
            line = line.replace('.', '')  # Remove periods
            line = line.replace('EmlTreesNode', 'tree_node')  # Replace EmlTreesNode with tree_node
            content += "\t"+line
        if end_pattern_nodes in line:
            found_nodes_section = False
        if start_pattern_roots in line:
            # Extract values from rf_tree_roots and store in roots_values
            digits_after_equal = re.findall(r'=\s*\{([^}]*)\}', line)
            roots_values = [int(value) for value in digits_after_equal[0].split(',')]

    content += f"""
    tree_node node;

    for (int depth = 0; depth < MAX_TREE_DEPTH; depth++){{
        if (current_node >= 0 && current_node < sizeof(rf_nodes)/sizeof(tree_node)) {{
            node = rf_nodes[current_node];
            if (node.feature == -1) {{
                return node.value;
            }}
            if (node.feature >= 0 && node.feature < sizeof(features)/sizeof(int)) {{
                if (features[node.feature]*10 < node.value) {{
                    current_node = node.left;
                }}
                else {{
                    current_node = node.right;
                }}
            }}
            else
                continue;
        }}
        else
            continue;
    }}
    return 0;

}}

static __always_inline int predict(int* features){{

    int result = 0;
    """
    content += "\r"
    for value in roots_values:
        content += f"\tresult += predict_tree({value},features);\n"

    content += "\n\treturn result;\n}"

    # Write the content string to the destination file
    with open(dest_file, 'w') as dest:
        dest.write(content)

def generate_C_code():
    # Specify your source and destination file paths
    source_file_path = 'rf.h'
    dest_file_path = 'rf_model.h'
    max_tree_depth_value = 3  # Replace with your desired value

    # Specify the start and end patterns for the sections you want to copy
    start_pattern_nodes = 'EmlTreesNode rf_nodes'
    end_pattern_nodes = '};'

    start_pattern_roots = 'int32_t rf_tree_roots'
    end_pattern_roots = '};'

    # Extract, copy, and store roots_values from the source file
    extract_and_copy(source_file_path, dest_file_path, start_pattern_nodes, end_pattern_nodes, start_pattern_roots, end_pattern_roots, max_tree_depth_value)

