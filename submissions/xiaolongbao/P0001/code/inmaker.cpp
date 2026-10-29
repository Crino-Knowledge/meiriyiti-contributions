#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    string mode = argc > 1 ? argv[1] : "random";
    vector<int> a;

    if (mode == "large_yes")
    {
        a.assign(100000, 1000000000);
    }
    else if (mode == "large_random")
    {
        a.push_back(1);
        for (int i = 1; i < 100000; i++)
        {
            a.push_back(rnd.next(1, 1000000000));
        }
    }
    else if (mode == "odd_common")
    {
        for (int i = 0; i < 100000; i++)
        {
            int k = 2 * rnd.next(1, 166666666) - 1;
            a.push_back(3 * k);
        }
    }
    else
    {
        int n = rnd.next(1, 30);
        for (int i = 0; i < n; i++)
        {
            a.push_back(rnd.next(1, 1000));
        }
    }

    cout << a.size() << "\n";
    for (int i = 0; i < (int)a.size(); i++)
    {
        if (i > 0)
        {
            cout << " ";
        }
        cout << a[i];
    }
    cout << "\n";
    return 0;
}
