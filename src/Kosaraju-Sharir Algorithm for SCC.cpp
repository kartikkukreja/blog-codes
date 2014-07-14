#include <cstdio>
#include <vector>
#include <cstring>
#define MAXV 1000
using namespace std;

typedef vector <int> vi;

// Assuming vertices are labeled 0...V-1
vi G[MAXV], Grev[MAXV];
bool explored[MAXV];
int leader[MAXV], finish[MAXV], order[MAXV], t = -1, parent = 0, V, E;

// running DFS on the reverse graph
void dfs_reverse(int i) {
    explored[i] = true;
    for(vi::iterator it = Grev[i].begin(); it != Grev[i].end(); it++)
        if(!explored[*it])
            dfs_reverse(*it);
    t++;
    finish[i] = t;
}

// running DFS on the actual graph
void dfs(int i) {
    explored[i] = true;
    leader[i] = parent;
    for(vi::iterator it = G[i].begin(); it != G[i].end(); it++)
        if(!explored[*it])
            dfs(*it);
}

// check if u & v are in the same connected component
bool stronglyConnected(int u, int v)    {
    return leader[u] == leader[v];
}

int main()  {
    int i, u, v, countCC, Q;

    //freopen("input.txt", "r", stdin);

    scanf("%d %d", &V, &E); // Enter the number of vertices & edges
    for(i=0; i<E; i++)  {   // Enter E edges : u -> v
        scanf("%d %d", &u, &v);
        G[u].push_back(v);  // Insert into the adjacency list of the graph
        Grev[v].push_back(u);   // and the reverse graph
    }

    printf("Original graph :\n");
    for(i=0; i<V; i++)  {
        if(!G[i].empty())   {
            printf("%d : ", i);
            for(vi::iterator it = G[i].begin(); it != G[i].end(); it++)
                printf("%d ", *it);
            printf("\n");
        }
    }

    printf("Reverse graph :\n");
    for(i=0; i<V; i++)  {
        if(!Grev[i].empty())   {
            printf("%d : ", i);
            for(vi::iterator it = Grev[i].begin(); it != Grev[i].end(); it++)
                printf("%d ", *it);
            printf("\n");
        }
    }

    // run dfs on the reverse graph to get reverse postorder
    memset(explored, false, V*sizeof(bool));
    for(i=0; i<V; i++)  {
        if(!explored[i])
            dfs_reverse(i);
        order[finish[i]] = i;
    }

    // run dfs on the actual graph in reverse postorder
    memset(explored, false, V*sizeof(bool));
    countCC = 0;
    for(i=V-1; i>=0; i--)
        if(!explored[order[i]]) {
            parent = order[i];
            dfs(order[i]);
            countCC++;
        }

    printf("No. of strongly connected components : %d\n", countCC);
    scanf("%d", &Q); // Enter number of SCC queries
    while(Q--)  {
        scanf("%d %d", &u, &v);
        stronglyConnected(u, v) ? printf("YES\n") : printf("NO\n");
    }
    return 0;
}