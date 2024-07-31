### generate_CR_and_store.py

- Input dataset and program to generate canonical representation and store it.
- Use find_periods function.

- Format:

  - `--datapath`: path to the input dataset
  - `--rulepath`: path to the program
  - `--CRpath`: path to store the canonical representation

- Usage:

```bash
# LUBM dataset with 10^4: ./data/lubm_10^5.txt
python generate_CR_and_store.py --datapath ./data/lubm_10^4.txt --rulepath ./programs/p.txt --CRpath ./canonical_representations/CR_10^4.pkl

# ITemporal dataset 10^4: ./iTemporal_data/10^4
python generate_CR_and_store.py --datapath ./iTemporal_data/10^4 --rulepath ./iTemporal_programs/iTemporal_program.txt --CRpath ./canonical_representations/ITemporal_10^4.pkl
```

### load_CR_and_auto_query.py

- Load a canonical representation and query it automatically.

- Format:

  - `--CRpath`: path to the canonical representation

- Usage:

```bash
# LUBM dataset with 10^4
python load_CR_and_auto_query.py --CRpath ./canonical_representations/CR_10^4.pkl

# ITemporal dataset with 10^4
python load_CR_and_auto_query.py --CRpath ./canonical_representations/ITemporal_10^4.pkl
```

### compute_CR_and_query.py

- Input a new dataset and a new program to generate canonical representation and query it once, without storing it.
- This code tests compute_CR_and_query algorithm and only one query is performed each time.
- Use find_periods_query function.
- Same as the init experiments.

- Format:

  - `--datapath`: path to the input dataset
  - `--rulepath`: path to the program
  - `--fact`: query fact

- Usage:

```bash
python compute_CR_and_query.py --datapath ./data/lubm_init.json --rulepath ./programs/p.txt --fact "a1:FullProfessor(http://www.department0.university0.edu/fullprofessor1)@(1,+inf)"

python compute_CR_and_query.py --datapath ./data/lubm_magic_T4.json --rulepath ./programs/p_magic_T4.txt --fact "a1:FullProfessor(http://www.department0.university0.edu/fullprofessor1)@50"
```

### Notice

1. DEBUG MODE WILL SIGNIFICANTLY SLOW DOWN THE PROGRAM... PLEASE DO NOT USE IT WHEN TESTING...
