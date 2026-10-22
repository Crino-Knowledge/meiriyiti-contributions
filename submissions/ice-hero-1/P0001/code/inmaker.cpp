#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// inmaker <mode>，把单个测试点写到 stdout。seed 由 registerGen 根据命令行参数确定。
int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    string mode = argc > 1 ? argv[1] : "random";
    int n = 200;
    string top;
    string bottom;

    if (mode == "open")
    {
        n = 100000;
        top.assign(n, '0');
        bottom.assign(n, '0');
    }
    else if (mode == "corridor")
    {
        n = 100000;
        top.assign(n, '0');
        bottom.assign(n, '1');
        bottom[n - 1] = '0';
    }
    else if (mode == "switch")
    {
        n = 100000;
        top.assign(n, '0');
        bottom.assign(n, '0');
        for (int i = 0; i < n; i++)
        {
            if (i % 4 == 3)
            {
                top[i] = '1';
            }
            if (i % 4 == 1)
            {
                bottom[i] = '1';
            }
        }
    }
    else if (mode == "wall")
    {
        n = 100000;
        top.assign(n, '0');
        bottom.assign(n, '0');
        top[n - 2] = '1';
        bottom[n - 2] = '1';
    }
    else
    {
        n = rnd.next(1, 300);
        top.assign(n, '0');
        bottom.assign(n, '0');
        for (int i = 0; i < n; i++)
        {
            if (rnd.next(100) < 35)
            {
                top[i] = '1';
            }
            if (rnd.next(100) < 35)
            {
                bottom[i] = '1';
            }
        }
    }

    top[0] = '0';
    bottom[n - 1] = '0';

    cout << n << "\n";
    cout << top << "\n";
    cout << bottom << "\n";
    return 0;
}
