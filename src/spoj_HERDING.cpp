#include <cstdio>
#include <cstring>
#define MAXN 1000
#define MAXM 1000

int N, M;
char G[MAXN][MAXM+5];
int parent[MAXN][MAXM];

int dfs(int i, int j, int p) {
    parent[i][j] = p;
    
    switch (G[i][j]) {
		case 'N':
			--i; break;
		case 'S':
			++i; break;
		case 'E':
			++j; break;
		case 'W':
			--j; break;
	}
	
	if (parent[i][j] == -1)
		return dfs(i, j, p);
	return parent[i][j] == p;
}

int numComponents() {	
    int i, j, countCC = 0;
    
    memset(parent, -1, MAXN * MAXM * sizeof(int));
    for (i = 0; i < N; ++i) 
		  for (j = 0; j < M; ++j)
	      if (parent[i][j] == -1)
	        countCC += dfs(i, j, i*M+j);    
    return countCC;
}

int main()  {
    scanf("%d %d", &N, &M);
    for (int i = 0; i < N; ++i)
    	scanf("%s", G[i]);
    printf("%d\n", numComponents());    
    
    return 0;
}
