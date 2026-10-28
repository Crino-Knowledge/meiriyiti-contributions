// 实现层错解：思路正确，但把羁绊指数存进 int，超过 2^31-1 时被截断。
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
    for (int k = 0; k < n; k++)
    {
        int q;
        cin >> q;
        vector<long long> a(q);
        for (int i = 0; i < q; i++)
        {
            cin >> a[i];
        }

        int bestValue = -1;
        long long bestA = a[0];
        long long bestB = a[1];
        for (int i = 0; i < q; i++)
        {
            for (int j = i + 1; j < q; j++)
            {
                long long g = gcdll(a[i], a[j]);
                long long product = (a[i] / g) * (a[j] / g);
                int value = (int)product;
                if (value > bestValue)
                {
                    bestValue = value;
                    bestA = a[i];
                    bestB = a[j];
                }
            }
        }
        cout << min(bestA, bestB) << " " << max(bestA, bestB) << " " << bestValue << endl;
    }
    return 0;
}
