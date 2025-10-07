#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

struct Transition {
    string from;
    string symbol;
    string to;
};

struct PathResult {
    vector<Transition> path; 
    bool accepted;           
    string finalState;      
};

map<string, vector<pair<string, string>>> transitions; // For each state: vector of {transition symbol, next state}
set<string> finalStates;
string initialState;
set<string> allStates; 


vector<PathResult> results;


void printTransitionTable() {
    cout << "\n==== Transition Table ====" << endl;
    cout << left << setw(15) << "From State" << setw(10) << "Symbol" << setw(15) << "To State" << endl;
    cout << string(40, '-') << endl;

    for (auto &entry : transitions) {
        for (auto &trans : entry.second) {
            cout << left << setw(15) << entry.first << setw(10) << trans.first << setw(15) << trans.second << endl;
        }
    }
    cout << string(40, '-') << endl;
}

void dfs(string &current, int pos,string &input,
         vector<Transition> currentPath,
         set<pair<string, int>> epsilonVisited) 
{
    bool moved = false;
    
    if (transitions.find(current) != transitions.end()) {
        for (auto &trans : transitions[current]) {
            if (trans.first == "epsilon") {
                pair<string, int> key = { trans.second, pos };
                if (epsilonVisited.find(key) == epsilonVisited.end()) {
                    moved = true;
                    vector<Transition> newPath = currentPath;
                    newPath.push_back({ current, "epsilon", trans.second });
                    auto newVisited = epsilonVisited;
                    newVisited.insert(key);
                    dfs(trans.second, pos, input, newPath, newVisited);
                }
            }
        }
        
        if (pos < input.size()) {
            string sym(1, input[pos]);
            for (auto &trans : transitions[current]) {
                if (trans.first == sym) {
                    moved = true;
                    vector<Transition> newPath = currentPath;
                    newPath.push_back({ current, sym, trans.second });
                    set<pair<string, int>> newEpsilonVisited;
                    dfs(trans.second, pos + 1, input, newPath, newEpsilonVisited);
                }
            }
        }
    }

    if (!moved) {
        bool accepted = (pos == input.size() && finalStates.find(current) != finalStates.end());
        PathResult pr;
        pr.path = currentPath;
        pr.accepted = accepted;
        pr.finalState = current;
        results.push_back(pr);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " input_file" << endl;
        return 1;
    }
    
    ifstream infile(argv[1]);
    if (!infile) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    enum Section { NONE, STATES, ALPHABET, INITIAL, FINAL, TRANSITIONS, TESTS };
    Section currentSection = NONE;
    string line;
    
    while (getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue; // Skip empty and comment lines

        if (line.find("states:") == 0) {
            currentSection = STATES;
            istringstream iss(line.substr(7));
            string state;
            while (iss >> state) {
                allStates.insert(state);
            }
            continue;
        } else if (line.find("alphabet:") == 0) {
            currentSection = ALPHABET;
            continue;
        } else if (line.find("initial:") == 0) {
            currentSection = INITIAL;
            istringstream iss(line.substr(8));
            iss >> initialState;
            continue;
        } else if (line.find("final:") == 0) {
            currentSection = FINAL;
            istringstream iss(line.substr(6));
            string f;
            while (iss >> f) {
                finalStates.insert(f);
            }
            continue;
        } else if (line.find("transitions:") == 0) {
            currentSection = TRANSITIONS;
            continue;
        }
        if (currentSection == TRANSITIONS) {
            istringstream iss(line);
            string from, sym, to;
            if (!(iss >> from >> sym >> to)) continue;
            transitions[from].push_back({ sym, to });
        } 
    }

    printTransitionTable();

    while(1)
    {
        cout<<"enter string to be tested"<<endl;
        string test;
        cin>>test;
        results.clear();
        cout << "\nTesting string: \"" << test << "\"" << endl;
        vector<Transition> path;
        set<pair<string, int>> epsilonVisited;
        dfs(initialState, 0, test, path, epsilonVisited);
        bool acceptedOverall = false;
        for (auto &pr: results) {
            cout << "Path: ";
            cout << initialState;
            for (auto &t : pr.path) {
                cout << " --" << t.symbol << "--> " << t.to;
            }
            cout << " | Final state: " << pr.finalState;
            if (pr.accepted) {
                cout << " [Accepted]";
                acceptedOverall = true;
            } else {
                cout << " [Rejected]";
            }
            cout << endl;
        }
        cout << "Result for string \"" << test << "\": " 
             << (acceptedOverall ? "Accepted" : "Rejected") << "\n" << endl;
        int x;
        cout<<"want to continue enter 1 else 0"<<endl;
        cin>>x;
        if(x == 0)return 0;
    }

    return 0;
}
