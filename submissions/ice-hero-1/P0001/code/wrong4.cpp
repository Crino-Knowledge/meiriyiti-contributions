#include <bits/stdc++.h>
using namespace std;

// 实现层错解：把地图中的 0 和 1 含义写反，把通路当成障碍。
int main()
{
    int n;
    cin >> n;
    vector<string> grid(2);
    cin >> grid[0] >> grid[1];

    vector<vector<bool>> visited(2, vector<bool>(n, false));
    queue<pair<int, int>> q;
    visited[0][0] = true;
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
            if (grid[nx][ny] == '0' || visited[nx][ny])
            {
                continue;
            }
            visited[nx][ny] = true;
            q.push(make_pair(nx, ny));
        }
    }

    cout << (visited[1][n - 1] ? "YES" : "NO") << endl;
    return 0;
}
