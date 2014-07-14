#include <cstdio>
#define MAX 1000000000ULL

typedef unsigned long long ull;

ull r, t;

bool infeasible(ull x) {
    return (x + 1)*(2*r + 2*x + 1) > t;
}

ull binary_search(ull hi)  {
    ull lo = 0, mid;
    while (lo < hi)  {
      mid = lo + (hi-lo)/2;
      if (infeasible(mid))
         hi = mid;
      else
         lo = mid+1;
    }
    if (!infeasible(lo))
      return -1;

    return lo;
}

int main()	{
    int T, i;
    ull ans, limit;

	freopen("A-large-practice.in", "r", stdin);
	freopen("A-large-practice.txt", "w", stdout);

	scanf("%d", &T);
	for(i=1; i<=T; i++)  {
        scanf("%llu %llu", &r, &t);
        limit = 1000000000000000000ULL / r;
        limit = (limit <= MAX) ? limit : MAX;
        ans = binary_search(limit);
        printf("Case #%d: %llu\n", i, ans);
	}

	return 0;
}