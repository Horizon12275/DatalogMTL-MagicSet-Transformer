from meteor_reasoner.materialization.coalesce import *
from meteor_reasoner.materialization.index_build import *
from meteor_reasoner.utils.loader import *
from meteor_reasoner.canonical.magic_utils import find_periods_with_query
from meteor_reasoner.canonical.canonical_representation import CanonicalRepresentation
from meteor_reasoner.utils.parser import parse_str_fact
from meteor_reasoner.classes.atom import Atom
from meteor_reasoner.canonical.utils import fact_entailment
from meteor_reasoner.utils.operate_dataset import get_min_max_rational
import time
import os
import re

datapath = f"./data/lubm_10^6.txt"

datapath_filename = os.path.basename(datapath).split('.')[0]

# python for_manual_baseline_test_with_query_results.py
# # entailed
# query_list = [
# "AssistantProfessorCandidate(ID0)@18",
# "AssistantProfessorCandidate(ID1)@14",
# "AssociateProfessor(ID0)@34",
# "AssociateProfessor(ID1)@19",
# "AssociateProfessorCandidate(ID0)@19",
# "AssociateProfessorCandidate(ID1)@18",
# "Chair(ID2002)@8",
# "Chair(ID3738)@36",
# "Employee(ID0)@36",
# "Employee(ID1)@13",
# "Faculty(ID0)@40",
# "Faculty(ID1)@18",
# "FullProfessor(ID11)@67",
# "FullProfessor(ID11)@78",
# "FullProfessor(ID24)@45",
# "FullProfessor(ID24)@57",
# "FullProfessorCandidate(ID4)@24",
# "FullProfessorCandidate(ID7)@12",
# "GoodDepartment(ID16)@40",
# "GoodDepartment(ID1971)@19",
# "Lecturer(ID0)@12",
# "Lecturer(ID1)@12",
# "LecturerCandidate(ID0)@35",
# "LecturerCandidate(ID1)@15",
# "Organization(ID12)@44",
# "Organization(ID16)@29",
# "Publication(ID6)@21",
# "ResearchAssistant(ID0)@32",
# "ResearchAssistant(ID1)@18",
# "ResearchAssistantCandidate(ID0)@12",
# "ResearchAssistantCandidate(ID1)@19",
# "Scientist(ID11)@61",
# "Scientist(ID11)@88",
# "Scientist(ID24)@34",
# "Scientist(ID24)@53",
# "ScientistCandidate(ID11)@46",
# "ScientistCandidate(ID24)@22",
# "SmartStudent(ID0)@25",
# "SmartStudent(ID1)@36",
# "Student(ID0)@29",
# "Student(ID1)@11",
# "TeachingAssistant(ID4)@5",
# "TeachingAssistant(ID7)@23",
# "Work(ID18)@26",
# "Work(ID2)@38",
# "Employee(ID24)@149",
# "Employee(ID26)@149",
# "Employee(ID50)@149",
# "Faculty(ID24)@149",
# "Faculty(ID26)@149",
# "Faculty(ID50)@149",
# "FullProfessor(ID50)@149",
# "FullProfessor(ID26)@149",
# "FullProfessor(ID24)@149",
# "Scientist(ID50)@149",
# "Scientist(ID26)@149",
# "Scientist(ID24)@149",
# ]


