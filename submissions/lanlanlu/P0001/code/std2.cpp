#include <bits/stdc++.h>
using namespace std;

// 与 std.cpp 的埃氏筛不同：这里使用线性筛（欧拉筛）预处理质数。
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

    vector<int> primes;
    vector<bool> composite(maxH + 1, false);
    for (int i = 2; i <= maxH; i++)
    {
        if (!composite[i])
        {
            primes.push_back(i);
        }
        for (int p : primes)
        {
            long long v = 1LL * p * i;
            if (v > maxH)
            {
                break;
            }
            composite[(int)v] = true;
            if (i % p == 0)
            {
                break;
            }
        }
    }

    for (int i = 0; i < t; i++)
    {
        int best = -1;
        long long physical = 0;
        int m = 0;
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
            else if (rest >= 2 && !composite[(int)rest])
            {
                if (best == -1 || m + 1 < best)
                {
                    best = m + 1;
                }
            }
            physical = physical * 2 + 1;
            m++;
            if (best != -1 && best <= m)
            {
                break;
            }
        }
        cout << best << '\n';
    }
    return 0;
}
