# 数据结构

### 1.并查集

```c++
const int maxn = 1e5 + 5;
int fa[maxn];
 
int ff(int x) {
    return fa[x] == x ? x : fa[x] = ff(fa[x]);
}
 
int uni(int x, int y) {
    int rx = ff(x);
    int ry = ff(y);
    if (rx == ry) return 0;
    else {
        fa[rx] = ry;
        return 1;
    }
}
```

### 2.线段树

```c++
const int maxn = 1e5 + 5;
int a[maxn];
ll lazy[4 * maxn];

struct node {
    int l, r;
    ll sum, mx;
} tree[4 * maxn];

void push_up(int i) {
    tree[i].sum = tree[2 * i].sum + tree[2 * i + 1].sum;
    tree[i].mx = max(tree[2 * i].mx, tree[2 * i + 1].mx);
}

void buildTree(int i, int l, int r) {
    tree[i].l = l;
    tree[i].r = r;
    lazy[i] = 0;
    if (l == r) {
        tree[i].sum = a[l];
        tree[i].mx = a[l];
        return;
    }
    int mid = (l + r) / 2;
    buildTree(2 * i, l, mid);
    buildTree(2 * i + 1, mid + 1, r);
    push_up(i);
}

void push_down(int i) {
    if (lazy[i]) {
        tree[2 * i].sum += (tree[2 * i].r - tree[2 * i].l + 1) * lazy[i];
        tree[2 * i + 1].sum += (tree[2 * i + 1].r - tree[2 * i + 1].l + 1) * lazy[i];
        tree[2 * i].mx += lazy[i];
        tree[2 * i + 1].mx += lazy[i];
        lazy[2 * i] += lazy[i];
        lazy[2 * i + 1] += lazy[i];
        lazy[i] = 0;
    }
}

void upd(int i, int x, int y, ll k) {
    if (x <= tree[i].l && y >= tree[i].r) {
        tree[i].sum += (tree[i].r - tree[i].l + 1) * k;
        tree[i].mx += k;
        lazy[i] += k;
        return;
    }
    push_down(i);
    int mid = (tree[i].l + tree[i].r) / 2;
    if (x <= mid) upd(2 * i, x, y, k);
    if (y > mid) upd(2 * i + 1, x, y, k);
    push_up(i);
}

//  Sum
ll query(int i, int x, int y) {
    if (tree[i].l >= x && tree[i].r <= y) return tree[i].sum;
    if (tree[i].l > y || tree[i].r < x) return 0;
    push_down(i);
    int mid = (tree[i].l + tree[i].r) / 2;
    ll ans = 0;
    if (x <= mid) ans += query(2 * i, x, y);
    if (y > mid) ans += query(2 * i + 1, x, y);
    return ans;
}

//  Max
ll query(int i, int x, int y) {
    if (tree[i].l >= x && tree[i].r <= y) return tree[i].mx;
    push_down(i);
    int mid = (tree[i].l + tree[i].r) / 2;
    ll ans = -1e18;
    if (x <= mid) ans = max(ans, query(2 * i, x, y));
    if (y > mid) ans = max(ans, query(2 * i + 1, x, y));
}
```

### 3.Splay(前驱后继、第k大)

