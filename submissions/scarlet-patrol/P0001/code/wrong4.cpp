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
    vector<vector<int>> dist(n, vector<int>(m, -1));
    stack<pair<int, int>> st;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (grid[i][j] == 'X')
            {
                dist[i][j] = 0;
                st.push(make_pair(i, j));
            }
        }
    }
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    int far = 0;
    while (!st.empty())
    {
        pair<int, int> cur = st.top();
        st.pop();
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
            far = max(far, dist[nx][ny]);
            st.push(make_pair(nx, ny));
        }
    }
    cout << far - 1 << endl;
    return 0;
}
