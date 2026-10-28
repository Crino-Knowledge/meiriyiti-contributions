#include "testlib.h"
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

int main(int argc, char *argv[])
{
    registerTestlibCmd(argc, argv);

    int n = inf.readInt(1, 10, "n");
    vector<vector<int>> roles(n);
    for (int k = 0; k < n; k++)
    {
        int q = inf.readInt(2, 2000, "q");
        roles[k] = inf.readInts(q, 1, 2000000000, "a");
        sort(roles[k].begin(), roles[k].end());
    }

    // 官方答案只提供可审计的最优羁绊指数，不限制选手选择哪一对。
    vector<long long> best(n, 0);
    for (int k = 0; k < n; k++)
    {
        long long juryA = ans.readInt(1, 2000000000, "jury a");
        long long juryB = ans.readInt(1, 2000000000, "jury b");
        long long juryValue = ans.readLong(1LL, 4000000000000000000LL, "jury S");
        long long g = gcdll(juryA, juryB);
        if (g == 0 || (juryA / g) * (juryB / g) != juryValue)
        {
            quitf(_fail, "第 %d 部作品的官方答案自相矛盾", k + 1);
        }
        best[k] = juryValue;
    }
    if (!ans.seekEof())
    {
        quitf(_fail, "官方答案包含多余内容");
    }

    for (int k = 0; k < n; k++)
    {
        long long a = ouf.readLong(1LL, 2000000000LL, "a");
        long long b = ouf.readLong(1LL, 2000000000LL, "b");
        long long s = ouf.readLong(1LL, 4000000000000000000LL, "S");

        if (a == b)
        {
            quitf(_wa, "第 %d 部作品选出的两位角色编号相同：%lld", k + 1, a);
        }
        if (!binary_search(roles[k].begin(), roles[k].end(), (int)a))
        {
            quitf(_wa, "第 %d 部作品的候选角色中没有编号 %lld", k + 1, a);
        }
        if (!binary_search(roles[k].begin(), roles[k].end(), (int)b))
        {
            quitf(_wa, "第 %d 部作品的候选角色中没有编号 %lld", k + 1, b);
        }

        long long g = gcdll(a, b);
        long long realValue = (a / g) * (b / g);
        if (s != realValue)
        {
            quitf(_wa, "第 %d 部作品：S(%lld,%lld) 应为 %lld，输出为 %lld", k + 1, a, b, realValue, s);
        }
        if (s != best[k])
        {
            quitf(_wa, "第 %d 部作品：最大羁绊指数为 %lld，输出的 %lld 不是最大", k + 1, best[k], s);
        }
    }

    if (!ouf.seekEof())
    {
        quitf(_wa, "输出在 %d 部作品之后还有多余内容", n);
    }
    quitf(_ok, "accepted");
}
