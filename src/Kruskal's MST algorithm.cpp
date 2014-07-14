#include <cstdio>
#include <algorithm>
using namespace std;

// representation of undirected edge (u, v) having weight 'weight'
struct Edge {
    int u, v, weight;
};

// Union-find data structure
class UF    {
    int *id, cnt, *sz;
public:
	// Create an empty union find data structure with N isolated sets.
    UF(int N)   {
        cnt = N;
	id = new int[N];
	sz = new int[N];
        for(int i=0; i<N; i++)	{
            id[i] = i;
  	    sz[i] = 1;
	}
    }
    ~UF()	{
	delete [] id;
	delete [] sz;
    }
	// Return the id of component corresponding to object p.
    int find(int p)	{
        int root = p;
        while (root != id[root])
            root = id[root];
        while (p != root) {
            int newp = id[p];
            id[p] = root;
            p = newp;
        }
        return root;
    }
	// Replace sets containing x and y with their union.
    void merge(int x, int y)	{
        int i = find(x);
        int j = find(y);
        if (i == j) return;
		
		// make smaller root point to larger one
        if   (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
        else                 { id[j] = i; sz[i] += sz[j]; }
        cnt--;
    }
	// Are objects x and y in the same set?
    bool connected(int x, int y)    {
        return find(x) == find(y);
    }
	// Return the number of disjoint sets.
    int count() {
        return cnt;
    }
};

// comparator function for sorting edges in ascending order by weight
bool comp(Edge *a, Edge *b)   {
    return a->weight < b->weight;
}

int main()  {
    int V, E, i, N, u, v, cost;
    Edge **edges, **mst;

    // Assuming vertices are labeled 0...V-1
    //freopen("input1.txt", "r", stdin);

    scanf("%d %d", &V, &E); // Enter the number of vertices and edges
    edges = new Edge*[E];
    for(i=0; i<E; i++)  {   // Enter E undirected edges (u, v, weight)
        edges[i] = new Edge;
        scanf("%d %d %d", &(edges[i]->u), &(edges[i]->v), &(edges[i]->weight));
    }
    sort(edges, edges + E, comp);

    UF uf(V);
    mst = new Edge*[V-1];
    for(N=i=cost=0; i<E && N<V-1; i++) {
        u = edges[i]->u; v = edges[i]->v;
        if(!uf.connected(u, v)) {
            uf.merge(u, v);
            mst[N++] = edges[i];
            cost += edges[i]->weight;
        }
    }

    printf("Total cost of MST : %d\n", cost);
    printf("Edges in MST :\n");
    for(i=0; i<N; i++)
        printf("%d %d %d\n", mst[i]->u, mst[i]->v, mst[i]->weight);

    return 0;
}