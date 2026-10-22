#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);
    int n = inf.readInt(1, 1000, "n");
    inf.readSpace();
    int m = inf.readInt(1, 1000, "m");
    inf.readSpace();
    int k = inf.readInt(1, n * m - 1, "k");
    inf.readEoln();

    int count = 0;
    for (int i = 0; i < n; i++)
    {
        string row = inf.readLine("[.X]{1,1000}", "row");
        ensuref((int)row.size() == m, "row %d length must be m", i + 1);
        for (int j = 0; j < m; j++)
        {
            if (row[j] == 'X')
            {
                count++;
            }
        }
    }
    ensuref(count == k, "the number of X must be k");
    inf.readEof();
    return 0;
}
