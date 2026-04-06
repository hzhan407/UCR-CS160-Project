# Phase 1: Concurrent Local Queries

## Introduction

In this phase, you will load a directed graph from an edge list file into memory using the Compressed Sparse Row (CSR) format, and implement K-hop neighborhood queries. Each query explores vertices reachable within K hops from a given source vertex (following outgoing edges) and computes a result using a user-defined callback.

You will then evaluate a sequence of such queries, first sequentially and then concurrently using multiple threads, and compare their execution time.

## Requirements

1. **Graph Loading**: Read an input graph in edge list format and store it in memory using the CSR representation.

2. **Query Callback**: Provide a callback interface that allows developers to implement custom logic for processing the K-hop query results. You should support at least the following queries:
   - Count the number of reachable nodes within K hops (excluding the source vertex itself).
   - Find the maximum node ID among the reachable nodes within K hops (excluding the source vertex itself).

3. **Sequential Evaluation**: Execute a given sequence of K-hop queries one by one and record the total execution time.

4. **Concurrent Evaluation**: Execute the same sequence of K-hop queries concurrently using multiple threads and record the total execution time.

5. **Performance Comparison**: Measure and compare the execution time between sequential and concurrent evaluation. Report your findings with analysis in the development journal.

## Hints

### Edge List Format

The input graph is stored as a text file where each line represents a directed edge:

```
src dst
```

Lines starting with `#` are comments and should be skipped. Node IDs are integers. Example:

```
# example graph
# fromNode toNode
0 1
0 2
1 3
2 3
```

### Data Structures & Interfaces

The following definitions are provided as a starting point. Feel free to adapt or redesign them to better suit your implementation.

```cpp
struct CSRGraph {
    int num_vertices;
    std::vector<int> offsets;  // size = num_vertices + 1
    std::vector<int> edges;    // concatenated adjacency lists
    // offsets[i]..offsets[i+1] gives the range of neighbors for vertex i in edges[]
};

using QueryCallback = std::function<std::string(const CSRGraph&, int src, int K)>;

struct QueryTask {
    int src;
    int K;
    QueryCallback cb;
    std::string result;
};

void RunQueriesSequential(const CSRGraph& g, std::vector<QueryTask>& tasks);

// execute tasks concurrently with num_threads threads
void RunTasksParallel(const CSRGraph& g, std::vector<QueryTask>& tasks, int num_threads);
```

### Pseudocode

The following pseudocode illustrates the overall workflow. It is provided as a referencea, and you may organize your code differently.

```
// 1. Load graph
graph = LoadGraph("edges.txt")

// 2. Define callbacks
count_cb = (graph, src, K) -> return count of K-hop reachable nodes
max_cb   = (graph, src, K) -> return max node ID among K-hop reachable nodes

// 3. Build query tasks
tasks = [
    QueryTask(src=0, K=2, cb=count_cb),
    QueryTask(src=3, K=1, cb=max_cb),
    ...
]

// 4. Run sequentially, record time
seq_tasks = copy(tasks)
t1 = now()
RunTasksSequential(graph, seq_tasks)
sequential_time = now() - t1

// 5. Run concurrently, record time
par_tasks = copy(tasks) // copy avoid overwriting results
t2 = now()
RunTasksParallel(graph, par_tasks, num_threads=4)
parallel_time = now() - t2

// 6. Compare and report
print(sequential_time, parallel_time)
```

## Testing & Verification

To verify the correctness of your implementation, we recommend the following approach:

1. **Construct your own test cases**: Start with small graphs where you can manually compute the expected K-hop results. For example:
   - A graph with cycles to ensure visited nodes are not revisited
   - A star graph to test fan-out behavior
   - Edge cases: isolated vertices, K=0, source vertex not in graph

2. **Compare sequential and concurrent results**: Run the same set of queries using both `RunTasksSequential` and `RunTasksParallel`, and verify that they produce identical results.

3. **Verify on the provided dataset**: Once small cases pass, run your implementations on the provided dataset ([cit-HepTh.txt.gz](https://snap.stanford.edu/data/cit-HepTh.html)). Note that node IDs in this dataset are **not necessarily contiguous** starting from 0. You may need to remap them to `[0, N)` or use a mapping structure to handle this. Construct your own queries on this graph and verify that sequential and concurrent results are identical. Consider varying the following factors to observe their impact on performance:
   - **Number of queries**: e.g., 100 vs. 10,000 queries
   - **K value**: small (1–2) vs. large (5+), which affects the amount of work per query
   - **Source vertex selection**: random vertices
   
   You should also expect a noticeable speedup from the concurrent version.