#include <bits/stdc++.h>
using namespace std;

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
    long long g = 0;
    for (int i = 0; i < n; i++)
    {
        long long a;
        cin >> a;
        g = gcdEuclid(g, a);
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
