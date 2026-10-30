#include <bits/stdc++.h>
using namespace std;

struct Chart
{
    int level10;
    int version;
    int best;
};

int coefficient_tenths(const string &rank)
{
    static const vector<string> names = {
        "SSS+", "SSS", "SS+", "SS", "S+", "S", "AAA",
        "AA", "A", "BBB", "BB", "B", "C", "D"};
    static const int values[14] = {
        224, 216, 211, 208, 203, 200, 168,
        152, 136, 120, 112, 96, 80, 64};
    for (int i = 0; i < 14; i++)
    {
        if (names[i] == rank)
        {
            return values[i];
        }
    }
    return -1;
}

long long sum_largest(const multiset<int> &scores, int limit)
{
    long long sum = 0;
    int taken = 0;
    for (auto it = scores.rbegin(); it != scores.rend() && taken < limit; it++, taken++)
    {
        sum += *it;
    }
    return sum;
}

int main()
{
    int n;
    cin >> n;
    map<int, Chart> charts;
    multiset<int> old_scores;
    multiset<int> new_scores;

    for (int i = 0; i < n; i++)
    {
        string level, rank;
        int version, id;
        cin >> level >> rank >> version >> id;

        int dot = (int)level.find('.');
        int level10 = stoi(level.substr(0, dot)) * 10 + (level[dot + 1] - '0');
        int score = level10 * coefficient_tenths(rank) / 100;
        multiset<int> &scores = version == 0 ? old_scores : new_scores;

        auto it = charts.find(id);
        if (it == charts.end())
        {
            charts[id] = {level10, version, score};
            scores.insert(score);
        }
        else if (score > it->second.best)
        {
            auto old = scores.find(it->second.best);
            scores.erase(old);
            scores.insert(score);
            it->second.best = score;
        }

        cout << sum_largest(old_scores, 35) + sum_largest(new_scores, 15) << "\n";
    }
    return 0;
}
