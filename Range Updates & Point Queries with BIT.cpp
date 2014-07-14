#include <cstdio>
#include <cstring>
#define LSOne(S) (S & (-S))
using namespace std;

typedef long long ll;

// Fenwick tree
// Only the updates are stored. Original array entries are not
// stored and are assumed to be 0. If the original array entries
// are non-zero, we can store them in another array A and modify
// point query to return query(b) + A[b].
ll ft[10005];	

// Array size
int N;

// Point query: Returns the value at position b in the array
ll query(int b)	{
	ll sum = 0;
	for (; b; b -= LSOne(b)) sum += ft[b];
	return sum;
}

// Point update: Adds v to the value at position k in the array
void update(int k, int v) {
	for (; k <= N; k += LSOne(k)) ft[k] += v;
}

// Range update: Adds v to each element in [i...j] in the array
void range_update(int i, int j, int v)	{
	update(i, v);
	update(j + 1, -v);
}

int main()  {
	int T, U, Q, i, l, r, val;
	
	scanf("%d", &T);
	while (T--)	{
		scanf("%d %d", &N, &U);
		memset(ft, 0, (N+1) * sizeof(ll));
		
		// U -> no. of updates
		for (i = 0; i < U; i++)	{
			// l and r are zero-based indices
			scanf("%d %d %d", &l, &r, &val);
			range_update(l + 1, r + 1, val);
		}
		// Q -> no. of queries
		scanf("%d", &Q);
		for (i = 0; i < Q; i++)	{
			// val is zero-based index
			scanf("%d", &val);
			printf("%lld\n", query(val + 1));
		}
	}

    return 0;
}