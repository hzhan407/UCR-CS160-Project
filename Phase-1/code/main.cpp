#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <functional>
#include <thread>
#include <chrono>

struct CSRGraph {
    int num_vertices;
    std::vector<int> offsets;
    std::vector<int> edges;
};

using QueryCallback = std::function<std::string(const CSRGraph&, int src, int K)>;

struct QueryTask {
    int src;
    int K;
    QueryCallback cb;
    std::string result;
    std::string expected;
};

CSRGraph LoadGraph(const std::string& filename) {
    std::ifstream infile(filename);
    if(!infile.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    std::vector<std::pair<int, int>> edge_list;
    std::string line;
    int max_vertex = -1;

    while(std::getline(infile, line)){
        if(line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream iss(line);
        int src, dst;
        if(!(iss >> src >> dst)) {
            continue;
        }
        edge_list.push_back({src, dst});
        max_vertex = std::max(max_vertex, std::max(src, dst));
    }

    infile.close();

    CSRGraph graph;
    graph.num_vertices = max_vertex + 1;
    std::vector<int> degree(graph.num_vertices, 0);
    for(auto &e : edge_list){
        degree[e.first]++;
    }
    graph.offsets.resize(graph.num_vertices + 1, 0);
    graph.edges.resize(edge_list.size());
    for(int i = 0; i < graph.num_vertices; ++i){
        graph.offsets[i + 1] = graph.offsets[i] + degree[i];
    }
    std::vector<int> curr = graph.offsets;
    for(const auto &e : edge_list){
        int src = e.first;
        int dst = e.second;
        graph.edges[curr[src]++] = dst;
    }

    return graph;
};

std::vector<bool> KHopBFS(const CSRGraph& g, int src, int K) {
    std::vector<bool> visited(g.num_vertices, false);
    std::queue<std::pair<int,int>> q;

    q.push({src, 0});
    visited[src] = true;

    while(!q.empty()) {
        auto [node, dist] = q.front();
        q.pop();

        if(dist == K) continue;

        for(int i = g.offsets[node]; i < g.offsets[node + 1]; ++i) {
            int neighbor = g.edges[i];

            if(!visited[neighbor]) {
                visited[neighbor] = true;
                q.push({neighbor, dist + 1});
            }
        }
    }

    return visited;
}

std::string count_cb(const CSRGraph& g, int src, int K) {
    auto visited = KHopBFS(g, src, K);

    int count = 0;
    for(int i = 0; i < g.num_vertices; i++) {
        if(i != src && visited[i]) count++;
    }

    return std::to_string(count);
}

std::string max_cb(const CSRGraph& g, int src, int K) {
    auto visited = KHopBFS(g, src, K);

    int mx = -1;
    for(int i = 0; i < g.num_vertices; ++i) {
        if(i != src && visited[i]) {
            mx = std::max(mx, i);
        }
    }

    return std::to_string(mx);
}

std::vector<QueryTask> LoadQueries(const std::string& filename) {
    std::vector<QueryTask> tasksSeq;
    std::ifstream infile(filename);

    if(!infile.is_open()) {
        std::cerr << "Error: cannot open query file " << filename << std::endl;
        exit(1);
    }

    std::string line;
    while(std::getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        int src, K, queryType;
        std::string expectedResult;

        if(!(iss >> src >> K >> queryType >> expectedResult)) {
            continue;
        }

        QueryTask task;
        task.src = src;
        task.K = K;
        task.cb = (queryType == 1) ? count_cb : max_cb;
        task.result = "";
        task.expected = expectedResult; 

        tasksSeq.push_back(task);
    }

    return tasksSeq;
}

void RunTasksSequential(const CSRGraph& g, std::vector<QueryTask>& tasksSeq) {
    for (auto& t : tasksSeq) {
        t.result = t.cb(g, t.src, t.K);
    }
}

void RunTasksParallel(const CSRGraph& g, std::vector<QueryTask>& tasksSeq, int num_threads) {
    int n = tasksSeq.size();
    std::vector<std::thread> threads;

    auto worker = [&](int tid) {
        int start = n * tid / num_threads;
        int end = n * (tid + 1) / num_threads;

        for(int i = start; i < end; ++i) {
            tasksSeq[i].result = tasksSeq[i].cb(g, tasksSeq[i].src, tasksSeq[i].K);
        }
    };

    for(int t = 0; t < num_threads; ++t) {
        threads.emplace_back(worker, t);
    }

    for(auto& th : threads) {
        th.join();
    }
}

int main() {
    CSRGraph graph = LoadGraph("soc-Slashdot0902.txt");
    // auto tasksSeq = LoadQueries("queries20.txt");
    // auto tasksPara = LoadQueries("queries20.txt");
    auto tasksSeq = LoadQueries("queries10000.txt");
    auto tasksPara = LoadQueries("queries10000.txt");

    RunTasksSequential(graph, tasksSeq);
    int correctSeq = 0;
    for (auto& t : tasksSeq) {
        if (t.result == t.expected) correctSeq++;
    }
    std::cout << "Sequential correct: " << correctSeq << "/" << tasksSeq.size() << std::endl;

    RunTasksParallel(graph, tasksPara, 4);
    int correctPara = 0;
    for (auto& t : tasksPara) {
        if (t.result == t.expected) correctPara++;
    }
    std::cout << "Parallel correct: " << correctPara << "/" << tasksPara.size() << std::endl;

    auto startSeq = std::chrono::high_resolution_clock::now();
    RunTasksSequential(graph, tasksSeq);
    auto endSeq = std::chrono::high_resolution_clock::now();
    auto durationSeq = std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq);
    std::cout << "Sequential time: " << durationSeq.count() << " ms" << std::endl;

    auto startPara = std::chrono::high_resolution_clock::now();
    RunTasksParallel(graph, tasksPara, 4);
    auto endPara = std::chrono::high_resolution_clock::now();
    auto durationPara = std::chrono::duration_cast<std::chrono::milliseconds>(endPara - startPara);
    std::cout << "Parallel time: " << durationPara.count() << " ms" << std::endl;

    return 0;
}