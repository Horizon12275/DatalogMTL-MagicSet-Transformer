# DatalogMTL-MagicSet-Transformer

## Usage

```cpp
MagicSet magicSet;                                          // use magic set method
Literal query;                                              // get the query
vector<Rule> ruleList;                                      // get the rules
vector<Rule> magicRules = magicSet.MS(query, ruleList);     // get the magic rules
```

## Configurations

### Method(need to install Microsoft Visual Studio AND CMake)

1. mkdir build

2. cd build

3. cmake ..

4. cmake --build . --clean-first

### Tips

1. Use a class diagram (provided by visual studio) to see the relationships of the classes in it.

### File Input Mode

```bash
input path:
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\T2.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\T3.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\T4.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\T5.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\lubm_FullProfessor.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\lubm_SmartStudent.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\lubm_AssociateProfessor.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\lubm_LecturerCandidate.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\lubm_ResearchAssistant.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\lubm_ResearchAssistantCandidate.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\input\iTemporal_test.txt
output path:
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\T2.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\T3.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\T4.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\T5.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\lubm_FullProfessor.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\lubm_SmartStudent.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\lubm_AssociateProfessor.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\lubm_LecturerCandidate.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\lubm_ResearchAssistant.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\lubm_ResearchAssistantCandidate.txt
    D:\Horizon\Projects\Ongoing\DatalogMTL-MagicSet-Transformer\test\output\iTemporal_test.txt
```
