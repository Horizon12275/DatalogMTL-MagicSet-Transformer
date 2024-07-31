from meteor_reasoner.materialization.coalesce import *
from meteor_reasoner.materialization.index_build import *
from meteor_reasoner.utils.loader import *
from meteor_reasoner.canonical.utils import fact_entailment
import argparse
import dill
import os

# To Load Canonical Representation and query it, need to input the Canonical Representation path
parser = argparse.ArgumentParser()
parser.add_argument("--CRpath", required=True, type=str, help="Input the Canonical Representation path")

# Check the input
args = parser.parse_args()
print("Input CRpath is:" + args.CRpath)

# Extract file name from path
CR_file_name = os.path.splitext(os.path.basename(args.CRpath))[0]

# Read the Canonical Representation
# CR_path = './canonical_representations/CR_magic_T4.pkl'
CR_path = args.CRpath
print("Loading the Canonical Representation from " + CR_path + "...")
with open(CR_path, 'rb') as f:
    D1, common, varrho_left, left_period, left_len, varrho_right, right_period, right_len = dill.load(f)


LUBM_predicate_list = [
    "AssistantProfessorCandidate",
    "AssociateProfessor",
    "AssociateProfessorCandidate",
    "Chair",
    "Course",
    "Employee",
    "Faculty",
    "FullProfessor",
    "FullProfessorCandidate",
    "GoodDepartment",
    "Lecturer",
    "LecturerCandidate",
    "Organization",
    "Publication",
    "ResearchAssistant",
    "ResearchAssistantCandidate",
    "Scientist",
    "ScientistCandidate",
    "SmartStudent",
    "Student",
    "TeachingAssistant",
    "University",
    "Work"
]
iTemporal_predicate_list = [
    "g4859",
    "g4860",
    "g4862",
    "g4863",
    "g4864",
    "g4866",
    "g4867",
    "g4869",
    "g4901"
]
predicate_list = LUBM_predicate_list
isITemporal = False
if "ITemporal" in CR_path:
    predicate_list = iTemporal_predicate_list
    isITemporal = True
# Change Here to test different predicates
# predicate = "AssociateProfessorCandidate"

for predicate in predicate_list:
    # Dictionary to store max and min intervalnum for each fact
    fact_interval_info = {}

    # Test the entailment in a loop, change here to test different fact entities and interval numbers
    for i in range(0,1000000):
        if not isITemporal:
            for interval_num in range(-50,100):
                query = predicate + "(ID" + str(i) + ")"
                fact_input = query + "@" + str(interval_num)

                # Parse the input fact
                try:
                    fact = parse_str_fact(fact_input)
                    F = Atom(fact[0], fact[1], fact[2])
                except:
                    print("\nThe format you input is not correct!")
                    continue

                # Try to entail
                entailment_result = fact_entailment(D1, F, common, left_period, left_len, right_period, right_len)
                if entailment_result:
                    # Update max and min intervalnum for the fact
                    if query in fact_interval_info:
                        current_min, current_max = fact_interval_info[query]
                        new_min = min(current_min, interval_num)
                        new_max = max(current_max, interval_num)
                        fact_interval_info[query] = (new_min, new_max)
                    else:
                        fact_interval_info[query] = (interval_num, interval_num)
        else:
            for interval_num in range(0, 20000, 200):
                query = predicate + "(" + str(i//100) + ".0,"+ str(i % 100) +".0)"
                fact_input = query + "@" + str(interval_num)
                try:
                    fact = parse_str_fact(fact_input)
                    F = Atom(fact[0], fact[1], fact[2])
                except:
                    print("\nThe format you input is not correct!")
                    continue

                # Try to entail
                entailment_result = fact_entailment(D1, F, common, left_period, left_len, right_period, right_len)
                if entailment_result:
                    # Update max and min intervalnum for the fact
                    if query in fact_interval_info:
                        current_min, current_max = fact_interval_info[query]
                        new_min = min(current_min, interval_num)
                        new_max = max(current_max, interval_num)
                        fact_interval_info[query] = (new_min, new_max)
                    else:
                        fact_interval_info[query] = (interval_num, interval_num)
    # Output the stored information
    # print("\nStored fact information:")
    # for fact, (min_interval, max_interval) in fact_interval_info.items():
    #     # print(fact, "=> Min Interval:", min_interval, "Max Interval:", max_interval)

    output_file = "./query_results/" + CR_file_name + "_" + predicate + ".txt"

    # Write the stored information to the output file
    with open(output_file, 'w') as outfile:
        outfile.write("Stored fact information:\n")
        for fact, (min_interval, max_interval) in fact_interval_info.items():
            outfile.write(f"{fact} => Min Interval: {min_interval}, Max Interval: {max_interval}\n")

    outfile.close()

    print(f"Results saved to {output_file}")