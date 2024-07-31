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
import re
import openpyxl
from openpyxl import Workbook

# LUBM_predicate_list = [
#     "AssistantProfessorCandidate",
#     "AssociateProfessor",
#     "AssociateProfessorCandidate",
#     "Chair",
#     "Course",
#     "Employee",
#     "Faculty",
#     "FullProfessor",
#     "FullProfessorCandidate",
#     "GoodDepartment",
#     "Lecturer",
#     "LecturerCandidate",
#     "Organization",
#     "Publication",
#     "ResearchAssistant",
#     "ResearchAssistantCandidate",
#     "Scientist",
#     "ScientistCandidate",
#     "SmartStudent",
#     "Student",
#     "TeachingAssistant",
#     "University",
#     "Work"
# ]

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

predicate_list = iTemporal_predicate_list

for dataset_index in range(1,6):
    datapath = f"./iTemporal_data/10^{dataset_index}/"

    wb_for_statistics = Workbook()
    ws_for_statistics = wb_for_statistics.active

    # 设定表头
    ws_for_statistics.append(["Predicate", "Total query count", "Total entailed count", "Total not entailed count", "Total entailed time", "Total not entailed time", "Avg entailed time", "Avg not entailed time"])


    for predicate in predicate_list:
        rulepath = f"./iTemporal_programs/iTemporal_{predicate}.txt"

        # 获取文件名（去除路径和扩展名）
        datapath_filename = "iTemporal_10^" + str(dataset_index)
        program_filename = os.path.basename(rulepath).split('.')[0]

        query_results = f"./query_results/ITemporal_10^{dataset_index}_{predicate}.txt"

        # Load the program
        with open(rulepath) as file:
            rules = file.readlines()
            program = load_program(rules)

        # 读取文本文件内容
        filename = query_results

        # 定义正则表达式来匹配每行中的信息
        pattern = r'g(\d+)\(([\d.]+),([\d.]+)\)'

        # 打开文件并读取内容
        with open(f'./query_results/ITemporal_10^{dataset_index}_{predicate}.txt', 'r') as file:
            text = file.read()

        # 使用正则表达式找到匹配的内容
        matches = re.findall(pattern, text)

        # 输出结果示例
        for match in matches:
            entity1 = match[1]
            entity2 = match[2]
            print(f"Predicate: g{match[0]}, Entity 1: {entity1}, Entity 2: {entity2}")

        # 打开文件
        with open(filename, 'r') as f:
            lines = f.readlines()

        min_intervals = []
        max_intervals = []

        # 遍历每一行
        for line in lines:
            # 使用find方法查找关键词位置
            id_start = 0
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
        total_entailed_time = 0
        total_not_entailed_time = 0

        # store the statistics
        statistics_file = open(f"./scalability_results/{datapath_filename}_{program_filename}_{predicate}_statistics_new.txt", 'w')

        # 创建一个新的 Workbook 和 active worksheet, to store the query results
        wb = Workbook()
        ws = wb.active

        # 设定表头
        ws.append(["Query", "Final Entailment", "Query Time","Is CR?"])

        cnt = 0

        for match in matches[:10]:
            for j in range(0, 10):
                # Load the dataset every time
                D = load_dataset_for_program(datapath, program)
                coalescing_d(D)
                min_rational, max_rational = get_min_max_rational(D)
                #tmp_fact_str = args.predicate + "(ID" + str(i) + ")" + "@" + str(random.randint(min_rational, max_rational))
                if(j <= 5):
                    tmp_fact_str = f"{predicate}({match[1]},{match[2]})@{random.randint(min_intervals[cnt], max_intervals[cnt])}"
                    print(min_intervals[cnt], max_intervals[cnt])
                if(j <= 8 and j > 5):
                    tmp_fact_str = f"{predicate}({match[1]},{match[2]})@{random.randint(max_rational, max_rational * 2)}"
                if(j == 9):
                    tmp_fact_str = f"{predicate}({match[1]},{match[2]})@{random.randint(-max_rational,0)}"
                    cnt += 1
                print("tmp_fact_str = ", tmp_fact_str)
                fact = parse_str_fact(tmp_fact_str)
                F = Atom(fact[0], fact[1], fact[2])

                D[f"magic_{predicate}_bb"][tuple([Term(match[1]),Term(match[2])])].append(Interval(decimal.Decimal("-inf"), decimal.Decimal("+inf"),True,True))

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
                    total_entailed_time += float(str(e))
                    continue

                # Stop the canonical timer
                end_canonical_build_time = time.time()

                # Test the entailment
                entailment_result = fact_entailment(D1, F, common, left_period, left_len, right_period, right_len)
                print("Final Entailment from canonical representation:", entailment_result)
                print("query_time is(with CR):", end_canonical_build_time - start_canonical_build_time, "seconds")

                if entailment_result:
                    total_entailed_count += 1
                    total_entailed_time += end_canonical_build_time - start_canonical_build_time
                else:
                    total_not_entailed_count += 1
                    total_not_entailed_time += end_canonical_build_time - start_canonical_build_time
                
                total_query_count += 1

                # 在循环内部将数据添加到工作表中
                ws.append([tmp_fact_str, entailment_result, f"{end_canonical_build_time - start_canonical_build_time}","True"])

        # excel file
        wb.save(f"./scalability_results/{datapath_filename}_{program_filename}_{predicate}_new.xlsx")

        statistics_file.write(f"Total query count: {total_query_count}\n")
        statistics_file.write(f"Total entailed count: {total_entailed_count}\n")
        statistics_file.write(f"Total not entailed count: {total_not_entailed_count}\n")
        statistics_file.write(f"Total entailed time: {total_entailed_time}\n")
        statistics_file.write(f"Total not entailed time: {total_not_entailed_time}\n")
        if total_entailed_count != 0:
            statistics_file.write(f"Avg entailed time: {total_entailed_time/total_entailed_count}\n")
        else:
            statistics_file.write(f"Avg entailed time: 0\n")
        if total_not_entailed_count != 0:
            statistics_file.write(f"Avg not entailed time: {total_not_entailed_time/total_not_entailed_count}\n")
        else:
            statistics_file.write(f"Avg not entailed time: 0\n")

        # 计算平均时间，避免除以零的情况
        avg_entailed_time = total_entailed_time / total_entailed_count if total_entailed_count != 0 else 0
        avg_not_entailed_time = total_not_entailed_time / total_not_entailed_count if total_not_entailed_count != 0 else 0

        # 在工作表中添加数据，将条件结果添加到列表中
        ws_for_statistics.append([
            predicate,
            total_query_count,
            total_entailed_count,
            total_not_entailed_count,
            total_entailed_time,
            total_not_entailed_time,
            avg_entailed_time,
            avg_not_entailed_time
        ])
    
    # 保存文件
    wb_for_statistics.save(f"./scalability_results/{datapath_filename}_statistics_new.xlsx")

# python iTemporal_scalability.py