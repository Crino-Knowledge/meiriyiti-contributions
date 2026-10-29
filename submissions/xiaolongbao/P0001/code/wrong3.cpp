#include <bits/stdc++.h>
using namespace std;

// 复杂度层错解：用减法实现 gcd，遇到相差很大的两个数时会执行大量减法。
long long gcdSubtract(long long a, long long b)
{
    while (a != b)
    {
        if (a > b)
        {
            a -= b;
        }
        else
        {
            b -= a;
        }
    }
    return a;
}

int main()
{
    int n;
    cin >> n;
    long long g = 0;
    for (int i = 0; i < n; i++)
    {
        long long a;
        cin >> a;
        if (g == 0)
        {
            g = a;
        }
        else
        {
            g = gcdSubtract(g, a);
        }
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
