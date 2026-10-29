#include <bits/stdc++.h>
using namespace std;

// 思路层错解：只求前两个容量的最大公因数，忽略了后面的蒸笼。
long long gcdEuclid(long long a, long long b)
{
    while (b != 0)
    {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main()
{
    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    long long g = a[0];
    if (n >= 2)
    {
        g = gcdEuclid(g, a[1]);
    }

    if (g > 1)
    {
        cout << "YES" << endl;
    }
    else
    {
        cout << "NO" << endl;
    }
    return 0;
}
