/*
 *  Input file format:
 *      V E
 *      u1 v1 c1
 *      u2 v2 c2
 *          :
 *      ue ve ce
 *      s
 *      t1
 *      t2
 *      :
 *      -1
 *  The first line contains two integers V & E, the number of vertices and the number of edges respectively.
 *  E lines follow, each containing 3 integers ui, vi, ci representing the directed edge ui -> vi having cost ci.
 *  (E + 2)nd line contains an integer s, representing the source vertex.
 *  Each next line contains an integer t, the target vertex, which represents a query to find the shortest path from s to t.
 *  The queries end with a single line containing -1.
 *
 *  Output format:
 *      For each query t, the program writes two lines, the first describes the shortest path from s to t
 *      and the second describes the cost of the shortest path from s to t.
 */

#include <cstdio>
#include <climits>
#include <vector>
#include <stack>
using namespace std;

/*
 * Indexed min priority queue
 * Supports insertion in O(log N), deletion of any key (regardless of whether
 * the key is the minimum key or not) in O(log N) and changes to key values
 * in O(log N), where N is the number of
 * elements currently in the PQ
 */
class MinIndexedPQ {
    int NMAX, N, *heap, *index, *keys;

    void swap(int i, int j) {
        int t = heap[i]; heap[i] = heap[j]; heap[j] = t;
        index[heap[i]] = i; index[heap[j]] = j;
    }
    void bubbleUp(int k)    {
        while(k > 1 && keys[heap[k/2]] > keys[heap[k]])   {
            swap(k, k/2);
            k = k/2;
        }
    }
    void bubbleDown(int k)  {
        int j;
        while(2*k <= N) {
            j = 2*k;
            if(j < N && keys[heap[j]] > keys[heap[j+1]])
                j++;
            if(keys[heap[k]] <= keys[heap[j]])
                break;
            swap(k, j);
            k = j;
        }
    }
public:
    // Create an empty MinIndexedPQ which can contain atmost NMAX elements
    MinIndexedPQ(int NMAX)  {
        this->NMAX = NMAX;
        N = 0;
        keys = new int[NMAX + 1];
        heap = new int[NMAX + 1];
        index = new int[NMAX + 1];
        for(int i = 0; i <= NMAX; i++)
            index[i] = -1;
    }
    ~MinIndexedPQ() {
        delete [] keys;
        delete [] heap;
        delete [] index;
    }
    // check if the PQ is empty
    bool isEmpty()  {
        return N == 0;
    }
    // check if i is an index on the PQ
    bool contains(int i)    {
        return index[i] != -1;
    }
    // associate key with index i; 0 < i < NMAX
    void insert(int i, int key) {
        N++;
        index[i] = N;
        heap[N] = i;
        keys[i] = key;
        bubbleUp(N);
    }
    // delete the minimal key and return its associated index
    // Warning: Don't try to read from this index after calling this function
    int deleteMin() {
        int min = heap[1];
        swap(1, N--);
        bubbleDown(1);
        index[min] = -1;
        heap[N+1] = -1;
        return min;
    }
    // decrease the key associated with index i to the specified value
    void decreaseKey(int i, int key)    {
        keys[i] = key;
        bubbleUp(index[i]);
    }
};

// representation of directed edge to vertex 'to' having weight 'weight'
struct Edge {
    int to, weight;
};

typedef vector <Edge> adjList;

int main()  {
    int V, E, i, N, u, v, cost, *dist, *edgeTo, s;
    adjList *G;
    stack <int> path;

    // Assuming vertices are labeled 0...V-1
    freopen("input.txt", "r", stdin);

    scanf("%d %d", &V, &E); // Enter the number of vertices and edges
    G = new adjList[V];
    for(i=0; i<E; i++)  {   // Enter E directed edges u -> v having weight 'cost'
        scanf("%d %d %d", &u, &v, &cost);
        G[u].push_back((Edge){v, cost});    // add edge to adjacency list of u
    }
    scanf("%d", &s);    // Enter the source vertex

    dist = new int[V];
    for(i=0; i<V; i++)
        dist[i] = INT_MAX;
    dist[s] = 0;
    edgeTo = new int[V];
    edgeTo[s] = s;

    MinIndexedPQ pq(V);
    pq.insert(s, 0);

    while(!pq.isEmpty())    {
        u = pq.deleteMin();
        for(adjList::iterator it = G[u].begin(); it != G[u].end(); it++)    {
            v = it->to;
            cost = dist[u] + it->weight;
            if(cost < dist[v])    {
                dist[v] = cost;
                edgeTo[v] = u;
                if(pq.contains(v)) pq.decreaseKey(v, cost);
                else pq.insert(v, cost);
            }
        }
    }

    while(true) {
        scanf("%d", &v);    // Enter the target vertex v
        if(v < 0) break;

        u = v;
        while(edgeTo[u] != u)   {
            path.push(edgeTo[u]);
            u = edgeTo[u];
        }
        while(!path.empty())    {   // print path from s to v
            printf("%d-", path.top());
            path.pop();
        }
        printf("%d\n", v);
        printf("%d\n", dist[v]);    // print cost of shortest path from s to v
    }

    delete [] dist;
    delete [] edgeTo;
    delete [] G;

    return 0;
}