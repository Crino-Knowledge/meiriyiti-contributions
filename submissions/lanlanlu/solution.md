# 蓝蓝路 题解

## 1. 题意简述

有 $t$ 组询问，每组给出一个正整数 $h$。小羊可以重复使用“吃吃吃”，第 $i$ 次造成 $2^{i-1}$ 点伤害；还可以至多使用一次“大吃一口”，选择一个不超过当前血量的质数 $x$，造成 $x$ 点伤害。所有攻击的总伤害必须恰好等于 $h$，求最少攻击次数；无解输出 `-1`。

## 2. 解题思路

### 2.1 物理攻击的总伤害

如果小羊一共使用了 $m$ 次“吃吃吃”，那么伤害依次是

$$
1,2,4,\ldots,2^{m-1},
$$

总和为

$$
1+2+4+\cdots+2^{m-1}=2^m-1.
$$

这个总和只和使用次数 $m$ 有关，和符卡插在中间还是最后没有关系。

### 2.2 符卡应该什么时候用

假设小羊使用了 $m$ 次物理攻击，并且使用了一次符卡。总伤害必须等于 $h$，所以符卡造成的伤害只能是

$$
x=h-(2^m-1).
$$

只要 $x$ 是质数，就可以在战斗一开始先使用符卡，选择这个质数 $x$，然后再使用 $m$ 次物理攻击。因为 $x \le h$，这个方案一定合法。

如果 $x=0$，说明不需要符卡，只用 $m$ 次物理攻击就能恰好打完。

如果 $x<0$，说明物理攻击已经打多了，后面的 $m$ 都不能再考虑。

### 2.3 枚举与答案

对于每组询问，从 $m=0$ 开始枚举物理攻击次数，维护 $2^m-1$：

- 若 $h-(2^m-1)=0$，用 $m$ 次攻击可以击败蓝蓝路；
- 若 $h-(2^m-1)$ 是大于等于 $2$ 的质数，用 $m+1$ 次攻击可以击败蓝蓝路；
- 当 $2^m-1>h$ 时停止枚举。

在所有合法方案中取最小值；一个都没有就输出 `-1`。

因为 $h \le 10^7$，$2^m-1$ 最多枚举到 $2^{24}-1$，所以每组询问只需要检查约 $24$ 个候选值。

### 2.4 质数预处理

如果每次都用试除法判断 $h-(2^m-1)$ 是否为质数，单次判断需要 $O(\sqrt h)$，在 $t=2\times 10^5$ 的极限数据下会超时。

可以先把所有询问读进来，求出最大的 $h$，然后用埃氏筛预处理出 $1 \sim \max h$ 中的所有质数。之后每次判断质数只需要 $O(1)$ 查表。

### 2.5 复杂度

设 $H=\max h$。

- 埃氏筛：$O(H\log\log H)$；
- 每组询问最多枚举 $O(\log H)$ 次；
- 总时间复杂度 $O(H\log\log H+t\log H)$；
- 空间复杂度 $O(H)$，$H=10^7$ 时约 10 MB 量级。

## 3. 常见易错点

- 把 $m$ 次物理攻击的总伤害写成 $2^m$，忘记减一。
- 只考虑“全部用物理攻击”或“全部用符卡”，漏掉先物理再符卡、或者先符卡再物理的混合方案。
- 误以为符卡可以使用多次，从而把本来无解的答案算小。
- 把 $1$ 当成质数。判断质数时要从 $2$ 开始，并且 $0,1$ 都不是质数。
- 枚举物理次数时没有在 $2^m-1>h$ 时停止，导致多算或死循环。
- 筛法忘记把 `0` 和 `1` 标记为“不是质数”，导致小数据判断错误。
- 所有题目均不强求在最后一行输出后换行。

## 4. 参考代码与语法精讲

### 4.1 C++ 参考代码

