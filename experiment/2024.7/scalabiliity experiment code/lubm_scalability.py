from meteor_reasoner.materialization.coalesce import *
from meteor_reasoner.materialization.index_build import *
from meteor_reasoner.utils.loader import *
from meteor_reasoner.canonical.magic_utils import find_periods_with_query
from meteor_reasoner.canonical.canonical_representation import CanonicalRepresentation
from meteor_reasoner.utils.parser import parse_str_fact
from meteor_reasoner.classes.atom import Atom
from meteor_reasoner.canonical.utils import fact_entailment
from meteor_reasoner.utils.operate_dataset import get_min_max_rational
import argparse
import time
import os
import openpyxl
from openpyxl import Workbook

# To Generate Canonical Representation and query it, need to input the dataset and the program and the fact
parser = argparse.ArgumentParser()
parser.add_argument("--datapath", required=True, type=str, help="Input the dataset path")
parser.add_argument("--rulepath", required=True, type=str, help="Input the program path")
parser.add_argument("--predicate", required=True, type=str, help="Input a prediate you wanna check the entailment")
parser.add_argument("--query_results", required=True, type=str, help="Input the query results file")

# Check the input
args = parser.parse_args()
print("Input datapath is:" + args.datapath)
print("Input rulepath is:" + args.rulepath)
print("Input predicate is:" + args.predicate)
print("Input query_results is:" + args.query_results)

# 获取文件名（去除路径和扩展名）
datapath_filename = os.path.basename(args.datapath).split('.')[0]
program_filename = os.path.basename(args.rulepath).split('.')[0]

# Load the program
with open(args.rulepath) as file:
    rules = file.readlines()
    program = load_program(rules)

# 读取文本文件内容
filename = args.query_results
id_list = []

with open(filename, 'r') as file:
    for line in file:
        # 使用字符串操作或正则表达式提取ID号码
        if args.predicate in line:
            # 找到ID号码的起始位置和结束位置
            start_index = line.find('ID') + 2
            end_index = line.find(')', start_index)
            
            # 提取ID号码并转换为整数
            candidate_id = int(line[start_index:end_index])
            
            # 将ID号码添加到列表中
            id_list.append(candidate_id)

id_list.sort()
# 打开文件
with open(filename, 'r') as f:
    lines = f.readlines()

min_intervals = []
max_intervals = []

# 遍历每一行
for line in lines:
    # 使用find方法查找关键词位置
    id_start = line.find('SmartStudent(ID') + len('SmartStudent(ID')
    min_start = line.find('Min Interval: ', id_start) + len('Min Interval: ')
    max_start = line.find('Max Interval: ', min_start) + len('Max Interval: ')
    
    # 找到最小间隔和最大间隔的结束位置
    min_end = line.find(',', min_start)
    max_end = line.find('\n', max_start)
    
    # 提取并转换为整数
    try:
        min_interval = int(line[min_start:min_end])
        max_interval = int(line[max_start:max_end])
    except ValueError as e:
        continue
    
    
    # 添加到列表
    min_intervals.append(min_interval)
    max_intervals.append(max_interval)

total_query_count = 0
total_entailed_count = 0
total_not_entailed_count = 0

# store the statistics
statistics_file = open(f"./scalability_results/{datapath_filename}_{program_filename}_{args.predicate}_statistics.txt", 'w')

# 创建一个新的 Workbook 和 active worksheet, to store the query results
wb = Workbook()
ws = wb.active

# 设定表头
ws.append(["Query", "Final Entailment", "Query Time","Is CR?"])

cnt = 0

for i in id_list[:100]:
    for j in range(0, 3):
        # Load the dataset every time
        D = load_dataset(args.datapath)
        coalescing_d(D)
        min_rational, max_rational = get_min_max_rational(D)
        #tmp_fact_str = args.predicate + "(ID" + str(i) + ")" + "@" + str(random.randint(min_rational, max_rational))
        if(j == 0):
            tmp_fact_str = f"{args.predicate}(ID{i})@{random.randint(min_intervals[cnt], max_intervals[cnt])}"
            print(min_intervals[cnt], max_intervals[cnt])
            cnt += 1
        if(j == 1):
            tmp_fact_str = f"{args.predicate}(ID{i})@{random.randint(max_rational, max_rational * 2)}"
        if(j == 2):
            tmp_fact_str = f"{args.predicate}(ID{i})@{random.randint(-max_rational,0)}"
        print("tmp_fact_str = ", tmp_fact_str)
        fact = parse_str_fact(tmp_fact_str)
        F = Atom(fact[0], fact[1], fact[2])

        D[f"magic_{args.predicate}_b"][tuple([Term(f"ID{i}")])].append(Interval(decimal.Decimal("-inf"), decimal.Decimal("+inf"),True,True))

        # Start the canonical timer
        start_canonical_build_time = time.time()

        # Find the Canonical Representation
        CR = CanonicalRepresentation(D, program)
        CR.initilization()

        try:
            D1, common, varrho_left, left_period, left_len, varrho_right, right_period, right_len = find_periods_with_query(CR,F)
        except Exception as e:
            print("entailed!")
            print(f"query_time is(without CR):{e}")
            ws.append([tmp_fact_str, True, f"{e}", "False"])
            total_entailed_count += 1
            total_query_count += 1
            continue

        # Stop the canonical timer
        end_canonical_build_time = time.time()

        # Test the entailment
        entailment_result = fact_entailment(D1, F, common, left_period, left_len, right_period, right_len)
        print("Final Entailment from canonical representation:", entailment_result)
        print("query_time is(with CR):", end_canonical_build_time - start_canonical_build_time, "seconds")

        if entailment_result:
            total_entailed_count += 1
        else:
            total_not_entailed_count += 1
        
        total_query_count += 1

        # 在循环内部将数据添加到工作表中
        ws.append([tmp_fact_str, entailment_result, f"{end_canonical_build_time - start_canonical_build_time}","True"])

# excel file
wb.save(f"./scalability_results/{datapath_filename}_{program_filename}_{args.predicate}.xlsx")

statistics_file.write(f"Total query count: {total_query_count}\n")
statistics_file.write(f"Total entailed count: {total_entailed_count}\n")
statistics_file.write(f"Total not entailed count: {total_not_entailed_count}\n")