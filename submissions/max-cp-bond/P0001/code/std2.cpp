#include <bits/stdc++.h>
using namespace std;

const int LIMIT = 44722;

// 羁绊指数 S(a,b) = lcm/gcd = 把 a、b 分解质因数后，逐个质因子取指数之差的绝对值再相乘。
// 本实现完全不调用 gcd，也不做除法，走的是与 std.cpp 不同的算法路线。
long long bondByFactors(const vector<pair<int, int>> &f, const vector<pair<int, int>> &g)
{
    long long value = 1;
    size_t i = 0;
    size_t j = 0;
    while (i < f.size() || j < g.size())
    {
        if (j == g.size() || (i < f.size() && f[i].first < g[j].first))
        {
            for (int t = 0; t < f[i].second; t++)
            {
                value *= f[i].first;
            }
            i++;
        }
        else if (i == f.size() || g[j].first < f[i].first)
        {
            for (int t = 0; t < g[j].second; t++)
            {
                value *= g[j].first;
            }
            j++;
        }
        else
        {
            int diff = abs(f[i].second - g[j].second);
            for (int t = 0; t < diff; t++)
            {
                value *= f[i].first;
            }
            i++;
            j++;
        }
    }
    return value;
}

int main()
{
    vector<bool> composite(LIMIT + 1, false);
    vector<int> primes;
    for (int i = 2; i <= LIMIT; i++)
    {
        if (composite[i])
        {
            continue;
        }
        primes.push_back(i);
        for (long long j = 1LL * i * i; j <= LIMIT; j += i)
        {
            composite[j] = true;
        }
    }

    int n;
    cin >> n;
    for (int k = 0; k < n; k++)
    {
        int q;
        cin >> q;
        vector<long long> a(q);
        vector<vector<pair<int, int>>> factor(q);
        for (int i = 0; i < q; i++)
        {
            cin >> a[i];
            long long x = a[i];
            for (int p : primes)
            {
                if (1LL * p * p > x)
                {
                    break;
                }
                if (x % p == 0)
                {
                    int e = 0;
                    while (x % p == 0)
                    {
                        x /= p;
                        e++;
                    }
                    factor[i].push_back(make_pair(p, e));
                }
            }
            if (x > 1)
            {
                factor[i].push_back(make_pair((int)x, 1));
            }
        }

        long long bestValue = -1;
        long long bestA = a[0];
        long long bestB = a[1];
        for (int i = 0; i < q; i++)
        {
            for (int j = i + 1; j < q; j++)
            {
                long long value = bondByFactors(factor[i], factor[j]);
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
