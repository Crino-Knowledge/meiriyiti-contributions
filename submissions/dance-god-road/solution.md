# 舞神之路 题解

## 1. 题意简述

小 Satori 的 Rating 初始为 $0$。接下来依次加入 $n$ 条成绩，每条成绩给出谱面定数 $\ell$、评级 $r$、版本 $v$ 和谱面编号 $id$。

一条成绩的单曲积分是

$$
\left\lfloor \ell \times c(r)\right\rfloor,
$$

其中 $c(r)$ 是评级对应的系数。

同一个谱面可以被重复游玩，但只能贡献一条成绩，并且只保留这个谱面历史上最高的单曲积分。最后，从旧版本的全部谱面最高分中取前 $35$ 条，从当前版本的全部谱面最高分中取前 $15$ 条，两部分之和就是当前 B50 总分。

要求每加入一条成绩，就输出一次当前总分。某一部分不足 $35$ 条或 $15$ 条时，有几条就取几条。

所有题目均不强求在最后一行输出后换行。

## 2. 解题思路

### 2.1 把单曲积分算准

谱面定数和评级系数最多都只有一位小数。为了避免浮点误差，可以把两者都扩大 $10$ 倍，用整数计算。

例如 $\ell=13.1$ 可以存成 $131$，评级 `S+` 的系数 $20.3$ 可以存成 $203$。真正的乘积是

$$
\frac{131\times 203}{100}=265.93.
$$

C++ 的整数除法会自动向下取整，所以 `131 * 203 / 100` 得到 $265$。Python 中应使用 `//`，结果同样正确。

### 2.2 为每个谱面记录最高分

用一张表 `best` 记录每个谱面目前最好的成绩：

```text
best[id] = (version, best_score)
```

读入一条新成绩后，先算出它的单曲积分 `score`：

- 如果 `id` 第一次出现，就把 `score` 加入对应版本的榜单；
- 如果 `score` 不大于 `best[id]` 中的旧最高分，这条成绩不会改变任何榜单；
- 如果 `score` 更大，就要删除这个谱面的旧最高分，再把新最高分加入榜单。

注意第二种情况：不能只把新成绩加入榜单，因为同一个谱面只能贡献一条成绩，旧最高分必须同时删除。

### 2.3 维护两个前 K 榜单

旧版本只需要最高的 $35$ 个谱面最高分，当前版本只需要最高的 $15$ 个。可以用两个降序数组保存：

- `old_top`：旧版本当前最高的至多 $35$ 个分数；
- `new_top`：当前版本当前最高的至多 $15$ 个分数。

加入一个分数 `score` 时：

1. 如果数组未满，直接把 `score` 插入正确位置；
2. 如果数组已满且 `score` 不大于最后一个元素，它进不了前 $K$ 名，直接忽略；
3. 否则删除最后一个元素，再插入 `score`。

当某个谱面刷新最高分时，先尝试从对应数组中删除它的旧分数。如果旧分数本来就不在前 $K$ 名，它不会出现在数组里，因此不需要删除。

### 2.4 为什么删除旧分数后数组仍然正确

数组始终保存当前所有谱面最高分中最大的至多 $K$ 个，并按降序排列。

刷新一个谱面时，这个谱面在“所有谱面最高分”这张表里的值从旧分数变成更大的新分数。因此先从榜单中删除旧值，再加入新值，就等价于重新计算一次前 $K$ 大。

最终，两个数组分别是旧版本前 $35$ 和当前版本前 $15$。把两个数组中的所有数相加，就是当前 B50。

### 2.5 复杂度

每个数组最多有 $35$ 或 $15$ 个元素。每次插入或删除最多移动 $50$ 个元素；用 `map` 和 Python 字典查找谱面最高分需要 $O(\log n)$ 或期望 $O(1)$。

- 时间复杂度：$O(50n+n\log n)$，可以看成 $O(n\log n)$；
- 空间复杂度：$O(n)$，用于记录每个出现过的谱面的最高分。

