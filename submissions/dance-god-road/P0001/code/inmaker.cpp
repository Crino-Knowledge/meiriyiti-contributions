#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct Row
{
    int level10;
    int rank;
    int version;
    int id;
};

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);

    const vector<string> ranks = {
        "SSS+", "SSS", "SS+", "SS", "S+", "S", "AAA",
        "AA", "A", "BBB", "BB", "B", "C", "D"};
    string mode = argc > 1 ? argv[1] : "random";
    vector<Row> rows;

    auto fixed_level = [](int id)
    {
        return 10 + id * 37 % 141;
    };
    auto fixed_version = [](int id)
    {
        return id % 2;
    };
    auto add_random_charts = [&](int n, int chart_count)
    {
        chart_count = max(1, min(n, chart_count));
        for (int i = 0; i < n; i++)
        {
            int id = 1 + i % chart_count;
            rows.push_back({fixed_level(id), rnd.next(0, 13), fixed_version(id), id});
        }
    };

    if (mode == "min")
    {
        rows.push_back({10, 13, 0, 1});
    }
    else if (mode == "floor")
    {
        rows = {
            {131, 4, 1, 1}, {139, 2, 1, 2}, {108, 13, 0, 3},
            {122, 12, 0, 4}, {145, 9, 1, 5}, {101, 13, 1, 6},
            {149, 6, 0, 7}, {137, 5, 1, 8}};
    }
    else if (mode == "old-fill")
    {
        for (int i = 0; i < 35; i++)
        {
            rows.push_back({100 + i, 13, 0, i + 1});
        }
    }
    else if (mode == "old-tie")
    {
        for (int i = 0; i < 36; i++)
        {
            rows.push_back({100, 13, 0, i + 1});
        }
    }
    else if (mode == "old-replace")
    {
        for (int i = 0; i < 40; i++)
        {
            rows.push_back({100 + i, 13, 0, i + 1});
        }
    }
    else if (mode == "new-fill")
    {
        for (int i = 0; i < 15; i++)
        {
            rows.push_back({100 + i, 13, 1, i + 1});
        }
    }
    else if (mode == "new-replace")
    {
        for (int i = 0; i < 20; i++)
        {
            rows.push_back({100 + i, 13, 1, i + 1});
        }
    }
    else if (mode == "split")
    {
        for (int i = 0; i < 40; i++)
        {
            rows.push_back({150, 0, 0, i + 1});
        }
        for (int i = 0; i < 20; i++)
        {
            rows.push_back({50, 5, 1, i + 41});
        }
    }
    else if (mode == "all-rank")
    {
        for (int r = 0; r < 14; r++)
        {
            rows.push_back({100 + 2 * r, r, 0, 2 * r + 1});
            rows.push_back({101 + 2 * r, 13 - r, 1, 2 * r + 2});
        }
    }
    else if (mode == "precision")
    {
        for (int i = 0; i < 12; i++)
        {
            rows.push_back({131, 4, i % 2, i + 1});
        }
    }
    else if (mode == "zero")
    {
        for (int i = 0; i < 50; i++)
        {
            rows.push_back({10, 13, i % 2, i + 1});
        }
    }
    else if (mode == "max")
    {
        for (int i = 0; i < 35; i++)
        {
            rows.push_back({150, 0, 0, i + 1});
        }
        for (int i = 0; i < 15; i++)
        {
            rows.push_back({150, 0, 1, i + 36});
        }
    }
    else if (mode == "ties")
    {
        for (int i = 0; i < 80; i++)
        {
            if (i % 2 == 0)
            {
                rows.push_back({125, 3, i % 4 == 0 ? 0 : 1, i + 1});
            }
            else
            {
                rows.push_back({130, 5, i % 4 == 1 ? 1 : 0, i + 1});
            }
        }
    }
    else if (mode == "interleave")
    {
        add_random_charts(200, 30);
    }
    else if (mode == "sparse")
    {
        for (int i = 0; i < 120; i++)
        {
            int id = 1 + i % 20;
            int rank = i % 11 == 10 ? 0 : 13;
            rows.push_back({fixed_level(id), rank, fixed_version(id), id});
        }
    }
    else if (mode == "grind")
    {
        for (int i = 0; i < 300; i++)
        {
            int id = 1 + i % 12;
            int round = i / 12;
            int rank = max(0, 13 - round);
            if (i % 7 == 0)
            {
                rank = min(13, rank + 2);
            }
            rows.push_back({fixed_level(id), rank, fixed_version(id), id});
        }
    }
    else if (mode == "stair")
    {
        for (int i = 0; i < 15; i++)
        {
            rows.push_back({50, 13, 1, i + 1});
        }
        for (int i = 0; i < 20; i++)
        {
            rows.push_back({100 + i, 0, 1, i + 16});
        }
    }
    else if (mode == "stress")
    {
        int n = rnd.next(1, 500);
        add_random_charts(n, 60);
    }
    else
    {
        int n = argc > 2 ? atoi(argv[2]) : 100;
        if (mode == "performance")
        {
            n = 200000;
        }
        add_random_charts(n, mode == "performance" ? 5000 : 500);
    }

    cout << rows.size() << "\n";
    for (const Row &row : rows)
    {
        cout << row.level10 / 10 << "." << row.level10 % 10 << " "
             << ranks[row.rank] << " " << row.version << " " << row.id << "\n";
    }
    return 0;
}
