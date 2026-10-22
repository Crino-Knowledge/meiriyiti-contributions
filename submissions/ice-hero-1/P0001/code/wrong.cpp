#include <bits/stdc++.h>
using namespace std;

// 思路层错解：认为整条路线最多只需要换一次行。
int main()
{
    int n;
    cin >> n;
    vector<string> grid(2);
    cin >> grid[0] >> grid[1];

    vector<int> topBad(n + 1, 0);
    vector<int> bottomBad(n + 1, 0);
    for (int i = 0; i < n; i++)
    {
        topBad[i + 1] = topBad[i] + (grid[0][i] == '1');
        bottomBad[i + 1] = bottomBad[i] + (grid[1][i] == '1');
    }

    bool ok = false;
    for (int i = 0; i < n; i++)
    {
        if (grid[0][i] != '0' || grid[1][i] != '0')
        {
            continue;
        }
        if (topBad[i + 1] == 0 && bottomBad[n] - bottomBad[i] == 0)
        {
            ok = true;
        }
        if (bottomBad[i + 1] == 0 && topBad[n] - topBad[i] == 0)
        {
            ok = true;
        }
    }

    cout << (ok ? "YES" : "NO") << endl;
    return 0;
}
