#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    int n = inf.readInt(1, 10, "n");
    inf.readEoln();

    for (int k = 1; k <= n; k++)
    {
        int q = inf.readInt(2, 1000, "q");
        inf.readEoln();

        vector<int> a(q);
        for (int i = 0; i < q; i++)
        {
            a[i] = inf.readInt(1, 2000000000, "a_i");
            if (i + 1 < q)
            {
                inf.readSpace();
            }
            else
            {
                inf.readEoln();
            }
        }

        vector<int> sorted = a;
        sort(sorted.begin(), sorted.end());
        for (int i = 1; i < q; i++)
        {
            ensuref(sorted[i] != sorted[i - 1],
                    "第 %d 部作品内的角色档案编号必须互不相同，出现了重复的 %d",
                    k, sorted[i]);
        }
    }

    inf.readEof();
    return 0;
}
