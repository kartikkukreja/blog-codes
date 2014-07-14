#include <cstdio>
#include <climits>
#include <cstring>
#include <vector>
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

typedef vector <Edge*> adjList;

int main()  {
    int V, E, i, u, v, cost, *dist, *edgeTo;
    bool *marked;
    Edge *tmp;
    adjList *G;

    // Assuming vertices are labeled 0...V-1
    //freopen("input1.txt", "r", stdin);

    scanf("%d %d", &V, &E); // Enter the number of vertices and edges
    G = new adjList[V];
    for(i=0; i<E; i++)  {   // Enter E undirected edges (u, v, weight)
        scanf("%d %d %d", &u, &v, &cost);
        tmp = new Edge;
        tmp->to = v; tmp->weight = cost;
        G[u].push_back(tmp);    // add edge to adjacency list of u
        tmp = new Edge;
        tmp->to = u; tmp->weight = cost;
        G[v].push_back(tmp);    // add edge to adjacency list of v
    }

    dist = new int[V];
    marked = new bool[V];
    for(i=1; i<V; i++)
        dist[i] = INT_MAX;
    dist[0] = 0;
    edgeTo = new int[V];
    memset(marked, false, V*sizeof(bool));

    MinIndexedPQ pq(V);
    pq.insert(0, 0);

    while(!pq.isEmpty())    {
        u = pq.deleteMin();
        marked[u] = true;
        for(adjList::iterator it = G[u].begin(); it != G[u].end(); it++)    {
            v = (*it)->to;
            if(marked[v]) continue;
            if((*it)->weight < dist[v])    {
                dist[v] = (*it)->weight;
                edgeTo[v] = u;
                if(pq.contains(v)) pq.decreaseKey(v, dist[v]);
                else pq.insert(v, dist[v]);
            }
        }
    }

    printf("Edges in MST :\n");
    for(i=1, cost=0; i<V; i++)  {
        cost += dist[i];
        printf("%d %d %d\n", edgeTo[i], i, dist[i]);
    }
    printf("Total cost of MST : %d\n", cost);

    delete [] dist;
    delete [] edgeTo;
    delete [] marked;
    delete [] G;

    return 0;
}