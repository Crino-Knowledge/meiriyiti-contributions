#include <bits/stdc++.h>
using namespace std;

// 思路层错解：只会向右或向下走，认为不需要回到上面一行。
int main()
{
    int n;
    cin >> n;
    vector<string> grid(2);
    cin >> grid[0] >> grid[1];

    int x = 0;
    int y = 0;
    bool ok = false;
    while (true)
    {
        if (x == 1 && y == n - 1)
        {
            ok = true;
            break;
        }
        if (y + 1 < n && grid[x][y + 1] == '0')
        {
            y++;
        }
        else if (x == 0 && grid[1][y] == '0')
        {
            x = 1;
        }
        else
        {
            break;
        }
    }

    cout << (ok ? "YES" : "NO") << endl;
    return 0;
}
