# 斯卡雷特警察巡逻贫民区24时 题解

## 1. 题意简述

给定一个 $n \times m$ 的网格，其中有 $k$ 个邪恶冰精。每一秒，所有邪恶冰精会同时把它们上下左右相邻的智慧冰精变成邪恶冰精，新感染的冰精从下一秒开始继续传播。求最晚在第几秒到达时，网格里仍然存在智慧冰精。

如果第 $p$ 秒结束时所有智慧冰精都被感染，那么答案就是 $p-1$。题目保证至少有一个智慧冰精，因此答案一定存在。

## 2. 解题思路

把每个格子看成一个点，相邻格子之间连一条边，所有边权都是 $1$。每个邪恶冰精都是传播的起点，一个智慧冰精被感染的时间，就是它到最近的邪恶冰精的距离。

这是一个多源最短路问题。把**所有**邪恶冰精同时作为起点加入队列，距离都设为 $0$，然后做一次 BFS。队列按距离从小到大扩展，每个格子第一次被访问时得到的距离，就是它到最近邪恶冰精的距离。

所有格子距离的最大值记为 $p$，表示第 $p$ 秒结束时最后一个智慧冰精被感染，因此警察最晚在第 $p-1$ 秒到达。

为什么不能只从一个邪恶冰精出发？其他邪恶冰精可能离某些格子更近，只从一个起点计算会把感染时间算大。为什么不能每个邪恶冰精各做一次 BFS？那样最坏要做 $k$ 次完整 BFS，复杂度是 $O(k \cdot n m)$，在 $k$ 很大时会超时。多源 BFS 把所有起点一起入队，只需要 $O(nm)$。

因为每个格子最多入队一次，每次检查四个方向，所以时间复杂度是 $O(nm)$，空间复杂度也是 $O(nm)$。

## 3. 常见易错点

- **只把第一个邪恶冰精当作起点**：其他邪恶冰精的传播被忽略，答案会偏大。
- **对每个邪恶冰精分别 BFS**：答案虽然正确，但复杂度是 $O(k \cdot n m)$，在 $k$ 很大时超时。
- **用 DFS 或栈代替 BFS**：第一次访问到的距离不一定是到最近邪恶冰精的距离，答案会偏大。
- **忘记减一**：第 $p$ 秒结束时全部感染，最晚到达时间是 $p-1$，不是 $p$。
- **以为答案最小是 $1$**：如果初始智慧冰精在第一秒就全部被感染，答案是 $0$。
- **读入时把空格或换行当成网格字符**：每行应读入一个长度为 $m$ 的字符串。
- 输出末尾有没有换行都算正确，所有题目均不强求在最后一行输出后换行。

## 4. 参考代码与语法精讲

### 4.1 C++ 参考代码

```cpp
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
    queue<pair<int, int>> q;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (grid[i][j] == 'X')
            {
                dist[i][j] = 0;
                q.push(make_pair(i, j));
            }
        }
    }
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    int far = 0;
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
            far = max(far, dist[nx][ny]);
            q.push(make_pair(nx, ny));
        }
    }
    cout << far - 1 << endl;
    return 0;
}
```

#### C++ 语法细节讲解

- 读入网格前先读入 $n,m,k$，`cin >>` 会自动跳过空格和换行，所以第一行的三个整数可以直接连续读。
- `vector<vector<int>> dist(n, vector<int>(m, -1))` 创建 $n \times m$ 的距离数组，`-1` 表示还没有被感染。
- 多源 BFS 的关键是初始化：扫描整个网格，把所有 `X` 的距离设为 $0$ 并加入同一个队列，而不是只加一个起点。
- `far` 在每次给新格子赋距离时更新，最后就是最大的感染时间。
- 输出 `far - 1`。题目保证至少有一个智慧冰精，所以 `far` 至少为 $1$。
- 队列的先进先出保证了距离从小到大扩展，因此每个格子第一次被访问时就是最短距离。

### 4.2 Python 参考代码

```python
from collections import deque

n, m, k = map(int, input().split())
grid = [input().strip() for _ in range(n)]

dist = [[-1] * m for _ in range(n)]
q = deque()
for i in range(n):
    for j in range(m):
        if grid[i][j] == 'X':
            dist[i][j] = 0
            q.append((i, j))

directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
while q:
    x, y = q.popleft()
    for dx, dy in directions:
        nx = x + dx
        ny = y + dy
        if 0 <= nx < n and 0 <= ny < m and dist[nx][ny] == -1:
            dist[nx][ny] = dist[x][y] + 1
            q.append((nx, ny))

print(max(max(row) for row in dist) - 1)
```

#### Python 语法细节讲解

- `n, m, k = map(int, input().split())` 把一行按空格拆开，转换成整数后依次赋给三个变量。
- `[input().strip() for _ in range(n)]` 读入 $n$ 行，并去掉每行末尾的换行符。
- `deque` 支持从队首 `popleft()`，这是 BFS 按层扩展的关键；普通列表从头部删除元素会很慢。
- 初始化时同样要把所有 `X` 都加入队列，不能只加第一个。
- `0 <= nx < n` 是 Python 的连续比较，等价于同时判断两个边界。
- `max(max(row) for row in dist)` 先求每一行的最大值，再求所有行的最大值。因为所有格子最终都会被感染，距离数组里不会留下 `-1`。
