#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

const int MAXH = 10000000;

vector<bool> makeSieve(int n)
{
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = false;
    isPrime[1] = false;
    for (int i = 2; 1LL * i * i <= n; i++)
    {
        if (!isPrime[i])
        {
            continue;
        }
        for (long long j = 1LL * i * i; j <= n; j += i)
        {
            isPrime[(int)j] = false;
        }
    }
    return isPrime;
}

// 返回第一次出现合法解时使用的物理攻击次数；-1 表示无解。
int firstSolution(int h, const vector<bool>& isPrime)
{
    long long physical = 0;
    int m = 0;
    while (physical <= h)
    {
        long long rest = h - physical;
        if (rest == 0)
        {
            return m;
        }
        if (rest >= 2 && isPrime[(int)rest])
        {
            return m;
        }
        physical = physical * 2 + 1;
        m++;
    }
    return -1;
}

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    string mode = argc > 1 ? argv[1] : "random";
    vector<int> hs;

    if (mode == "prime")
    {
        vector<bool> isPrime = makeSieve(MAXH);
        vector<int> primes;
        for (int i = 9000000; i <= MAXH; i++)
        {
            if (isPrime[i])
            {
                primes.push_back(i);
            }
        }
        int t = 200000;
        for (int i = 0; i < t; i++)
        {
            hs.push_back(primes[rnd.next(0, (int)primes.size() - 1)]);
        }
    }
    else if (mode == "max")
    {
        int t = 200000;
        for (int i = 0; i < t; i++)
        {
            hs.push_back(rnd.next(1, MAXH));
        }
        hs[0] = 1;
        hs[1] = MAXH;
        hs[2] = MAXH - 1;
    }
    else if (mode == "impossible")
    {
        vector<bool> isPrime = makeSieve(MAXH);
        int t = 200000;
        hs.push_back(21);
        hs.push_back(51);
        hs.push_back(55);
        hs.push_back(57);
        hs.push_back(69);
        hs.push_back(75);
        while ((int)hs.size() < t)
        {
            int h = rnd.next(5000000, MAXH);
            if (firstSolution(h, isPrime) == -1)
            {
                hs.push_back(h);
            }
        }
    }
    else if (mode == "late")
    {
        vector<bool> isPrime = makeSieve(MAXH);
        int t = 200000;
        while ((int)hs.size() < t)
        {
            int h = rnd.next(5000000, MAXH);
            if (firstSolution(h, isPrime) >= 15)
            {
                hs.push_back(h);
            }
        }
    }
    else if (mode == "small")
    {
        int t = 200000;
        for (int i = 0; i < t; i++)
        {
            hs.push_back(rnd.next(1, 1000));
        }
    }
    else if (mode == "powers")
    {
        int base[7] = {15, 63, 255, 511, 1023, 2047, 4095};
        int t = 200000;
        for (int i = 0; i < t; i++)
        {
            hs.push_back(base[i % 7]);
        }
    }
    else
    {
        int t = rnd.next(5, 30);
        for (int i = 0; i < t; i++)
        {
            hs.push_back(rnd.next(1, 100000));
        }
        hs[0] = 1;
        hs[1] = 2;
        hs[2] = 3;
        hs[3] = 6;
        hs[4] = 15;
    }

    cout << hs.size() << "\n";
    for (int i = 0; i < (int)hs.size(); i++)
    {
        cout << hs[i] << "\n";
    }
    return 0;
}
