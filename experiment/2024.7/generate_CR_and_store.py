from meteor_reasoner.materialization.coalesce import *
from meteor_reasoner.materialization.index_build import *
from meteor_reasoner.utils.loader import *
from meteor_reasoner.canonical.utils import find_periods
from meteor_reasoner.canonical.canonical_representation import CanonicalRepresentation
from meteor_reasoner.canonical.utils import fact_entailment
import argparse
import time
import dill

# To Generate Canonical Representation and save it, need to input the dataset and the program
parser = argparse.ArgumentParser()
parser.add_argument("--datapath", required=True, type=str, help="Input the dataset path")
parser.add_argument("--rulepath", required=True, type=str, help="Input the program path")
parser.add_argument("--CRpath", required=True, type=str, help="Input the Canonical Representation path")

# Check the input
args = parser.parse_args()
print("Input datapath is:" + args.datapath)
print("Input rulepath is:" + args.rulepath)
print("Input CRpath is:" + args.CRpath)

# Load the program
with open(args.rulepath) as file:
    rules = file.readlines()
    program = load_program(rules)

# Load the dataset
D = load_dataset_for_program(args.datapath, program)
coalescing_d(D)

# Start the canonical timer
print("\nBuilding the Canonical Representation...")
start_canonical_build_time = time.time()

# Find the Canonical Representation
CR = CanonicalRepresentation(D, program)
CR.initilization()
D1, common, varrho_left, left_period, left_len, varrho_right, right_period, right_len = find_periods(CR)

# Stop the canonical timer
end_canonical_build_time = time.time()
canonical_build_time = end_canonical_build_time - start_canonical_build_time
print("Execution_canonical_build_time =", canonical_build_time, "seconds")

# Test the entailment
# fact = parse_str_fact("a1:FullProfessor(http://www.department0.university0.edu/fullprofessor1)@(1,+inf)")
# F = Atom(fact[0], fact[1], fact[2])
# print(F)
# print("Entailment:", fact_entailment(D1, F, common, left_period, left_len, right_period, right_len))

# Save the Canonical Representation
# CR_path = './canonical_representations/CR_magic_T4.pkl'
CR_path = args.CRpath
with open(CR_path, 'wb') as f:
    dill.dump((D1, common, varrho_left, left_period, left_len, varrho_right, right_period, right_len), f)
print("Canonical Representation saved to " + CR_path)

# Also save the canonical representation build time to a file
CR_time_path = CR_path.replace('./canonical_representations/', './canonical_representations_time/')
CR_time_path = CR_time_path.rstrip('.pkl') +'_time.txt'
with open(CR_time_path, 'w') as f:
    f.write(str(canonical_build_time))