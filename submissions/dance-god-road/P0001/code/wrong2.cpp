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
    int old_count = 0;
    int new_count = 0;
    long long sum = 0;

    for (int i = 0; i < n; i++)
    {
        string level, rank;
        int version, id;
        cin >> level >> rank >> version >> id;
        int dot = (int)level.find('.');
        int level10 = stoi(level.substr(0, dot)) * 10 + (level[dot + 1] - '0');
        int idx = find(ranks.begin(), ranks.end(), rank) - ranks.begin();
        int score = level10 * values[idx] / 100;

        if (version == 0 && old_count < 35)
        {
            old_count++;
            sum += score;
        }
        if (version == 1 && new_count < 15)
        {
            new_count++;
            sum += score;
        }
        cout << sum << "\n";
    }
    return 0;
}
