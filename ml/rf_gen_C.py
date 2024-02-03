import re

def extract_and_copy(source_file_path, dest_file_path, start_pattern_trees, end_pattern_trees,number_of_trees):
    with open(source_file_path, 'r') as src:
        lines = src.readlines()

    content = "\n"
    content += "\rstatic inline int predict_C(__u64 duration,__u16 protocol,__u64 packet_counter, __u64 transmited_bytes, int current_flags) {" \
               "\n\tint features[5] = {duration*10,protocol*10,packet_counter*10,transmited_bytes*10,current_flags*10};" \
               "\n\tint votes[2] = {0,0};\n\r"

    found_trees_section = False
    first_tree = True
    for line in lines:
        if end_pattern_trees in line:
            break
        if start_pattern_trees in line:
            found_trees_section = True
            if not first_tree:
                content = content.rsplit('}', 1)[0]
                content += "\n"
            first_tree = False
            continue
        if found_trees_section:
            line = line.replace('.', '')  # Remove periods
            line = line.replace('return 0;',"votes[0] += 1;")
            line = line.replace('return 1;',"votes[1] += 1;")
            content += line

    content = content.rsplit('}', 1)[0]
    content += "\r\tint most_voted_class = -1;\n" \
               "\tint most_voted_votes = 0;\n" \
               "\tfor (int i=0; i<sizeof(votes)/sizeof(int); i++) {\n" \
               "\t\tif (votes[i] > most_voted_votes) {\n" \
               "\t\t\tmost_voted_class = i;\n" \
               "\t\t\tmost_voted_votes = votes[i];\n" \
               "\t\t}\n" \
               "\t}\n" \
               "return most_voted_class;\n" \
               "}\n"


    with open(dest_file_path, 'w') as dest:
        dest.write(content)

def generate_C_code(source_file_path, dest_file_path,number_of_trees):

    start_pattern_trees = 'static inline'
    end_pattern_trees = 'int32_t rf_predict(const float *features, int32_t features_length) {'

    extract_and_copy(source_file_path, dest_file_path, start_pattern_trees, end_pattern_trees,number_of_trees)

