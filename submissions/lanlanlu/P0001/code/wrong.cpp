#include <bits/stdc++.h>
using namespace std;

// 思路层错解：只考虑“全程只用物理攻击”或“全程只用符卡”，忽略了混合使用。
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
        long long physical = 0;
        int m = 0;
        while (physical <= h[i])
        {
            if (physical == h[i])
            {
                if (best == -1 || m < best)
                {
                    best = m;
                }
            }
            physical = physical * 2 + 1;
            m++;
        }
        cout << best << endl;
    }
    return 0;
}
