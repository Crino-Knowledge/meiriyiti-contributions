# 有哪些优秀的百合作品 题解

## 1. 题意简述

小D 收到了 $n$ 部百合作品，每部作品给出 $q$ 个互不相同的角色性格类型。对同一部作品中的任意两个性格类型 $a,b$，定义命运羁绊指数

$$
S(a,b)=\frac{\operatorname{lcm}(a,b)}{\gcd(a,b)}.
$$

其中 $\operatorname{lcm}$ 被题面称作两人的命运相近程度，$\gcd$ 被称作命运相似程度，它们的比值 $S$ 就是命运羁绊指数。

要求对每部作品找出一对角色，使 $S(a,b)$ 最大，输出这两个性格类型和 $S(a,b)$。最大值被多对角色取到时，输出任意一对即可。

所有题目均不强求在最后一行输出后换行。

## 2. 解题思路

### 2.1 先把公式化简

直接按定义算 $\operatorname{lcm}$ 需要先乘再除，很容易超出整数范围。我们先把公式化简。

设 $g=\gcd(a,b)$，把两个数都除以 $g$：

$$
a=g\cdot x,\qquad b=g\cdot y,\qquad \gcd(x,y)=1 .
$$

此时最小公倍数是 $\operatorname{lcm}(a,b)=g\cdot x\cdot y$（因为 $x,y$ 互质，互质的两个数的最小公倍数就是它们的乘积）。代回定义：

$$
S(a,b)=\frac{g\cdot x\cdot y}{g}=x\cdot y=\frac{a}{g}\cdot\frac{b}{g}.
$$

也就是说：**只要先求出两个数的最大公约数，再把两个数各除以它、相乘，就是羁绊指数**，完全不需要单独算最小公倍数。

这个化简还有两个好处：

- $a/g$ 和 $b/g$ 都至少是 $1$，它们的乘积不超过 $(2\times 10^9)^2=4\times 10^{18}$，用 64 位整数一定存得下；
- 不需要先算出可能达到 $4\times 10^{18}$ 的最小公倍数再去除以 $\gcd$ 的平方，中间量更小，思路也更清楚。

### 2.2 最大公约数怎么求

用欧几里得算法（辗转相除）：

```text
gcd(a, b):
    当 b 不等于 0 时反复执行:
        t = a % b
        a = b
        b = t
    最后 a 就是答案
```

例如 $\gcd(18,25)$：$18\bmod 25=18$，$25\bmod 18=7$，$18\bmod 7=4$，$7\bmod 4=3$，$4\bmod 3=1$，$3\bmod 1=0$，最后得到 $1$。

每做一次取模，较小的那个数至少减半，所以单次求 $\gcd$ 只要 $O(\log \max(a,b))$ 次取模，非常快。

### 2.3 枚举所有配对

每部作品最多 $q=1000$ 个角色，配对总数是

$$
\binom{q}{2}=\frac{q(q-1)}{2}\le \frac{1000\times 999}{2}\approx 5\times 10^{5}.
$$

一部作品最多约 $5\times 10^5$ 对，$n\le 10$ 部作品一共约 $5\times 10^6$ 对。每对做一次 $O(\log (2\times 10^9))$ 的 $\gcd$，总运算量在 $10^8$ 次以内，$2$ 秒足够。

所以本题不需要任何巧妙的数据结构，**直接双重循环枚举所有配对，边算边更新最大值**即可。

具体做法（对每部作品）：

1. 读入 $q$ 个性格类型，存进数组；
2. 用 `bestValue` 记录目前的最大羁绊指数，初始为 $-1$；用 `bestA`、`bestB` 记录对应的两个性格类型；
3. 用两层循环枚举 $i<j$，算出 $g=\gcd(a_i,a_j)$ 与 $v=(a_i/g)\cdot(a_j/g)$；
4. 如果 $v>$ `bestValue`，就更新三个变量；
5. 循环结束后输出 `bestA`、`bestB`、`bestValue`。

因为题目允许多解，遇到和当前最大值相等的组合直接跳过即可；为了让输出看起来整齐，可以把较小的性格类型先输出。

### 2.4 复杂度

- 时间复杂度：$O\!\left(\sum q^2\log A\right)$，其中 $A\le 2\times 10^9$，最坏约 $5\times10^6$ 次 $\gcd$。
- 空间复杂度：$O(q)$，只需要存下当前作品的角色性格类型。

## 3. 常见易错点

