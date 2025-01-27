#include <bits/stdc++.h>
using namespace std;

class MaxFlow {
private:
    vector<vector<int>> residualGraph;
    vector<int> parent;
    int n;

    bool bfs(int source, int sink) {
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < n; v++) {
                if (!visited[v] && residualGraph[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                    if (v == sink) return true;
                }
            }
        }
        return false;
    }

public:
    MaxFlow(int size) : n(size) {
        residualGraph.assign(n, vector<int>(n, 0));
        parent.resize(n);
    }

    void addEdge(int u, int v, int capacity) {
        residualGraph[u][v] = capacity;
    }

    int fordFulkerson(int source, int sink) {
        int maxFlow = 0;

        cout << "\nStarting Ford-Fulkerson Algorithm...\n";
        while (bfs(source, sink)) {
            vector<int> path;
            int pathFlow = INT_MAX;

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                path.push_back(v);
                pathFlow = min(pathFlow, residualGraph[u][v]);
            }
            path.push_back(source);
            reverse(path.begin(), path.end());

            cout << "Augmenting path: ";
            for (int node : path) cout << node << " ";
            cout << "\nBottleneck capacity: " << pathFlow << endl;

            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residualGraph[u][v] -= pathFlow;
                residualGraph[v][u] += pathFlow;
            }

            maxFlow += pathFlow;

            cout << "Updated Residual Graph:\n";
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    cout << residualGraph[i][j] << " ";
                }
                cout << endl;
            }
            cout << "------------------------------------\n";
        }

        return maxFlow;
    }
};

int main() {
    int n, m;
    cout << "Enter the number of vertices: ";
    cin >> n;
    cout << "Enter the number of edges: ";
    cin >> m;

    MaxFlow maxFlowSolver(n);

    cout << "Enter the edges and their capacities (format: u v capacity):\n";
    for (int i = 0; i < m; i++) {
        int u, v, capacity;
        cin >> u >> v >> capacity;
        maxFlowSolver.addEdge(u, v, capacity);
    }

    int source, sink;
    cout << "Enter the source vertex: ";
    cin >> source;
    cout << "Enter the sink vertex: ";
    cin >> sink;

    int maxFlow = maxFlowSolver.fordFulkerson(source, sink);
    cout << "\nThe maximum possible flow is: " << maxFlow << endl;

    return 0;
}
