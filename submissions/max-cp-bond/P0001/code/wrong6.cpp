// 复杂度层错解：逻辑正确，但每一对都用试除法分解质因数再拼出 gcd，
// 单次求 gcd 是 O(sqrt(1e9))，q = 2000 时每部作品要跑 2e6 对，在极限数据上必然超时。
#include <bits/stdc++.h>
using namespace std;

long long slowGcd(long long a, long long b)
{
    long long g = 1;
    long long x = a;
    long long y = b;
    for (long long p = 2; p * p <= x || p * p <= y; p++)
    {
        int cx = 0;
        int cy = 0;
        while (x % p == 0)
        {
            x /= p;
            cx++;
        }
        while (y % p == 0)
        {
            y /= p;
            cy++;
        }
        for (int t = 0; t < min(cx, cy); t++)
        {
            g *= p;
        }
    }
    if (x == y && x > 1)
    {
        g *= x;
    }
    return g;
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

        long long bestValue = -1;
        long long bestA = a[0];
        long long bestB = a[1];
        for (int i = 0; i < q; i++)
        {
            for (int j = i + 1; j < q; j++)
            {
                long long g = slowGcd(a[i], a[j]);
                long long value = (a[i] / g) * (a[j] / g);
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