```cpp
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int t;
    cin >> t;
    vector<int> h(t);
    int maxH = 0;
    for (int i = 0; i < t; i++)
    {
        cin >> h[i];
        maxH = max(maxH, h[i]);
    }

    vector<bool> isPrime(maxH + 1, true);
    isPrime[0] = false;
    isPrime[1] = false;
    for (int i = 2; 1LL * i * i <= maxH; i++)
    {
        if (!isPrime[i])
        {
            continue;
        }
        for (long long j = 1LL * i * i; j <= maxH; j += i)
        {
            isPrime[(int)j] = false;
        }
    }

    for (int i = 0; i < t; i++)
    {
        int best = -1;
        long long physical = 0;
        int m = 0;
        while (physical <= h[i])
        {
            long long rest = h[i] - physical;
            if (rest == 0)
            {
                if (best == -1 || m < best)
                {
                    best = m;
                }
            }
            else if (rest >= 2 && isPrime[(int)rest])
            {
                if (best == -1 || m + 1 < best)
                {
                    best = m + 1;
                }
            }
            physical = physical * 2 + 1;
            m++;
        }
        cout << best << endl;
    }
    return 0;
}
```

#### C++ 语法细节讲解

- `vector<int> h(t);` 创建一个长度为 $t$ 的数组，用来保存所有询问，方便后面求最大值并统一筛质数。
- `vector<bool> isPrime(maxH + 1, true);` 创建一个下标 $0 \sim maxH$ 的布尔数组，初始都当作质数。`vector<bool>` 是按位存储的，所以 $10^7$ 个元素只占约 1.25 MB。
- `isPrime[0] = false; isPrime[1] = false;` 是必须的边界处理，$0$ 和 $1$ 都不是质数。
- 外层循环只枚举到 $\sqrt{maxH}$，因为一个合数一定有一个不超过 $\sqrt{maxH}$ 的质因子。
- 内层循环从 `i * i` 开始，把 $i$ 的倍数标记成合数；比 $i^2$ 小的倍数已经被更小的质数筛过了。
- `1LL * i * i` 先把 `i` 转成 `long long` 再相乘，避免 $i^2$ 在 `int` 中溢出。
- `long long physical = 0;` 表示当前物理攻击总伤害 $2^m-1$。虽然本题不会超过 `int`，但用 `long long` 更稳妥。
- `physical = physical * 2 + 1;` 是从 $2^m-1$ 递推到 $2^{m+1}-1$，不需要每次都计算 $2^m$。
- `best == -1` 表示还没有找到任何合法方案；后面每次找到候选答案时都和 `best` 取较小值。

### 4.2 Python 参考代码

```python
import math

t = int(input())
hs = []
limit = 0
for _ in range(t):
    h = int(input())
    hs.append(h)
    if h > limit:
        limit = h

is_prime = bytearray(b'\x01') * (limit + 1)
is_prime[0] = 0
is_prime[1] = 0

i = 2
while i * i <= limit:
    if is_prime[i]:
        start = i * i
        count = (limit - start) // i + 1
        is_prime[start:limit + 1:i] = b'\x00' * count
    i += 1

for h in hs:
    best = -1
    physical = 0
    m = 0
    while physical <= h:
        rest = h - physical
        if rest == 0:
            if best == -1 or m < best:
                best = m
        elif rest >= 2 and is_prime[rest]:
            if best == -1 or m + 1 < best:
                best = m + 1
        physical = physical * 2 + 1
        m += 1
    print(best)
```

#### Python 语法细节讲解

- `hs = []` 用一个列表保存所有询问，后面需要先知道最大值 `limit`，才能确定筛法范围。
- `bytearray(b'\x01') * (limit + 1)` 创建一个长度为 `limit + 1` 的字节数组，每个字节初值为 `1`，表示“暂时认为是质数”。相比普通列表，`bytearray` 更省内存。
- `is_prime[0] = 0` 和 `is_prime[1] = 0` 把 $0,1$ 标记为非质数。
- `is_prime[start:limit + 1:i]` 是带步长的切片，表示从 `start` 开始，每隔 `i` 个取一个位置。给这个切片赋值相同长度的 `b'\x00' * count`，就一次性把所有 $i$ 的倍数改成 `0`。
- `count = (limit - start) // i + 1` 计算切片中一共有多少个元素，必须和切片长度完全一致，否则 Python 会报错。
- `while i * i <= limit` 只枚举到 $\sqrt{limit}$，和 C++ 版本思路一致。
- `rest >= 2 and is_prime[rest]` 先判断 `rest` 是否至少为 $2$，再查表；这样不会把 $0,1$ 误判成质数。
- `print(best)` 会自动换行，输出格式符合要求。
