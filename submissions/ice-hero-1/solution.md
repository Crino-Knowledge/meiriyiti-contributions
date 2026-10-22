# 冰之勇者记1 题解

## 1. 题意简述

有一个 $2 \times n$ 的迷宫，`0` 是通路，`1` 是障碍。勇者从左上角 $(1,1)$ 出发，每次可以向上、下、左、右移动一格，问能否到达右下角的 $(2,n)$。起点和终点保证是通路。

## 2. 解题思路

把每一个格子看成一个点：如果两个格子相邻（上下或左右）且都不是障碍，就在它们之间连一条边。这样整张迷宫就变成了一张图，问题变成：从起点出发，能不能走到终点。

判断“能不能走到”最直接的方法是广度优先搜索（BFS）：

1. 把起点放进队列，并标记起点已经访问过。
2. 每次从队头取出一个格子，检查它上下左右四个相邻格子。
3. 如果相邻格子没有越界、不是障碍、也没有访问过，就把它标记为已访问并放进队列。
4. 队列为空时搜索结束。此时如果终点被访问过，答案就是 `YES`，否则是 `NO`。

每个格子最多入队一次，每个格子最多检查四条边，所以时间复杂度是 $O(n)$，空间复杂度也是 $O(n)$。

本题只有两行，看起来很简单，但路线可能需要在两行之间来回切换很多次，所以不能只检查“换一次行”的情况，仍然需要完整搜索。

## 3. 常见易错点

- 只允许向右或向下走，忘记勇者还可以向上、向左走。
- 认为最多只需要换一次行，漏掉需要多次换行才能到达的情况。
- 入队时忘记标记 `visited`，同一个格子被反复加入队列，导致超时。
- 把 `0` 和 `1` 的含义写反，把通路当成障碍。
- 行列下标从 $0$ 开始还是从 $1$ 开始没有统一，导致越界或判断错格子。
- 只判断终点是不是 `0`，却忘记判断起点是否能走出去。
- 字符串长度是 $n$，读入时不要多读或少读字符。
- 所有题目均不强求在最后一行输出后换行。

## 4. 参考代码与语法精讲

### 4.1 C++ 参考代码

```cpp
#include<bits/stdc++.h>
using namespace std;

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
            if (grid[nx][ny] == '1' || visited[nx][ny])
            {
                continue;
            }
            visited[nx][ny] = true;
            q.push(make_pair(nx, ny));
        }
    }

    if (visited[1][n - 1])
    {
        cout << "YES" << endl;
    }
    else
    {
        cout << "NO" << endl;
    }
    return 0;
}
```

#### C++ 语法细节讲解

- `vector<string> grid(2);` 创建了一个长度为 $2$ 的字符串数组，`grid[0]` 是第一行，`grid[1]` 是第二行。`grid[0][0]` 就是左上角格子。
- `vector<vector<bool>> visited(2, vector<bool>(n, false));` 创建 $2$ 行 $n$ 列的访问标记，初始全部为 `false`。注意两层 `>` 之间要留空格，避免旧编译器把它当成右移运算符。
- `queue<pair<int, int>> q;` 是队列。`pair<int, int>` 可以把行号和列号打包成一个整体，`make_pair(x, y)` 用来创建它，`now.first` 和 `now.second` 分别取出两个值。
- `q.front()` 取队头元素，`q.pop()` 删除队头元素，`q.push(...)` 把元素放到队尾，`q.empty()` 判断队列是否为空。
- 方向数组 `dx` 和 `dy` 把“上下左右”四种移动写成循环，避免重复写四段几乎相同的代码。
- 越界检查和障碍检查都通过后，才把新格子标记为已访问并入队。**标记必须发生在入队时**，否则同一个格子可能被多次入队。
- 最后检查 `visited[1][n - 1]`，也就是公主所在的格子是否被访问过。

### 4.2 Python 参考代码

```python
n = int(input())
grid = [input(), input()]

visited = [[False] * n for _ in range(2)]
visited[0][0] = True
queue = [(0, 0)]
head = 0

dx = [-1, 1, 0, 0]
dy = [0, 0, -1, 1]

while head < len(queue):
    x, y = queue[head]
    head += 1
    for d in range(4):
        nx = x + dx[d]
        ny = y + dy[d]
        if nx < 0 or nx >= 2 or ny < 0 or ny >= n:
            continue
        if grid[nx][ny] == '1' or visited[nx][ny]:
            continue
        visited[nx][ny] = True
        queue.append((nx, ny))

if visited[1][n - 1]:
    print("YES")
else:
    print("NO")
```

#### Python 语法细节讲解

- `int(input())` 把第一行读成整数，`input()` 读入一行字符串并自动去掉行末换行。
- `grid = [input(), input()]` 连续读入两行，得到一个包含两个字符串的列表。
- `[[False] * n for _ in range(2)]` 创建 $2$ 行 $n$ 列的二维列表。这里必须用列表推导式，如果写成 `[[False] * n] * 2`，两行会指向同一个列表，修改一行会影响另一行。
- `queue = [(0, 0)]` 用列表模拟队列，`head` 指向当前队头。`queue.append(...)` 入队，`head += 1` 相当于出队。这样不需要额外导入其他模块。
- `x, y = queue[head]` 是元组解包，把 `(行号, 列号)` 分别赋给 `x` 和 `y`。
- Python 的字符串可以用 `grid[nx][ny]` 直接取字符，不需要把它转成数字。
- 最后用 `print("YES")` 或 `print("NO")` 输出答案。
