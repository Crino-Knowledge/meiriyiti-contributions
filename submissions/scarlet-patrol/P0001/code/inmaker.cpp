#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    string mode = argc > 1 ? argv[1] : "random";
    int n, m, k;
    vector<string> grid;

    if (mode == "corner")
    {
        n = argc > 2 ? atoi(argv[2]) : 10;
        m = argc > 3 ? atoi(argv[3]) : 10;
        k = 1;
        grid.assign(n, string(m, '.'));
        grid[0][0] = 'X';
    }
    else if (mode == "line")
    {
        n = 1;
        m = argc > 2 ? atoi(argv[2]) : 10;
        k = 1;
        grid.assign(n, string(m, '.'));
        grid[0][0] = 'X';
    }
    else if (mode == "many")
    {
        n = argc > 2 ? atoi(argv[2]) : 10;
        m = argc > 3 ? atoi(argv[3]) : 10;
        k = argc > 4 ? atoi(argv[4]) : 3;
        int total = n * m;
        if (k < 1)
        {
            k = 1;
        }
        if (k > total - 1)
        {
            k = total - 1;
        }
        grid.assign(n, string(m, '.'));
        vector<int> ids(total);
        for (int i = 0; i < total; i++)
        {
            ids[i] = i;
        }
        shuffle(ids.begin(), ids.end());
        for (int i = 0; i < k; i++)
        {
            int id = ids[i];
            grid[id / m][id % m] = 'X';
        }
    }
    else
    {
        n = argc > 3 ? atoi(argv[3]) : rnd.next(1, 6);
        m = argc > 4 ? atoi(argv[4]) : rnd.next(1, 6);
        while (n * m < 2)
        {
            n = rnd.next(1, 6);
            m = rnd.next(1, 6);
        }
        int total = n * m;
        k = argc > 5 ? atoi(argv[5]) : rnd.next(1, total - 1);
        if (k < 1)
        {
            k = 1;
        }
        if (k > total - 1)
        {
            k = total - 1;
        }
        grid.assign(n, string(m, '.'));
        vector<int> ids(total);
        for (int i = 0; i < total; i++)
        {
            ids[i] = i;
        }
        shuffle(ids.begin(), ids.end());
        for (int i = 0; i < k; i++)
        {
            int id = ids[i];
            grid[id / m][id % m] = 'X';
        }
    }

    cout << n << " " << m << " " << k << "\n";
    for (int i = 0; i < n; i++)
    {
        cout << grid[i] << "\n";
    }
    return 0;
}
