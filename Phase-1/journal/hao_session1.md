**Name/NetID**: Hao Zhang/hzhan407

**Session Date**:  4/14/2026(6 days)

**Objective**:  Compare and report the correctness and performance results of sequential and concurrent execution.  

**Attempts made:** 

- All expected result showed wrong, and they loaded incorrectly.

int src, K, queryType，expectedResult;

Task 0: result = 20, expected =  [WRONG]
Task 1: result = 53604, expected = d [WRONG]
Task 2: result = -1, expected =   [WRONG]
Task 3: result = 81637, expected = σ [WRONG]
Task 4: result = 224, expected = α [WRONG]
Task 5: result = 81840, expected = ░ [WRONG]
Task 6: result = 46488, expected = ÿ [WRONG]
Task 7: result = 77265, expected = ╤ [WRONG]
Task 8: result = 67124, expected = 4 [WRONG]
Task 9: result = 39064, expected = ÿ [WRONG]
Task 10: result = 71907, expected = π [WRONG]
Task 11: result = 80883, expected = ≤ [WRONG]
Task 12: result = -1, expected =   [WRONG]
Task 13: result = 80608, expected = α [WRONG]
Task 14: result = 81674, expected = 
[WRONG]
Task 15: result = 163, expected = ú [WRONG]
Task 16: result = 2, expected =  [WRONG]
Task 17: result = -1, expected =   [WRONG]
Task 18: result = 79426, expected = B [WRONG]
Task 19: result = 0, expected =  [WRONG]
Correct: 0 / 20

**Solutions/Workarounds**:  

- I change the type of expectedResult from int to string.

Commit 57d8366

Task 0: result = 20, expected = 20 [OK]
Task 1: result = 53604, expected = 53604 [OK]
Task 2: result = -1, expected = -1 [OK]
Task 3: result = 81637, expected = 81637 [OK]
Task 4: result = 224, expected = 224 [OK]
Task 5: result = 81840, expected = 81840 [OK]
Task 6: result = 46488, expected = 46488 [OK]
Task 7: result = 77265, expected = 77265 [OK]
Task 8: result = 67124, expected = 67124 [OK]
Task 9: result = 39064, expected = 39064 [OK]
Task 10: result = 71907, expected = 71907 [OK]
Task 11: result = 80883, expected = 80883 [OK]
Task 12: result = -1, expected = -1 [OK]
Task 13: result = 80608, expected = 80608 [OK]
Task 14: result = 81674, expected = 81674 [OK]
Task 15: result = 163, expected = 163 [OK]
Task 16: result = 2, expected = 2 [OK]
Task 17: result = -1, expected = -1 [OK]
Task 18: result = 79426, expected = 79426 [OK]
Task 19: result = 0, expected = 0 [OK]
Correct: 20 / 20

**Experiments/Analysis (if applicable)**:

- I ran my system using the graph in "soc-Slashdot0902.txt" and tested the performance of my system using "queries20.txt" and "queries10000.txt". The results show that the speed of the concurrent version is faster than the speed of the sequential version. This is because multiple threads process query tasks concurrently.

**Learning outcome**: Briefly describe technical things you learned in this session.
- I learned how implement the CSR and K-hop BFS in C++.
- I learned how to use lambda and std::function