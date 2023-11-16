import re

def extract_and_copy(source_file, dest_file, start_pattern_nodes, end_pattern_nodes, start_pattern_roots, max_tree_depth):
    with open(source_file, 'r') as src:
        lines = src.readlines()

    roots_values = []

    # Construct the content string
    content = f"""
import ctypes

max_tree_depth = {max_tree_depth}

class TreeNode(ctypes.Structure):
    _fields_ = [("feature", ctypes.c_int),
                ("value", ctypes.c_int),
                ("left", ctypes.c_int),
                ("right", ctypes.c_int)]
rf_nodes = [
"""
    found_nodes_section = False
    for line in lines:
        if start_pattern_nodes in line:
            found_nodes_section = True
            continue
        if end_pattern_nodes in line:
            found_nodes_section = False
        if found_nodes_section:
            line = line.replace('.', '')  # Remove periods
            digits = [int(match.group()) for match in re.finditer(r'-?\d+', line)]
            if(len(digits)==0):
                continue
            content += f"\tTreeNode({digits[0]}, {digits[1]}, {digits[2]}, {digits[3]}),\n"
        if start_pattern_roots in line:
            # Extract values from rf_tree_roots and store in roots_values
            digits_after_equal = re.findall(r'=\s*\{([^}]*)\}', line)
            roots_values = [int(value) for value in digits_after_equal[0].split(',')]
    content += "]\nrf_tree_roots = [0"
    for value in roots_values[1:]:
        content += f",{value}"
    content += "]\n"

    with open(dest_file, 'w') as dest:
        dest.write(content)

def generate_PY_code(source_file_path,dest_file_path,max_tree_depth_value):

    start_pattern_nodes = 'EmlTreesNode rf_nodes'
    end_pattern_nodes = '};'

    start_pattern_roots = 'int32_t rf_tree_roots'

    extract_and_copy(source_file_path, dest_file_path, start_pattern_nodes, end_pattern_nodes, start_pattern_roots,max_tree_depth_value)

