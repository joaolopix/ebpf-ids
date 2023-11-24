import re

def extract_and_copy(source_file_path, dest_file_path, start_pattern_trees, end_pattern_trees,number_of_trees):
    with open(source_file_path, 'r') as src:
        lines = src.readlines()
    content = "\n"
    found_trees_section = False
    for line in lines:
        if end_pattern_trees in line:
            break
        if start_pattern_trees in line:
            found_trees_section = True
        if found_trees_section:
            line = line.replace('.', '')  # Remove periods
            line = line.replace(', int32_t features_length','')
            line = line.replace('int32_t','int')
            line = line.replace('const float','int')
            line = line.replace('EmlTreesNode', 'tree_node')  # Replace EmlTreesNode with tree_node
            content += line


    content += "\rstatic inline int predict_C(__u64 duration,__u16 protocol,__u64 packet_counter, __u64 transmited_bytes, int current_flags) {" \
               "\n\tint features[5] = {duration*10,protocol*10,packet_counter*10,transmited_bytes*10,current_flags*10};" \
               "\n\tint vote=0;" \
               "\n\tint normal=0;" \
               "\n\tint attack = 0;\n"

    for i in range(number_of_trees):
        content += f"\tvote = rf_predict_tree_{i}(features);if(vote==0) attack +=1; else normal += 1;\n"

    content+="\n\tif(normal >= attack)\n" \
             "\t\treturn 1;\n" \
             "\telse\n" \
             "\t\treturn 0;\n" \
             "}\n"

    with open(dest_file_path, 'w') as dest:
        dest.write(content)

def generate_C_code(source_file_path, dest_file_path,number_of_trees):

    start_pattern_trees = 'static inline'
    end_pattern_trees = 'int32_t rf_predict(const float *features, int32_t features_length) {'

    extract_and_copy(source_file_path, dest_file_path, start_pattern_trees, end_pattern_trees,number_of_trees)

