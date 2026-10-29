#include <bits/stdc++.h>
using namespace std;

// 实现层错解：把最小的容量当成所有容量的最大公因数。
int main()
{
    int n;
    cin >> n;
    long long minimum = 1000000000000000000LL;
    for (int i = 0; i < n; i++)
    {
        long long a;
        cin >> a;
        minimum = min(minimum, a);
    }

    if (minimum > 1)
    {
        cout << "YES" << endl;
    }
    else
    {
        cout << "NO" << endl;
    }
    return 0;
}
