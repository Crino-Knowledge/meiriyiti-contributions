#include <bits/stdc++.h>
using namespace std;

// 与 std.cpp 的四方向 BFS 不同：按列从左到右递推，只记录当前列两行是否可达。
int main()
{
    int n;
    cin >> n;
    vector<string> grid(2);
    cin >> grid[0] >> grid[1];

    vector<bool> top(n, false);
    vector<bool> bottom(n, false);
    top[0] = true;
    if (grid[1][0] == '0')
    {
        bottom[0] = true;
    }

    for (int i = 1; i < n; i++)
    {
        bool t = top[i - 1] && grid[0][i] == '0';
        bool b = bottom[i - 1] && grid[1][i] == '0';
        if (grid[0][i] == '0' && grid[1][i] == '0')
        {
            if (t)
            {
                b = true;
            }
            if (b)
            {
                t = true;
            }
        }
        top[i] = t;
        bottom[i] = b;
    }

    cout << (bottom[n - 1] ? "YES" : "NO") << endl;
    return 0;
}
