#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

// Men and women are represented by integers 1...N

// ManPref is the preference list of all men for all women.
// ManPref[m][i] = w iff w is at ith position in the preference list of m

// WomanPref is the preference list of all women for all men.
// WomanPref[w][i] = m iff m is at ith position in the preference list of w

// Ranking gives the ranking of each man in the preference list of each woman
// Ranking[w][m] = i iff WomanPref[w][i] = m

// Current gives the current engagement of each women
// Current[w] = m iff w is currently engaged to m

// Next gives the index of next woman to propose to in the preference list of each man
// Next[m] = i iff m has proposed to all w s.t. ManPref[m][j] = w for j = 1...i-1 but not ManPref[m][i]
int Ranking[505][505], ManPref[505][505], WomanPref[505][505], Next[505], Current[505];

int main()  {
    int T, N, i, j, m, w;

    // list of men who are not currently engaged
    queue <int> freeMen;

    scanf("%d", &T);    // No. of test cases
    while (T--) {
        scanf("%d", &N);    // No. of men and women
        for (i = 1; i <= N; i++) {
            scanf("%d", &w);    // Woman number (b/w 1 and N) followed by her preference list
            for (j = 1; j <= N; j++)
                scanf("%d", &WomanPref[w][j]);
        }
        for (i = 1; i <= N; i++) {
            scanf("%d", &m);    // Man number (b/w 1 and N) followed by his preference list
            for (j = 1; j <= N; j++)
                scanf("%d", &ManPref[m][j]);
        }

        for (i = 1; i <= N; i++)
            for (j = 1; j <= N; j++)
                Ranking[i][WomanPref[i][j]] = j;

        memset(Current, 0, (N + 1) * sizeof(int));

        for (i = 1; i <= N; i++) {
            freeMen.push(i);
            Next[i] = 1;
        }

        while (!freeMen.empty())    {
            m = freeMen.front();
            w = ManPref[m][Next[m]++];
            if (Current[w] == 0)   {
                Current[w] = m;
                freeMen.pop();
            } else if (Ranking[w][m] < Ranking[w][Current[w]])  {
                freeMen.pop();
                freeMen.push(Current[w]);
                Current[w] = m;
            }
        }

        // (m, w) pairs in the stable matching
        for (w = 1; w <= N; w++)
            printf("%d %d\n", Current[w], w);
    }

    return 0;
}