1. **用 `int` 存羁绊指数**。$S(a,b)$ 最大可以接近 $4\times 10^{18}$，而 `int` 只能存到 $2147483647$，会发生截断，答案直接错掉。请用 `long long`（Python 的整数天然没有这个问题）。
2. **用浮点数存羁绊指数**。`double` 只有 53 位有效数字，$10^{18}$ 附近的整数无法精确表示，例如 $S(999999937,999999929)=999999866000004473$ 用 `double` 存下来就已经变了。整数的比较必须用整数。
3. **约分顺序写错**。`a / g / g * b` 会先做两次整除，第一个数被多除一次且向下取整，结果偏小。正确写法是 `(a / g) * (b / g)`。
4. **先算最小公倍数再除**。$\operatorname{lcm}(a,b)=a/g\cdot b$ 可能达到 $4\times 10^{18}$，如果中途用 `int` 存就会溢出；按 $S=(a/g)\cdot(b/g)$ 计算更安全。
5. **以为最优 CP 一定包含最大的性格类型**。反例：候选性格类型 $6,10,15$，最优是 $S(6,10)=15$，而包含最大性格类型 $15$ 的组合只有 $S(6,15)=10$、$S(10,15)=6$。
6. **以为排序后相邻的两个性格类型就是答案**。反例：$6,10,15,21$ 排序后相邻组合最好只有 $S(15,21)=35$，真正的最优是 $S(10,21)=210$。
7. **多测没有重置变量**。本题有多部作品，`bestValue`、`bestA`、`bestB` 必须每部作品重新初始化，否则会把上一部作品的答案带过来。
8. 输出末行有没有换行都算正确，不要为此纠结。

## 4. 参考代码与语法精讲

### 4.1 C++ 参考代码

```cpp
#include <bits/stdc++.h>
using namespace std;

long long gcdll(long long a, long long b)
{
    while (b != 0)
    {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

long long bond(long long a, long long b)
{
    long long g = gcdll(a, b);
    return (a / g) * (b / g);
}

int main()
{
    int n;
    cin >> n;
    for (int k = 0; k < n; k++)
    {
        int q;
        cin >> q;
        vector<long long> a(q);
        for (int i = 0; i < q; i++)
        {
            cin >> a[i];
        }

        long long bestValue = -1;
        long long bestA = a[0];
        long long bestB = a[1];
        for (int i = 0; i < q; i++)
        {
            for (int j = i + 1; j < q; j++)
            {
                long long value = bond(a[i], a[j]);
                if (value > bestValue)
                {
                    bestValue = value;
                    bestA = a[i];
                    bestB = a[j];
                }
            }
        }

        cout << min(bestA, bestB) << " " << max(bestA, bestB) << " " << bestValue << endl;
    }
    return 0;
}
```

#### C++ 语法细节讲解

- `#include <bits/stdc++.h>`：一次性包含常用的标准库，写题时省事。
- `long long gcdll(long long a, long long b)`：自己写一个求最大公约数的函数，参数和返回值都用 `long long`。`while (b != 0)` 就是“还没除完就继续”，循环体里用临时变量 `t` 保存余数，然后依次右移，这是欧几里得算法的标准写法。
- `vector<long long> a(q)`：开一个长度为 $q$ 的数组（`vector` 可以按变量长度创建，比固定长度数组灵活）。把性格类型存成 `long long`，后面算乘积时就不用到处写强制转换。
- 两层 `for`：外层 `i` 从 $0$ 到 $q-1$，内层 `j` 从 `i + 1` 开始，这样每一对只被访问一次，也不会出现自己和自己配对。
- `long long value = bond(...)`：羁绊指数最大接近 $4\times 10^{18}$，必须用 `long long`（64 位，能存到约 $9.2\times 10^{18}$）；`int` 是 32 位，会溢出。
- `bestValue = -1`：初始值取一个比任何答案都小的数，第一次比较一定成立，避免了特判。
- `min` / `max`：标准库函数，用来把较小的性格类型放在前面输出。
- `endl`：输出换行并刷新缓冲。本题数据不大，不需要 `ios::sync_with_stdio(false)` 之类的加速。

### 4.2 Python 参考代码

```python
def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a


def main():
    n = int(input())
    for _ in range(n):
        q = int(input())
        a = list(map(int, input().split()))

        best_value = -1
        best_a = a[0]
        best_b = a[1]
        for i in range(q):
            for j in range(i + 1, q):
                g = gcd(a[i], a[j])
                value = (a[i] // g) * (a[j] // g)
                if value > best_value:
                    best_value = value
                    best_a = a[i]
                    best_b = a[j]

        print(min(best_a, best_b), max(best_a, best_b), best_value)


main()
```

#### Python 语法细节讲解

- `def gcd(a, b)`：定义一个函数。Python 里不需要写参数类型。
- `a, b = b, a % b`：多重赋值，一行同时完成“把 `b` 给 `a`、把 `a % b` 给 `b`”，等价于 C++ 里借助临时变量 `t` 的三步操作，而且不会因为赋值顺序出错。
- `int(input())`：读入一行并转成整数，用来读 $n$ 和 $q$。
- `list(map(int, input().split()))`：`input()` 读整行字符串，`split()` 按空格切成若干段，`map(int, ...)` 把每段转成整数，`list(...)` 收集成列表，这就是读入一行整数数组的标准写法。
- `//` 是整除。这里必须用 `//` 而不是 `/`，因为 `/` 会得到浮点数，丢了精度就不再是精确答案。
- `print(min(...), max(...), best_value)`：`print` 用空格分隔参数，正好是题目要求的三个整数，也会自动换行。
- Python 的整数是任意精度的，不会像 C++ 的 `int` 那样溢出；不过 $5\times10^6$ 次 `gcd` 在极限数据下仍然会比较慢，这是语言常数差异，不影响算法本身。
