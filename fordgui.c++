#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <climits>
#include <algorithm>

using namespace std;

struct Node {
    std::string name;
    Node(std::string n) : name(n) {}
};

void printPath(std::map<Node*, Node*> parent, Node* source, Node* sink) {
    std::vector<std::string> path;
    for (Node* v = sink; v != nullptr; v = parent[v]) {
        path.push_back(v->name);
    }
    std::reverse(path.begin(), path.end());
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i != path.size() - 1) std::cout << " -> ";
    }
    std::cout << std::endl;
}

int fordFulkerson(Node* source, Node* sink, 
    std::vector<std::pair<std::pair<Node*, Node*>, int>>& edges) {

    std::map<std::pair<Node*, Node*>, int> residualGraph;
    std::map<std::pair<Node*, Node*>, int> flowGraph;

    for (auto& edgePair : edges) {
        residualGraph[edgePair.first] = edgePair.second;
        flowGraph[edgePair.first] = 0;
    }

    int maxFlow = 0;
    while (true) {
        std::map<Node*, Node*> parent;
        std::queue<Node*> q;
        q.push(source);
        parent[source] = nullptr;

        bool foundPath = false;
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (current == sink) {
                foundPath = true;
                break;
            }

            for (auto& edge : edges) {
                Node* u = edge.first.first;
                Node* v = edge.first.second;
                if (u == current && residualGraph[{u, v}] > 0 && parent.find(v) == parent.end()) {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (!foundPath) break;

        int pathFlow = INT_MAX;
        for (Node* v = sink; v != source; v = parent[v]) {
            Node* u = parent[v];
            pathFlow = std::min(pathFlow, residualGraph[{u, v}]);
        }

        std::cout << "Augmenting path: ";
        printPath(parent, source, sink);
        std::cout << "Path capacity: " << pathFlow << "\n";

        for (Node* v = sink; v != source; v = parent[v]) {
            Node* u = parent[v];
            residualGraph[{u, v}] -= pathFlow;
            residualGraph[{v, u}] += pathFlow;
            flowGraph[{u, v}] += pathFlow;
        }

        maxFlow += pathFlow;

        std::cout << "Intermediate Flow: " << maxFlow << "\n";
    }

    return maxFlow;
}

int main() {
    int n;
    std::cout << "Enter number of edges: ";
    std::cin >> n;

    std::map<std::string, Node*> nodes;
    std::vector<std::pair<std::pair<Node*, Node*>, int>> edges;

    for (int i = 0; i < n; ++i) {
        std::string u, v;
        int capacity;
        std::cout << "Enter edge (from to capacity): ";
        std::cin >> u >> v >> capacity;

        if (nodes.find(u) == nodes.end()) nodes[u] = new Node(u);
        if (nodes.find(v) == nodes.end()) nodes[v] = new Node(v);

        edges.push_back({{nodes[u], nodes[v]}, capacity});
    }

    std::string sourceName, sinkName;
    std::cout << "Enter source node: ";
    std::cin >> sourceName;
    std::cout << "Enter sink node: ";
    std::cin >> sinkName;

    Node* source = nodes[sourceName];
    Node* sink = nodes[sinkName];

    int maxFlow = fordFulkerson(source, sink, edges);
    std::cout << "The maximum possible flow is: " << maxFlow << std::endl;

    return 0;
}

//  g.addEdge(0, 1, 3);
//     g.addEdge(0, 2, 7);
//     g.addEdge(2, 1, 5);
//     g.addEdge(1, 3, 3);
//     g.addEdge(1, 4, 4);
//     g.addEdge(2, 4, 3);
//     g.addEdge(3, 4, 3);
//     g.addEdge(3, 5, 2);
//     g.addEdge(4, 5, 6);