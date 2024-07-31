from meteor_reasoner.datagenerator import generate_owl

univ_nume = 100 # input the number of universities you want to generate
dir_name = "./data" # input the directory path used for the generated owl files.

generate_owl.generate(univ_nume, dir_name)
