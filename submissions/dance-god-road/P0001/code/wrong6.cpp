#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;
    vector<string> ranks = {
        "SSS+", "SSS", "SS+", "SS", "S+", "S", "AAA",
        "AA", "A", "BBB", "BB", "B", "C", "D"};
    int values[14] = {
        224, 216, 211, 208, 203, 200, 168,
        152, 136, 120, 112, 96, 80, 64};
    map<int, int> best;
    multiset<int> scores;

    for (int i = 0; i < n; i++)
    {
        string level, rank;
        int version, id;
        cin >> level >> rank >> version >> id;
        int dot = (int)level.find('.');
        int level10 = stoi(level.substr(0, dot)) * 10 + (level[dot + 1] - '0');
        int idx = find(ranks.begin(), ranks.end(), rank) - ranks.begin();
        int score = level10 * values[idx] / 100;

        auto it = best.find(id);
        if (it == best.end())
        {
            best[id] = score;
            scores.insert(score);
        }
        else if (score > it->second)
        {
            scores.erase(scores.find(it->second));
            scores.insert(score);
            it->second = score;
        }

        long long sum = 0;
        int taken = 0;
        for (auto jt = scores.rbegin(); jt != scores.rend() && taken < 50; jt++, taken++)
        {
            sum += *jt;
        }
        cout << sum << "\n";
    }
    return 0;
}
