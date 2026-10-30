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
    priority_queue<int, vector<int>, greater<int>> old_heap;
    priority_queue<int, vector<int>, greater<int>> new_heap;
    long long old_sum = 0;
    long long new_sum = 0;

    for (int i = 0; i < n; i++)
    {
        string level, rank;
        int version, id;
        cin >> level >> rank >> version >> id;
        int dot = (int)level.find('.');
        int level10 = stoi(level.substr(0, dot)) * 10 + (level[dot + 1] - '0');
        int idx = find(ranks.begin(), ranks.end(), rank) - ranks.begin();
        int score = level10 * values[idx] / 100;

        priority_queue<int, vector<int>, greater<int>> &heap =
            version == 0 ? old_heap : new_heap;
        long long &sum = version == 0 ? old_sum : new_sum;
        int limit = version == 0 ? 35 : 15;
        if ((int)heap.size() < limit)
        {
            heap.push(score);
            sum += score;
        }
        else if (score > heap.top())
        {
            sum -= heap.top();
            heap.pop();
            heap.push(score);
            sum += score;
        }
        cout << old_sum + new_sum << "\n";
    }
    return 0;
}
