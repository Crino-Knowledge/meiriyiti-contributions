#include <bits/stdc++.h>
using namespace std;

// 实现层错解：把 m 次“吃吃吃”的总伤害写成了 2^m，而不是 2^m - 1。
int main()
{
    int t;
    cin >> t;
    vector<int> h(t);
    int maxH = 0;
    for (int i = 0; i < t; i++)
    {
        cin >> h[i];
        maxH = max(maxH, h[i]);
    }

    vector<bool> isPrime(maxH + 1, true);
    isPrime[0] = false;
    isPrime[1] = false;
    for (int i = 2; 1LL * i * i <= maxH; i++)
    {
        if (!isPrime[i])
        {
            continue;
        }
        for (long long j = 1LL * i * i; j <= maxH; j += i)
        {
            isPrime[(int)j] = false;
        }
    }

    for (int i = 0; i < t; i++)
    {
        int best = -1;
        if (isPrime[h[i]])
        {
            best = 1;
        }
        long long physical = 2;
        int m = 1;
        while (physical <= h[i])
        {
            long long rest = h[i] - physical;
            if (rest == 0)
            {
                if (best == -1 || m < best)
                {
                    best = m;
                }
            }
            else if (rest >= 2 && isPrime[(int)rest])
            {
                if (best == -1 || m + 1 < best)
                {
                    best = m + 1;
                }
            }
            physical = physical * 2;
            m++;
        }
        cout << best << endl;
    }
    return 0;
}
