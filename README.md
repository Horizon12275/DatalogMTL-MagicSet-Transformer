# DatalogMTL-MagicSet-Transformer

## Usage

```cpp
MagicSet magicSet;                                          // use magic set method
Literal query;                                              // get the query
vector<Rule> ruleList;                                      // get the rules
vector<Rule> magicRules = magicSet.MS(query, ruleList);     // get the magic rules
```

## Configurations

### Method

1. mkdir build

2. cd build

3. cmake ..

4. cmake --build . --clean-first

### Tips

1. Use a class diagram (provided by visual studio) to see the relationships of the classes in it.