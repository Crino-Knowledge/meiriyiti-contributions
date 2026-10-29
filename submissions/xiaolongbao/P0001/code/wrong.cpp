#include <bits/stdc++.h>
using namespace std;

// 思路层错解：只检查所有蒸笼容量是否都是偶数，忽略了其他公因数。
int main()
{
    int n;
    cin >> n;
    bool allEven = true;
    for (int i = 0; i < n; i++)
    {
        long long a;
        cin >> a;
        if (a % 2 != 0)
        {
            allEven = false;
        }
    }

    if (allEven)
    {
        cout << "YES" << endl;
    }
    else
    {
        cout << "NO" << endl;
    }
    return 0;
}
