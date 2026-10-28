// 实现层错解：担心答案超过 int，于是改用 double 保存和比较羁绊指数，最后再转回整数输出。
// double 只有 53 位有效位，1e18 附近的答案会被舍入。
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

        double bestValue = -1.0;
        long long bestA = a[0];
        long long bestB = a[1];
        for (int i = 0; i < q; i++)
        {
            for (int j = i + 1; j < q; j++)
            {
                long long g = gcdll(a[i], a[j]);
                double value = (double)(a[i] / g) * (double)(a[j] / g);
                if (value > bestValue)
                {
                    bestValue = value;
                    bestA = a[i];
                    bestB = a[j];
                }
            }
        }
        cout << min(bestA, bestB) << " " << max(bestA, bestB) << " " << (long long)bestValue << endl;
    }
    return 0;
}
