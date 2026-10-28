// 实现层错解：多测没有重置最优解，前一部作品的答案会串到后面的作品里。
#include <bits/stdc++.h>
using namespace std;

long long gcdll(long long a, long long b)
{
    while (b != 0)
    {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int main()
{
    int n;
    cin >> n;

    long long bestValue = -1;
    int bestI = 0;
    int bestJ = 1;

    for (int k = 0; k < n; k++)
    {
        int q;
        cin >> q;
        vector<long long> a(q);
        for (int i = 0; i < q; i++)
        {
            cin >> a[i];
        }

        for (int i = 0; i < q; i++)
        {
            for (int j = i + 1; j < q; j++)
            {
                long long g = gcdll(a[i], a[j]);
                long long value = (a[i] / g) * (a[j] / g);
                if (value > bestValue)
                {
                    bestValue = value;
                    bestI = i;
                    bestJ = j;
                }
            }
        }

        cout << min(a[bestI], a[bestJ]) << " " << max(a[bestI], a[bestJ]) << " " << bestValue << endl;
    }
    return 0;
}