```c++
const int N = 1e5 + 5;
int root = 0, tot = 0, n;
 
struct Splay {
    int ch[2], fa, cnt, val, sz;
} tree[N];
 
void push_up(int x) {
    tree[x].sz = tree[tree[x].ch[0]].sz + tree[tree[x].ch[1]].sz + tree[x].cnt;
}
 
int id(int x) {
    return tree[tree[x].fa].ch[1] == x;
}
 
void rotate(int x) {
    int y = tree[x].fa;
    int z = tree[y].fa;
    int k = id(x);
    tree[z].ch[id(y)] = x;
    tree[x].fa = z;
    tree[y].ch[k] = tree[x].ch[k ^ 1];
    tree[tree[x].ch[k ^ 1]].fa = y;
    tree[x].ch[k ^ 1] = y;
    tree[y].fa = x;
    push_up(y), push_up(x);
}
 
void splay(int x, int goal) {
    while (tree[x].fa != goal) {
        int y = tree[x].fa;
        int z = tree[y].fa;
        if (z != goal) {
            if (id(x) == id(y)) rotate(y);
            else rotate(x);
        }
        rotate(x);
    }
    if (goal == 0) root = x;
}
 
void ins(int x) {
    int u = root, fa = 0;
    while (u && tree[u].val != x) {
        fa = u;
        u = tree[u].ch[x > tree[u].val];
    }
    if (u) tree[u].cnt++;
    else {
        u = ++tot;
        if (fa) tree[fa].ch[x > tree[fa].val] = u;
        tree[tot].ch[0] = tree[tot].ch[1] = 0;
        tree[tot].fa = fa;
        tree[tot].val = x;
        tree[tot].cnt = tree[tot].sz = 1;
    }
    splay(u, 0);
}
 
void find(int x) {
    int u = root;
    if (!u) return ;
    while (tree[u].ch[x > tree[u].val] && x != tree[u].val) {
        u = tree[u].ch[x > tree[u].val];
    }
    splay(u, 0);
}
 
int nxt(int x, int fg) {
    find(x);
    int u = root;
    if ((tree[u].val > x && fg) || (tree[u].val < x && !fg)) return u;
    u = tree[u].ch[fg];
    while (tree[u].ch[fg ^ 1]) u = tree[u].ch[fg ^ 1];
    return u;
}
 
void del(int x) {
    int pre = nxt(x, 0);
    int suf = nxt(x, 1);
    splay(pre, 0);
    splay(suf, pre);
    int de = tree[suf].ch[0];
    if (tree[de].cnt > 1) {
        tree[de].cnt--;
        splay(de, 0);
    } else {
        tree[suf].ch[0] = 0;
    }
}
 
int kth(int x) {
    int u = root;
    if (tree[u].sz < x) return 0;
    while (true) {
        int y = tree[u].ch[0];
        if (x > tree[y].sz + tree[u].cnt) {
            x -= tree[y].sz + tree[u].cnt;
            u = tree[u].ch[1];
        } else {
            if (tree[y].sz >= x) u = y;
            else return tree[u].val;
        }
    }
}
 
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    ins(inf);
    ins(-inf);
    cin >> n;
    while (n--) {
        int op, x;
        cin >> op >> x;
        if (op == 1) ins(x);
        if (op == 2) del(x);
        if (op == 3) {find(x);cout << tree[tree[root].ch[0]].sz<< "\n";}
        if (op == 4) cout << kth(x + 1) << "\n";
        if (op == 5) cout << tree[nxt(x, 0)].val << "\n";
        if (op == 6) cout << tree[nxt(x, 1)].val << "\n";
    }
}
```

### 4.Splay2(区间翻转)

