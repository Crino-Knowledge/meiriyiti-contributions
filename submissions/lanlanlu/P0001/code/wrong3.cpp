#include <bits/stdc++.h>
using namespace std;

// 复杂度层错解：逻辑正确，但每个候选都用试除法判断质数，复杂度为 O(t log h sqrt(h))。
bool isPrime(long long x)
{
    if (x < 2)
    {
        return false;
    }
    for (long long d = 2; d * d <= x; d++)
    {
        if (x % d == 0)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    int t;
    cin >> t;
    vector<int> h(t);
    for (int i = 0; i < t; i++)
    {
        cin >> h[i];
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
            else if (rest >= 2 && isPrime(rest))
            {
                if (best == -1 || m + 1 < best)
                {
                    best = m + 1;
                }
            }
            physical = physical * 2 + 1;
            m++;
        }
        cout << best << endl;
    }
    return 0;
}
