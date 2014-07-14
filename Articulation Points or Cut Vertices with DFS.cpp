#include <cstdio>
#include <vector>
#include <list>
#include <cstring>
using namespace std;

// Class to represent an undirected graph, with vertices labeled
// from 0 to V-1, where V is the number of vertices
class Graph {
private:
	// V is the number of vertices in the graph
	// time is used to determine when a node has a back-link to one of its ancestors
    int V, time;
    
	// adjList[u] is the adjacency list of vertex u, 0 <= u < V
    vector <int> *adjList;
    
	// explored[u] is true if u has been explored
	// articulation_point[u] is true is u is an articulation point
    bool *explored, *articulation_point, done;
    
	// disc_time[u] is the time at which vertex u was explored
	// parent[u] = v if in the dfs tree, there is an edge from v to u
	// low[u] is the time of the earliest explored vertex reachable from u
	// If low[u] < disc_time[u], then there is a back-link from some node in the 
	// subtree rooted at u to some ancestor of u
    int *disc_time, *parent, *low;
    
        // articulation_points stores the articulation points/cut vertices in the graph
    vector <int> articulation_points;

    void dfsUtil(int u) {
        explored[u] = true;
        int num_child = 0;
        disc_time[u] = low[u] = ++time;
        
        for (vector <int>::iterator v = adjList[u].begin(); v != adjList[u].end(); v++)	{
            if (!explored[*v])	{
            	num_child++;
            	parent[*v] = u;
                dfsUtil(*v);
                low[u] = min(low[u], low[*v]);
                
		// u is an articulation point iff
		// 1. It the the root and has more than 1 child.
		// 2. It is not the root and no vertex in the subtree rooted at one of its
		//    children has a back-link to its ancestor.
		//    A child has a back-link to an ancestor of its parent when its low
		//    value is less than the discovery time of its parent.
                if (parent[u] == -1 && num_child > 1)
                	articulation_point[u] = true;
                else if (parent[u] != -1 && low[*v] >= disc_time[u])
                	articulation_point[u] = true;
            } else if (*v != parent[u])
            	low[u] = min(low[u], disc_time[*v]);
        }
    }

    void dfs()    {
        for (int u = 0; u < V; u++)
            if (!explored[u])
                dfsUtil(u);
    }

public:

    // create an empty undirected graph having V vertices
    Graph(int V) {
        this->V = V;
        time = 0;
        done = false;
        
        adjList = new vector <int>[V];
        explored = new bool[V];
        articulation_point = new bool[V];
        disc_time = new int[V];
        parent = new int[V];
        low = new int[V];
        
        memset(explored, false, V * sizeof(bool));
        memset(articulation_point, false, V * sizeof(bool));
        memset(parent, -1, V * sizeof (int));
    }

    ~Graph()    {
        delete [] adjList;
        delete [] articulation_point;
        delete [] explored;
        delete [] parent;
        delete [] disc_time;
        delete [] low;
    }

    // add an undirected edge (u, v) to the graph
    // returns false if either u or v is less than 0 or greater than equal to V
    // returns true if the edge was added to the digraph
    bool addEdge(int u, int v)  {
        if (u < 0 || u >= V) return false;
        if (v < 0 || v >= V) return false;
        adjList[u].push_back(v);
        adjList[v].push_back(u);
        return true;
    }
    
    // Performs dfs over the graph and returns a vector containing
    // the articulation points
    vector <int> getArticulationPoints()	{
    	if (done)
    		return articulation_points;
    	dfs();
    	done = true;
    	for (int u = 0; u < V; u++)
    		if (articulation_point[u])
    			articulation_points.push_back(u);
    	return articulation_points;
    }
};

int main() {
    Graph G(5);

    G.addEdge(1, 0);
    G.addEdge(1, 2);
    G.addEdge(2, 0);
    G.addEdge(0, 3);
    G.addEdge(3, 4);

    vector <int> articulationPoints = G.getArticulationPoints();
    printf("Graph has %d articulation points given below :\n", articulationPoints.size());
    for (vector <int> :: iterator u = articulationPoints.begin(); u != articulationPoints.end(); u++)
    	printf("%d ", *u);
    printf("\n");

    return 0;
}