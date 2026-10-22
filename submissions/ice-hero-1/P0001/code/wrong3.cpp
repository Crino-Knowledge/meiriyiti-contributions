#include <bits/stdc++.h>
using namespace std;

// 复杂度层错解：BFS 逻辑正确，但用 vector 线性查找代替 visited 数组，复杂度退化为 O(n^2)。
int main()
{
    int n;
    cin >> n;
    vector<string> grid(2);
    cin >> grid[0] >> grid[1];

    vector<pair<int, int>> visited;
    queue<pair<int, int>> q;
    visited.push_back(make_pair(0, 0));
    q.push(make_pair(0, 0));

    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    while (!q.empty())
    {
        pair<int, int> now = q.front();
        q.pop();
        int x = now.first;
        int y = now.second;
        for (int d = 0; d < 4; d++)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx < 0 || nx >= 2 || ny < 0 || ny >= n)
            {
                continue;
            }
            if (grid[nx][ny] == '1')
            {
                continue;
            }
            bool seen = false;
            for (int i = 0; i < (int)visited.size(); i++)
            {
                if (visited[i].first == nx && visited[i].second == ny)
                {
                    seen = true;
                    break;
                }
            }
            if (!seen)
            {
                visited.push_back(make_pair(nx, ny));
                q.push(make_pair(nx, ny));
            }
        }
    }

    bool ok = false;
    for (int i = 0; i < (int)visited.size(); i++)
    {
        if (visited[i].first == 1 && visited[i].second == n - 1)
        {
            ok = true;
        }
    }
    cout << (ok ? "YES" : "NO") << endl;
    return 0;
}
