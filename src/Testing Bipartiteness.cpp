#include <cstdio>
#include <vector>
#include <queue>
using namespace std;

enum Color {Uncolored, Red, Black};

// Adjacency list representation for graph
// Assumes vertices are labeled 0...V-1, where V is the number of vertices
class Graph {
public:
    vector <int> * adjList;
    int V;
    Graph(int V)    {
        adjList = new vector <int>[V];
        this->V = V;
    }
    void addUndirectedEdge(int u, int v)    {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }
};

// checks if graph G is bipartite. G is assumed to be connected
bool isBipartite(Graph& G, int s)  {
    Color * coloring = new Color[G.V];
    for (int i = 0; i < G.V; i++)
        coloring[i] = Uncolored;
    coloring[s] = Red;

    queue <int> q;
    q.push(s);

    while (!q.empty())  {
        int u = q.front();
        q.pop();
        for (vector <int>::iterator it = G.adjList[u].begin(); it != G.adjList[u].end(); it++)  {
            if (coloring[*it] == Uncolored) {
                coloring[*it] = (coloring[u] == Red) ? Black : Red;
                q.push(*it);
            } else if (coloring[*it] == coloring[u])    {
                delete [] coloring;
                return false;
            }
        }
    }
    delete [] coloring;
    return true;
}

int main()  {
    Graph G(5);

    G.addUndirectedEdge(0, 2);
    G.addUndirectedEdge(0, 3);
    G.addUndirectedEdge(0, 4);
    G.addUndirectedEdge(1, 2);
    G.addUndirectedEdge(1, 3);
    G.addUndirectedEdge(4, 3);

    if (isBipartite(G, 0))
        printf("Bipartite");
    else
        printf("Not bipartite");

    return 0;
}