如果每次刷新成绩后都把全部历史重新排序，最坏会达到 $O(n^2\log n)$，在 $n=2\times 10^5$ 时会超时。

## 3. 常见易错点

1. **忽略谱面 id。** 同一谱面的每次游玩不能分别计入 B50，只会保留最高分。
2. **刷新高分时忘记删除旧分。** 同一谱面会同时出现两次，榜单变大且总分偏高。
3. **低分覆盖了高分。** 新成绩不超过当前最高分时，`best[id]` 不应改变。
4. **把 B35 和 B15 都从全部谱面中取。** 两个榜单必须先按版本分类，再分别取前 $35$ 和前 $15$。
5. **只保留前 $35$ 个或前 $15$ 个输入的谱面。** 后面的谱面也可能进入榜单，必须允许替换最小值。
6. **把所有谱面最高分直接相加。** 超过 $35$ 条或 $15$ 条后，低分谱面不应继续计入。
7. **把向下取整写成四舍五入。** 例如 $13.1\times 20.3=265.93$，答案是 $265$，不是 $266$。
8. **忽略定数的小数部分。** 例如 $13.9$ 必须按 $139$ 参与计算，不能先截断成 $13$。
9. 输出末尾有没有换行都算正确，不需要为最后一行是否有换行做额外处理。

## 4. 参考代码与语法精讲

### 4.1 C++ 参考代码

```cpp
#include<bits/stdc++.h>
using namespace std;

struct Chart
{
    int version;
    int best_score;
};

int coefficient(string rank)
{
    if (rank == "SSS+")
    {
        return 224;
    }
    if (rank == "SSS")
    {
        return 216;
    }
    if (rank == "SS+")
    {
        return 211;
    }
    if (rank == "SS")
    {
        return 208;
    }
    if (rank == "S+")
    {
        return 203;
    }
    if (rank == "S")
    {
        return 200;
    }
    if (rank == "AAA")
    {
        return 168;
    }
    if (rank == "AA")
    {
        return 152;
    }
    if (rank == "A")
    {
        return 136;
    }
    if (rank == "BBB")
    {
        return 120;
    }
    if (rank == "BB")
    {
        return 112;
    }
    if (rank == "B")
    {
        return 96;
    }
    if (rank == "C")
    {
        return 80;
    }
    return 64;
}

void add_score(vector<int> &top, long long &sum, int limit, int score)
{
    if ((int)top.size() == limit && score <= top.back())
    {
        return;
    }
    if ((int)top.size() == limit)
    {
        sum -= top.back();
        top.pop_back();
    }

    int pos = 0;
    while (pos < (int)top.size() && top[pos] >= score)
    {
        pos++;
    }
    top.insert(top.begin() + pos, score);
    sum += score;
}

void remove_score(vector<int> &top, long long &sum, int score)
{
    for (int i = 0; i < (int)top.size(); i++)
    {
        if (top[i] == score)
        {
            sum -= score;
            top.erase(top.begin() + i);
            return;
        }
    }
}

int main()
{
    int n;
    cin >> n;
    map<int, Chart> best;
    vector<int> old_top;
    vector<int> new_top;
    long long old_sum = 0;
    long long new_sum = 0;

    for (int i = 0; i < n; i++)
    {
        string level;
        string rank;
        int version;
        int id;
        cin >> level >> rank >> version >> id;

        int dot = level.find('.');
        int level10 = 0;
        for (int j = 0; j < dot; j++)
        {
            level10 = level10 * 10 + (level[j] - '0');
        }
        level10 = level10 * 10 + (level[dot + 1] - '0');
        int score = level10 * coefficient(rank) / 100;

        auto it = best.find(id);
        if (it == best.end())
        {
            best[id] = {version, score};
            if (version == 0)
            {
                add_score(old_top, old_sum, 35, score);
            }
            else
            {
                add_score(new_top, new_sum, 15, score);
            }
        }
        else if (score > it->second.best_score)
        {
            int old_score = it->second.best_score;
            if (version == 0)
            {
                remove_score(old_top, old_sum, old_score);
                add_score(old_top, old_sum, 35, score);
            }
            else
            {
                remove_score(new_top, new_sum, old_score);
                add_score(new_top, new_sum, 15, score);
            }
            it->second.best_score = score;
        }

        cout << old_sum + new_sum << endl;
    }
    return 0;
}
```

