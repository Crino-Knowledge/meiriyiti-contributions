#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    const vector<string> ranks = {
        "SSS+", "SSS", "SS+", "SS", "S+", "S", "AAA",
        "AA", "A", "BBB", "BB", "B", "C", "D"};
    unordered_map<int, pair<int, int>> chart_info;
    int n = inf.readInt(1, 200000, "n");
    inf.readEoln();

    for (int i = 0; i < n; i++)
    {
        string level = inf.readToken("[0-9]{1,2}\\.[0-9]", "level");
        int dot = (int)level.find('.');
        int level10 = stoi(level.substr(0, dot)) * 10 + (level[dot + 1] - '0');
        ensuref(10 <= level10 && level10 <= 150,
                "level must be in [1.0, 15.0] on line %d", i + 2);

        inf.readSpace();
        string rank = inf.readToken("[A-Z+]+", "rank");
        ensuref(find(ranks.begin(), ranks.end(), rank) != ranks.end(),
                "unknown rank '%s' on line %d", rank.c_str(), i + 2);
        inf.readSpace();
        int version = inf.readInt(0, 1, "version");
        inf.readSpace();
        int id = inf.readInt(1, 1000000000, "id");
        inf.readEoln();

        auto it = chart_info.find(id);
        if (it == chart_info.end())
        {
            chart_info[id] = {level10, version};
        }
        else
        {
            ensuref(it->second.first == level10,
                    "chart %d has inconsistent level on line %d", id, i + 2);
            ensuref(it->second.second == version,
                    "chart %d has inconsistent version on line %d", id, i + 2);
        }
    }
    inf.readEof();
    return 0;
}
