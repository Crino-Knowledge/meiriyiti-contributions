#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    // argv[1] 是模式，argv[2..4] 是规模参数；多出来的参数只参与 testlib 的种子计算，
    // 因此 stress 可以在末尾追加 {seed} 而不影响模式与规模。
    string mode = argc > 1 ? argv[1] : "random";
    vector<vector<int>> works;

    if (mode == "big")
    {
        int count = argc > 2 ? atoi(argv[2]) : 10;
        for (int k = 0; k < count; k++)
        {
            works.push_back(rnd.distinct(1000, 1, 2000000000));
        }
    }
    else if (mode == "dense")
    {
        int count = argc > 2 ? atoi(argv[2]) : 10;
        int limit = argc > 3 ? atoi(argv[3]) : 100000;
        for (int k = 0; k < count; k++)
        {
            works.push_back(rnd.distinct(1000, 1, limit));
        }
    }
    else
    {
        int maxWorks = argc > 2 ? atoi(argv[2]) : 3;
        int maxQ = argc > 3 ? atoi(argv[3]) : 8;
        int maxValue = argc > 4 ? atoi(argv[4]) : 1000;
        if (maxWorks < 1)
        {
            maxWorks = 1;
        }
        if (maxQ < 2)
        {
            maxQ = 2;
        }
        if (maxQ > maxValue)
        {
            maxQ = maxValue;
        }
        int count = rnd.next(1, maxWorks);
        for (int k = 0; k < count; k++)
        {
            int q = rnd.next(2, maxQ);
            vector<int> values;
            set<int> used;
            while ((int)values.size() < q)
            {
                int value;
                int kind = rnd.next(0, 3);
                if (kind == 0)
                {
                    value = rnd.next(1, min(maxValue, 30));
                }
                else if (kind == 1 && maxValue >= 12)
                {
                    int base = rnd.next(1, 12);
                    value = base * rnd.next(1, maxValue / base);
                }
                else
                {
                    value = rnd.next(1, maxValue);
                }
                if (value < 1 || value > maxValue)
                {
                    continue;
                }
                if (used.insert(value).second)
                {
                    values.push_back(value);
                }
            }
            shuffle(values.begin(), values.end());
            works.push_back(values);
        }
    }

    cout << works.size() << "\n";
    for (size_t k = 0; k < works.size(); k++)
    {
        cout << works[k].size() << "\n";
        for (size_t i = 0; i < works[k].size(); i++)
        {
            cout << works[k][i] << (i + 1 == works[k].size() ? '\n' : ' ');
        }
    }
    return 0;
}
