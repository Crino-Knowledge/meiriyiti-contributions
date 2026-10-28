// 思路层错解：先把编号排序，认为羁绊指数最大的 CP 一定相邻，只比较排序后相邻的两项。
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
        sort(a.begin(), a.end());

        long long bestValue = -1;
        long long bestA = a[0];
        long long bestB = a[1];
        for (int i = 0; i + 1 < q; i++)
        {
            long long g = gcdll(a[i], a[i + 1]);
            long long value = (a[i] / g) * (a[i + 1] / g);
            if (value > bestValue)
            {
                bestValue = value;
                bestA = a[i];
                bestB = a[i + 1];
            }
        }
        cout << min(bestA, bestB) << " " << max(bestA, bestB) << " " << bestValue << endl;
    }
    return 0;
}
