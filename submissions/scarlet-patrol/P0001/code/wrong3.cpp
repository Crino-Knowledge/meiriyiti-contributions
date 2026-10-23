#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n, m, k;
    cin >> n >> m >> k;
    vector<string> grid(n);
    for (int i = 0; i < n; i++)
    {
        cin >> grid[i];
    }
    vector<pair<int, int>> sources;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (grid[i][j] == 'X')
            {
                sources.push_back(make_pair(i, j));
            }
        }
    }
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    vector<vector<int>> best(n, vector<int>(m, n * m + 1));
    for (int s = 0; s < (int)sources.size(); s++)
    {
        int sx = sources[s].first;
        int sy = sources[s].second;
        vector<vector<int>> dist(n, vector<int>(m, -1));
        queue<pair<int, int>> q;
        dist[sx][sy] = 0;
        q.push(make_pair(sx, sy));
        while (!q.empty())
        {
            pair<int, int> cur = q.front();
            q.pop();
            int x = cur.first;
            int y = cur.second;
            for (int d = 0; d < 4; d++)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (nx < 0 || nx >= n || ny < 0 || ny >= m)
                {
                    continue;
                }
                if (dist[nx][ny] != -1)
                {
                    continue;
                }
                dist[nx][ny] = dist[x][y] + 1;
                q.push(make_pair(nx, ny));
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                best[i][j] = min(best[i][j], dist[i][j]);
            }
        }
    }
    int far = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            far = max(far, best[i][j]);
        }
    }
    cout << far - 1 << endl;
    return 0;
}
