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
    int total = n * m;
    vector<int> dist(total, -1);
    vector<int> current;
    vector<int> nxt;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (grid[i][j] == 'X')
            {
                int id = i * m + j;
                dist[id] = 0;
                current.push_back(id);
            }
        }
    }
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    int p = 0;
    while (!current.empty())
    {
        nxt.clear();
        for (int id : current)
        {
            int x = id / m;
            int y = id % m;
            for (int d = 0; d < 4; d++)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (nx < 0 || nx >= n || ny < 0 || ny >= m)
                {
                    continue;
                }
                int nid = nx * m + ny;
                if (dist[nid] != -1)
                {
                    continue;
                }
                dist[nid] = p + 1;
                nxt.push_back(nid);
            }
        }
        if (!nxt.empty())
        {
            p++;
        }
        current.swap(nxt);
    }
    cout << p - 1 << endl;
    return 0;
}
