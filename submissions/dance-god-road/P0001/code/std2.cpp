#include <bits/stdc++.h>
using namespace std;

struct Chart
{
    int version;
    int best;
};

int coefficient_tenths(const string &rank)
{
    if (rank == "SSS+")
    {
        return 224;
    }
    if (rank == "SSS")
    {
        return 216;
    }
    if (rank == "SS+")
    {
        return 211;
    }
    if (rank == "SS")
    {
        return 208;
    }
    if (rank == "S+")
    {
        return 203;
    }
    if (rank == "S")
    {
        return 200;
    }
    if (rank == "AAA")
    {
        return 168;
    }
    if (rank == "AA")
    {
        return 152;
    }
    if (rank == "A")
    {
        return 136;
    }
    if (rank == "BBB")
    {
        return 120;
    }
    if (rank == "BB")
    {
        return 112;
    }
    if (rank == "B")
    {
        return 96;
    }
    if (rank == "C")
    {
        return 80;
    }
    return 64;
}

long long sum_largest(const int frequency[337], int limit)
{
    long long sum = 0;
    int taken = 0;
    for (int score = 336; score >= 0 && taken < limit; score--)
    {
        int use = min(frequency[score], limit - taken);
        sum += 1LL * use * score;
        taken += use;
    }
    return sum;
}

int main()
{
    int n;
    cin >> n;
    map<int, Chart> charts;
    int old_frequency[337] = {};
    int new_frequency[337] = {};

    for (int i = 0; i < n; i++)
    {
        int whole, decimal, version, id;
        string rank;
        char dot;
        cin >> whole >> dot >> decimal >> rank >> version >> id;

        int level10 = whole * 10 + decimal;
        int score = level10 * coefficient_tenths(rank) / 100;
        int *frequency = version == 0 ? old_frequency : new_frequency;

        auto it = charts.find(id);
        if (it == charts.end())
        {
            charts[id] = {version, score};
            frequency[score]++;
        }
        else if (score > it->second.best)
        {
            frequency[it->second.best]--;
            frequency[score]++;
            it->second.best = score;
        }

        cout << sum_largest(old_frequency, 35) + sum_largest(new_frequency, 15) << "\n";
    }
    return 0;
}