```c++
const int N = 1e5 + 5;
int root = 0, tot = 0, n, m, a[N], lazy[N];

struct Splay {
    int ch[2], fa, cnt, val, sz;
} tree[N];

void push_up(int x) {
    tree[x].sz = tree[tree[x].ch[0]].sz + tree[tree[x].ch[1]].sz + 1;
}

void push_down(int x) {
    if (lazy[x]) {
        lazy[tree[x].ch[0]] ^= 1;
        lazy[tree[x].ch[1]] ^= 1;
        swap(tree[x].ch[0], tree[x].ch[1]);
        lazy[x] = 0;
    }
}

int id(int x) {
    return tree[tree[x].fa].ch[1] == x;
}

void rotate(int x) {
    int y = tree[x].fa;
    int z = tree[y].fa;
    int k = id(x);
    tree[z].ch[id(y)] = x;
    tree[x].fa = z;
    tree[y].ch[k] = tree[x].ch[k ^ 1];
    tree[tree[x].ch[k ^ 1]].fa = y;
    tree[x].ch[k ^ 1] = y;
    tree[y].fa = x;
    push_up(y), push_up(x);
}

void splay(int x, int goal) {
    while (tree[x].fa != goal) {
        int y = tree[x].fa;
        int z = tree[y].fa;
        if (z != goal) {
            if (id(x) == id(y)) rotate(y);
            else rotate(x);
        }
        rotate(x);
    }
    if (goal == 0) root = x;
}

int find(int x) {
    int u = root;
    while (true) {
        push_down(u);
        if (tree[u].ch[0] && x <= tree[tree[u].ch[0]].sz) u = tree[u].ch[0];
        else {
            int tmp = (tree[u].ch[0] ? tree[tree[u].ch[0]].sz : 0) + 1;
            if (x == tmp) return u;
            x -= tmp;
            u = tree[u].ch[1];
        }
    }
}

int build(int l, int r, int rt) {
    int mid = (l + r) / 2;
    tree[mid].fa = rt;
    tree[mid].val = a[mid];
    if (l < mid) tree[mid].ch[0] = build(l, mid - 1, mid);
    if (r > mid) tree[mid].ch[1] = build(mid + 1, r, mid);
    push_up(mid);
    return mid;
}

void print(int x) {
    push_down(x);
    if (tree[x].ch[0]) print(tree[x].ch[0]);
    a[++tot] = tree[x].val;
    if (tree[x].ch[1]) print(tree[x].ch[1]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    memset(lazy, 0, sizeof(lazy));
    for (int i = 1; i <= n + 2; i++) a[i] = i - 1;
    root = build(1, n + 2, 0);
    while (m--) {
        int x, y;
        cin >> x >> y;
        x = find(x);
        y = find(y + 2);
        splay(x, 0);
        splay(y, x);
        lazy[tree[tree[root].ch[1]].ch[0]] ^= 1;
    }
    print(root);
    for (int i = 2; i <= n + 1; i++) {
        cout << a[i] << ' ';
    }
}
```

### 5.树状数组

```c++
int c[100005];
 
int lowbit(int x) {
    return x & (-x);
}
 
void upd(int i, int k) {
    while (i <= n) {
        c[i] += k;
        i += lowbit(i);
    }
}
 
int getSum(int i) {
    int res = 0;
    while (i > 0) {
        res += c[i];
        i -= lowbit(i);
    }
    return res;
}
```

### 6.主席树

```c++
const int maxn = 1e5 + 10;
int n, m;
int cnt;

struct node {
    int L, R;//分别指向左右子树
    int sum;//该节点所管辖区间范围内数的个数
    node() {
        sum = 0;
    }
} Tree[maxn * 20];

struct value {
    int x;//值的大小
    int id;//离散之前在原数组中的位置
} Value[maxn];

bool cmp(value v1, value v2) {
    return v1.x < v2.x;
}

int root[maxn];//多颗线段树的根节点
int rk[maxn];//原数组离散之后的数组
void init() {
    cnt = 1;
    root[0] = 0;
    Tree[0].L = Tree[0].R = Tree[0].sum = 0;
}

void update(int num, int &rt, int l, int r) {
    Tree[cnt++] = Tree[rt];
    rt = cnt - 1;
    Tree[rt].sum++;
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (num <= mid) update(num, Tree[rt].L, l, mid);
    else update(num, Tree[rt].R, mid + 1, r);
}

int query(int i, int j, int k, int l, int r) {
    int d = Tree[Tree[j].L].sum - Tree[Tree[i].L].sum;
    if (l == r) return l;
    int mid = (l + r) >> 1;
    if (k <= d) return query(Tree[i].L, Tree[j].L, k, l, mid);
    else return query(Tree[i].R, Tree[j].R, k - d, mid + 1, r);
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &Value[i].x);
        Value[i].id = i;
    }
    //进行离散化
    sort(Value + 1, Value + n + 1, cmp);
    for (int i = 1; i <= n; i++) {
        rk[Value[i].id] = i;
    }
    init();
    for (int i = 1; i <= n; i++) {
        root[i] = root[i - 1];
        update(rk[i], root[i], 1, n);
    }
    int left, right, k;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &left, &right, &k);
        printf("%d\n", Value[query(root[left - 1], root[right], k, 1, n)].x);
    }
    return 0;
}
```

