#include <cstdio>
#include <queue>
#include <vector>
#include <cstdlib>
using namespace std;
typedef pair<int,int> pii;

vector< pii > G[10001];
bool finished[10001];

int main() {
	int N, M, u, v, c;
	long long cost;
	
	scanf("%d %d", &N, &M);
	while (M--) {
		scanf("%d %d %d", &u, &v, &c);
		G[u].push_back(make_pair(v, c));
		G[v].push_back(make_pair(u, c));
	}
	
	priority_queue< pii, vector< pii >, greater< pii > > Q;
	Q.push(make_pair(0, 1));	// insert vertex 1 with cost 0
	cost = 0;
	while (!Q.empty()) {
		pii top = Q.top(); Q.pop();
		if (!finished[top.second]) {
			cost += top.first;
			finished[top.second] = true;
		}
		for (vector< pii >::iterator it = G[top.second].begin(); it != G[top.second].end(); ++it)
			if (!finished[it->first])
				Q.push(make_pair(it->second, it->first));
	}
	printf("%lld\n", cost);
	
	return 0;
}
