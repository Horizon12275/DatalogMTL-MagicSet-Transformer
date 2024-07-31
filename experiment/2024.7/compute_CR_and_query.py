from meteor_reasoner.materialization.coalesce import *
from meteor_reasoner.materialization.index_build import *
from meteor_reasoner.utils.loader import *
from meteor_reasoner.canonical.magic_utils import find_periods_with_query
from meteor_reasoner.canonical.canonical_representation import CanonicalRepresentation
from meteor_reasoner.utils.parser import parse_str_fact
from meteor_reasoner.classes.atom import Atom
from meteor_reasoner.canonical.utils import fact_entailment
import argparse
import time

# To Generate Canonical Representation and query it, need to input the dataset and the program and the fact
parser = argparse.ArgumentParser()
parser.add_argument("--datapath", required=True, type=str, help="Input the dataset path (json format)")
parser.add_argument("--rulepath", required=True, type=str, help="Input the program path")
parser.add_argument("--fact", required=True, type=str, help="Input a fact you wanna check the entailment")

# Check the input
args = parser.parse_args()
print("Input datapath is:" + args.datapath)
print("Input rulepath is:" + args.rulepath)
print("Input fact is:" + args.fact)

# Load the dataset
D = load_dataset(args.datapath)
coalescing_d(D)

# Load the program
with open(args.rulepath) as file:
    rules = file.readlines()
    program = load_program(rules)

# Parse the input fact
try:
    fact = parse_str_fact(args.fact)
    F = Atom(fact[0], fact[1], fact[2])
except:
   raise ("The format you input is not correct")

# Print the fact
print("\nYou are querying: ", F)
print("If fact can be entailed before the canonical representation computed, the program will stop and output the time spent till this point!")

# Start the canonical timer
start_canonical_build_time = time.time()

# Find the Canonical Representation
CR = CanonicalRepresentation(D, program)
CR.initilization()
D1, common, varrho_left, left_period, left_len, varrho_right, right_period, right_len = find_periods_with_query(CR,F)

# Stop the canonical timer
end_canonical_build_time = time.time()
print("Final Entailment time = Canonical build time =", end_canonical_build_time - start_canonical_build_time, "seconds")

# Test the entailment
entailment_result = fact_entailment(D1, F, common, left_period, left_len, right_period, right_len)
print("Final Entailment from canonical representation:", entailment_result)