#### C++ 语法细节讲解

- `map<int, Chart> best` 把谱面编号映射到该谱面的版本和最高分。`find(id)` 找到时返回对应位置，找不到时返回 `best.end()`。
- `best[id] = {version, score}` 新建一条谱面记录；`it->second.best_score` 读取已经找到的谱面最高分。
- `vector<int> &top` 使用引用，函数中删除或插入元素会直接修改原数组，不会复制一份。
- `top.back()` 是榜单最后一个元素，也就是当前保留的最小分数。
- `top.pop_back()` 删除末尾元素，`top.insert(top.begin() + pos, score)` 在指定位置插入元素。
- `remove_score` 只删除一个旧分数；如果它不在榜单里，说明它本来就不是前 $K$ 名，不需要修改。
- `long long old_sum` 和 `long long new_sum` 保存两个榜单的总和，所以每轮直接输出二者之和即可。
- 本题不需要快读快写，直接使用 `cin`、`cout` 和 `endl` 即可。

### 4.2 Python 参考代码

```python
def coefficient(rank):
    if rank == "SSS+":
        return 224
    if rank == "SSS":
        return 216
    if rank == "SS+":
        return 211
    if rank == "SS":
        return 208
    if rank == "S+":
        return 203
    if rank == "S":
        return 200
    if rank == "AAA":
        return 168
    if rank == "AA":
        return 152
    if rank == "A":
        return 136
    if rank == "BBB":
        return 120
    if rank == "BB":
        return 112
    if rank == "B":
        return 96
    if rank == "C":
        return 80
    return 64


def add_score(top, score, limit):
    if len(top) == limit and score <= top[-1]:
        return
    if len(top) == limit:
        top.pop()

    pos = 0
    while pos < len(top) and top[pos] >= score:
        pos += 1
    top.insert(pos, score)


def remove_score(top, score):
    if score in top:
        top.remove(score)


def main():
    n = int(input())
    best = {}
    old_top = []
    new_top = []

    for _ in range(n):
        level, rank, version_text, id_text = input().split()
        version = int(version_text)
        chart_id = int(id_text)

        whole, decimal = level.split(".")
        level10 = int(whole) * 10 + int(decimal)
        score = level10 * coefficient(rank) // 100

        if chart_id not in best:
            best[chart_id] = (version, score)
            if version == 0:
                add_score(old_top, score, 35)
            else:
                add_score(new_top, score, 15)
        elif score > best[chart_id][1]:
            old_score = best[chart_id][1]
            if version == 0:
                remove_score(old_top, old_score)
                add_score(old_top, score, 35)
            else:
                remove_score(new_top, old_score)
                add_score(new_top, score, 15)
            best[chart_id] = (version, score)

        print(sum(old_top) + sum(new_top))


main()
```

#### Python 语法细节讲解

- `best = {}` 创建一个空字典，键是谱面编号，值是这个谱面的 `(版本, 最高分)`。
- `chart_id not in best` 判断谱面是否第一次出现；`best[chart_id][1]` 取出元组中的最高分。
- `level.split(".")` 把类似 `"13.1"` 的文本拆成两部分，再组合成整数 `131`。
- `//` 是整除，`26593 // 100` 的结果是向下取整的 $265$。
- `top[-1]` 是列表最后一个元素；`top.pop()` 删除它；`top.insert(pos, score)` 在指定位置插入元素。
- `score in top` 只检查旧分数是否仍在当前前 $K$ 榜单中。不在时说明它本来就没有被计入。
- `top.remove(score)` 删除第一个等于该分数的元素。分数相同时，删除哪一个元素不影响榜单的总和。
- `sum(old_top) + sum(new_top)` 求两个榜单的总分。两个榜单合起来最多只有 $50$ 个元素，所以每次重新求和也足够快。
