from meteor_reasoner.datagenerator import generate_datalog

owl_path = "./data" # input the dir_path where owl files locate
out_dir = "./output" # input the path for the converted datalog triplets

generate_datalog.extract_triplets(owl_path, out_dir)