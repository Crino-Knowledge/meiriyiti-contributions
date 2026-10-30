#include <bits/stdc++.h>
using namespace std;

struct Chart
{
    int version;
    int best;
};

int main()
{
    int n;
    cin >> n;
    map<int, Chart> charts;
    multiset<int> old_scores;
    multiset<int> new_scores;
    vector<string> ranks = {
        "SSS+", "SSS", "SS+", "SS", "S+", "S", "AAA",
        "AA", "A", "BBB", "BB", "B", "C", "D"};
    int values[14] = {
        224, 216, 211, 208, 203, 200, 168,
        152, 136, 120, 112, 96, 80, 64};

    for (int i = 0; i < n; i++)
    {
        string level, rank;
        int version, id;
        cin >> level >> rank >> version >> id;
        int dot = (int)level.find('.');
        int level10 = stoi(level.substr(0, dot)) * 10 + (level[dot + 1] - '0');
        int idx = find(ranks.begin(), ranks.end(), rank) - ranks.begin();
        int score = level10 * values[idx] / 100;
        multiset<int> &scores = version == 0 ? old_scores : new_scores;

        auto it = charts.find(id);
        if (it == charts.end())
        {
            charts[id] = {version, score};
            scores.insert(score);
        }
        else if (score > it->second.best)
        {
            scores.erase(scores.find(it->second.best));
            scores.insert(score);
            it->second.best = score;
        }

        vector<int> old_vector(old_scores.begin(), old_scores.end());
        vector<int> new_vector(new_scores.begin(), new_scores.end());
        sort(old_vector.rbegin(), old_vector.rend());
        sort(new_vector.rbegin(), new_vector.rend());
        long long sum = 0;
        for (int j = 0; j < (int)old_vector.size() && j < 35; j++)
        {
            sum += old_vector[j];
        }
        for (int j = 0; j < (int)new_vector.size() && j < 15; j++)
        {
            sum += new_vector[j];
        }
        cout << sum << "\n";
    }
    return 0;
}