# not entailed
query_list = [
"AssistantProfessorCandidate(ID0)@56",
"AssistantProfessorCandidate(ID1)@94",
"AssociateProfessor(ID0)@70",
"AssociateProfessor(ID1)@88",
"AssociateProfessorCandidate(ID0)@94",
"AssociateProfessorCandidate(ID1)@93",
"Chair(ID2002)@92",
"Chair(ID3738)@93",
"Course(ID18)@55",
"Course(ID2)@69",
"Employee(ID0)@68",
"Employee(ID1)@84",
"Faculty(ID0)@90",
"Faculty(ID1)@78",
"FullProfessorCandidate(ID4)@85",
"FullProfessorCandidate(ID7)@51",
"GoodDepartment(ID16)@87",
"GoodDepartment(ID1971)@78",
"Lecturer(ID0)@87",
"Lecturer(ID1)@90",
"LecturerCandidate(ID0)@61",
"LecturerCandidate(ID1)@67",
"Organization(ID12)@54",
"Organization(ID16)@80",
"Publication(ID3)@56",
"Publication(ID6)@74",
"ResearchAssistant(ID0)@76",
"ResearchAssistant(ID1)@74",
"ResearchAssistantCandidate(ID0)@100",
"ResearchAssistantCandidate(ID1)@82",
"ScientistCandidate(ID11)@98",
"ScientistCandidate(ID24)@53",
"SmartStudent(ID0)@78",
"SmartStudent(ID1)@96",
"Student(ID0)@84",
"Student(ID1)@51",
"TeachingAssistant(ID4)@71",
"TeachingAssistant(ID7)@85",
"University(ID12)@65",
"University(ID31)@59",
"Work(ID18)@69",
"Work(ID2)@51",
]


fact_list = []

for query in query_list:
    fact = parse_str_fact(query)
    F = Atom(fact[0], fact[1], fact[2])
    fact_list.append(F)
    print(F)

parsed_queries = []

for query in query_list:
    match = re.match(r'^([a-zA-Z]+)\((ID\d+)\)@(\d+)$', query)
    if match:
        predicate = match.group(1)
        ID = match.group(2)
        score = match.group(3)
        parsed_queries.append({
            'predicate': predicate,
            'ID': ID,
            'score': score
        })

# 输出解析结果
for parsed_query in parsed_queries:
    print(f"Predicate: {parsed_query['predicate']}, ID: {parsed_query['ID']}, Score: {parsed_query['score']}")


cnt = 0
for parsed_query in parsed_queries:
    
    rulepath = f"./programs/lubm_{parsed_query['predicate']}.txt"

    # Load the program
    with open(rulepath) as file:
        rules = file.readlines()
        program = load_program(rules)
    
    D = load_dataset_for_program(datapath, program)

    # add magic fact into D
    D[f"magic_{parsed_query['predicate']}_b"][tuple([Term(parsed_query['ID'])])].append(Interval(decimal.Decimal("-inf"), decimal.Decimal("+inf"),True,True))
    
    # Start the canonical timer
    start_canonical_build_time = time.time()

    fact = parse_str_fact(query_list[cnt])
    F = Atom(fact[0], fact[1], fact[2])
    print(F)
    print(parsed_query['predicate'])
    print(parsed_query['ID'])
    print(parsed_query['score'])


    # Find the Canonical Representation
    CR = CanonicalRepresentation(D, program)
    CR.initilization()

    try:
        D1, common, varrho_left, left_period, left_len, varrho_right, right_period, right_len = find_periods_with_query(CR,F)
    except Exception as e:
        print("entailed!")
        print(f"query_time is(without CR):{e}")
        with open(f"./lubm_new_program_results_not_entailed/{query_list[cnt]}_magic_new_inside.txt", 'w') as f:
            f.write(f"query_time is(without CR):{e}")
        cnt += 1
        continue

    # Stop the canonical timer
    end_canonical_build_time = time.time()

    # Test the entailment
    entailment_result = fact_entailment(D1, F, common, left_period, left_len, right_period, right_len)
    print("Final Entailment from canonical representation:", entailment_result)
    print("query_time is(with CR):", end_canonical_build_time - start_canonical_build_time, "seconds")
    print("end with cr!")
    with open(f"./lubm_new_program_results_not_entailed/{query_list[cnt]}_magic_new_outside.txt", 'w') as f:
        f.write(f"Final Entailment from canonical representation:{entailment_result}\nquery_time is(with CR):{end_canonical_build_time - start_canonical_build_time}")
    cnt += 1