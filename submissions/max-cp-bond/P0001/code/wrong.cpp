// 思路层错解：认为最优 CP 一定包含档案编号最大的角色，只检查与最大值配对的组合。
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
        int pos = 0;
        for (int i = 0; i < q; i++)
        {
            cin >> a[i];
            if (a[i] > a[pos])
            {
                pos = i;
            }
        }

        long long bestValue = -1;
        long long bestA = a[pos];
        long long bestB = a[pos == 0 ? 1 : 0];
        for (int j = 0; j < q; j++)
        {
            if (j == pos)
            {
                continue;
            }
            long long g = gcdll(a[pos], a[j]);
            long long value = (a[pos] / g) * (a[j] / g);
            if (value > bestValue)
            {
                bestValue = value;
                bestA = a[pos];
                bestB = a[j];
            }
        }
        cout << min(bestA, bestB) << " " << max(bestA, bestB) << " " << bestValue << endl;
    }
    return 0;
}
