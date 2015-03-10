#include <cstdio>
#include <cmath>
#include <algorithm>
#include <bitset>
#define MAX 100000005
using namespace std;

bitset <MAX/2 + 1> prime;
int primes[5761460], num_primes;

void EratostheneSieve()	{
    int x = MAX / 2, y = (sqrt (MAX) - 1) / 2, i, j;
    for (i = 1; i <= y; i++) {
      if (prime[i] == 0)
         for (j = (i * (i + 1)) << 1; j <= x; j += (i << 1) + 1)
            prime[j] = 1;
    }
    prime[0] = 1;
    primes[0] = 2;
    for (i = 3, j = 1; i < MAX; i += 2)
        if (!prime[i >> 1])
            primes[j++] = i;
    num_primes = j;
}

int main()  {
    int x, count_primes;
    double num;

    EratostheneSieve();

    scanf("%d", &x);
    while (x)   {
        count_primes = upper_bound(primes, primes + num_primes, x) - primes;
        num = abs(count_primes - (x / log(x))) * 100;
        printf("%.1lf\n", num / count_primes);
        scanf("%d", &x);
    }

    return 0;
}
