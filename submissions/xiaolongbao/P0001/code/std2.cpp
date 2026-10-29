#include <bits/stdc++.h>
using namespace std;

// 与 std.cpp 的欧几里得除法不同：这里使用二进制 gcd（Stein 算法），
// 通过移位和减法求最大公因数。
long long gcdBinary(long long a, long long b)
{
    if (a == 0)
    {
        return b;
    }
    if (b == 0)
    {
        return a;
    }

    int shift = 0;
    while (((a | b) & 1) == 0)
    {
        a >>= 1;
        b >>= 1;
        shift++;
    }
    while ((a & 1) == 0)
    {
        a >>= 1;
    }
    do
    {
        while ((b & 1) == 0)
        {
            b >>= 1;
        }
        if (a > b)
        {
            swap(a, b);
        }
        b -= a;
    } while (b != 0);

    return a << shift;
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
        g = gcdBinary(g, a);
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
