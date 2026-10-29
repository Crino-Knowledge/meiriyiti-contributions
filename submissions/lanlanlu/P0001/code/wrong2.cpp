#include <bits/stdc++.h>
using namespace std;

// 思路层错解：误以为符卡可以使用任意多次，并且完全忽略了物理攻击。
int main()
{
    int t;
    cin >> t;
    vector<int> h(t);
    int maxH = 0;
    for (int i = 0; i < t; i++)
    {
        cin >> h[i];
        maxH = max(maxH, h[i]);
    }

    vector<bool> isPrime(maxH + 1, true);
    isPrime[0] = false;
    isPrime[1] = false;
    for (int i = 2; 1LL * i * i <= maxH; i++)
    {
        if (!isPrime[i])
        {
            continue;
        }
        for (long long j = 1LL * i * i; j <= maxH; j += i)
        {
            isPrime[(int)j] = false;
        }
    }

    for (int i = 0; i < t; i++)
    {
        int rest = h[i];
        int attacks = 0;
        while (rest > 0)
        {
            int x = rest;
            while (x >= 2 && !isPrime[x])
            {
                x--;
            }
            if (x < 2)
            {
                attacks = -1;
                break;
            }
            rest -= x;
            attacks++;
        }
        cout << attacks << endl;
    }
    return 0;
}
