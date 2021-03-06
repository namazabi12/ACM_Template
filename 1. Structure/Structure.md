# 数据结构

### 1.并查集

```c++
const int N = 1e5 + 5;
int fa[N];
 
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
const int N = 1e5 + 5;
int a[N];
ll lazy[4 * N];

struct node {
    int l, r;
    ll sum, mx;
} tree[4 * N];

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
const int N = 1e5 + 5;
int root = 0, tot = 0, n;

struct Splay {
    int ch[2], fa, cnt, val, sz;
} tree[N];

void push_up(int x) {
    tree[x].sz = tree[tree[x].ch[0]].sz + tree[tree[x].ch[1]].sz + tree[x].cnt;
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
    push_up(x), push_up(y);
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

void ins(int x) {
    int u = root, fa = 0;
    while (u && tree[u].val != x) {
        fa = u;
        u = tree[u].ch[x > tree[u].val];
    }
    if (u) tree[u].cnt++;
    else {
        u = ++tot;
        if (fa) tree[fa].ch[x > tree[fa].val] = u;
        tree[tot].ch[0] = tree[tot].ch[1] = 0;
        tree[tot].fa = fa;
        tree[tot].val = x;
        tree[tot].cnt = tree[tot].sz = 1;
    }
    splay(u, 0);
}

void find(int x) {
    int u = root;
    if (!u) return ;
    while (tree[u].ch[x > tree[u].val] && x != tree[u].val) {
        u = tree[u].ch[x > tree[u].val];
    }
    splay(u, 0);
}

int nxt(int x, int fg) {
    find(x);
    int u = root;
    if ((tree[u].val > x && fg) || (tree[u].val < x && !fg)) return u;
    u = tree[u].ch[fg];
    while (tree[u].ch[fg ^ 1]) u = tree[u].ch[fg ^ 1];
    return u;
}

void del(int x) {
    int pre = nxt(x, 0);
    int suf = nxt(x, 1);
    splay(pre, 0);
    splay(suf, pre);
    int de = tree[suf].ch[0];
    if (tree[de].cnt > 1) {
        tree[de].cnt--;
        splay(de, 0);
    } else {
        tree[suf].ch[0] = 0;
    }
}

int kth(int x) {
    int u = root;
    if (tree[u].sz < x) return 0;
    while (true) {
        int y = tree[u].ch[0];
        if (x > tree[y].sz + tree[u].cnt) {
            x -= tree[y].sz + tree[u].cnt;
            u = tree[u].ch[1];
        } else {
            if (tree[y].sz >= x) u = y;
            else return tree[u].val;
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
    cin.tie(0);
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
    system("pause");
}
```

### 5.树状数组

```c++
const int N = 500005;
int c[N], n, m;

int lowbit(int x) {
    return x & (-x);
}

void upd(int i, int k) {
    while (i <= n) {
        c[i] += k;
        i += lowbit(i);
    }
}

int getSum(int i) {
    int res = 0;
    while (i > 0) {
        res += c[i];
        i -= lowbit(i);
    }
    return res;
}
```

### 6.主席树1(可持久化数组)

```c++
const int N = 1000005;
int n, m, cnt;
int a[N], root[N];

struct node {
    int l, r, val;
} tree[N * 20];

void buildTree(int i, int l, int r) {
    cnt = max(cnt, i);
    if (l == r) {
        tree[i].val = a[l];
        return ;
    }
    tree[i].l = 2 * i;
    tree[i].r = 2 * i + 1;
    int mid = (l + r) / 2;
    buildTree(tree[i].l, l, mid);
    buildTree(tree[i].r, mid + 1, r);
}

int upd(int i, int l, int r, int x, int k) {
    tree[++cnt] = tree[i];
    i = cnt;
    if (l == r) {
        tree[i].val = k;
        return i;
    }
    int mid = (l + r) / 2;
    if (x <= mid) tree[i].l = upd(tree[i].l, l, mid, x, k);
    else tree[i].r = upd(tree[i].r, mid + 1, r, x, k);  
    return i;
}

int query(int i, int l, int r, int x) {
    if (l == r) return tree[i].val;
    int mid = (l + r) / 2;
    if (x <= mid) return query(tree[i].l, l, mid, x);
    else return query(tree[i].r, mid + 1, r, x);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> a[i];
    buildTree(1, 1, n);
    root[0] = 1;
    for (int i = 1; i <= m; i++) {
        int v, op;
        cin >> v >> op;
        if (op == 1) {
            int x, k;
            cin >> x >> k;
            root[i] = upd(root[v], 1, n, x, k);
        }
        if (op == 2) {
            int x;
            cin >> x;
            root[i] = root[v];
            cout << query(root[v], 1, n, x) << '\n';
        }
    }
    // cout << endl;
    system("pause");
}
```

### 7.主席树2(区间第k大)

```c++
const int N = 200005;
int n, m, cnt;
int root[N], rk[N];

struct node {
    int l, r, sum;
} tree[N * 20];

struct value {
    int x, id;

    bool operator <(const value a) const {
        return x < a.x;
    }
} val[N];

void upd(int num, int &rt, int l, int r) {
    tree[++cnt] = tree[rt];
    rt = cnt;
    tree[rt].sum++;
    if (l == r) return;
    int mid = (l + r) / 2;
    if (num <= mid) upd(num, tree[rt].l, l, mid);
    else upd(num, tree[rt].r, mid + 1, r);
}

int query(int i, int j, int k, int l, int r) {
    int d = tree[tree[j].l].sum - tree[tree[i].l].sum;
    if (l == r) return l;
    int mid = (l + r) / 2;
    if (k <= d) return query(tree[i].l, tree[j].l, k, l, mid);
    else return query(tree[i].r, tree[j].r, k - d, mid + 1, r);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> val[i].x;
        val[i].id = i;
    }
    sort(val + 1, val + n + 1);
    for (int i = 1; i <= n; i++) rk[val[i].id] = i;
    cnt = root[0] = tree[0].l = tree[0].r = tree[0].sum = 0;
    for (int i = 1; i <= n; i++) {
        root[i] = root[i - 1];
        upd(rk[i], root[i], 1, n);
    }
    for (int i = 1; i <= m; i++) {
        int l, r, k;
        cin >> l >> r >> k;
        cout << val[query(root[l - 1], root[r], k, 1, n)].x << "\n";
    }
    system("pause");
}
```

### 8.树链剖分

```c++
const int N = 100005;
int n, m, r, mod, cnt = 0;
int sz[N], dep[N], fa[N], son[N], top[N], id[N], a[N], w[N];
ll lazy[4 * N];
vi eg[N];

struct node {
    int l, r;
    ll sum, mx;
} tree[4 * N];

void dfs1(int x) {
    sz[x] = 1;
    for (int &i : eg[x]) {
        if (i == fa[x]) continue;
        fa[i] = x;
        dep[i] = dep[x] + 1;
        dfs1(i);
        sz[x] += sz[i];
        if (!(son[x] && sz[i] <= sz[son[x]])) son[x] = i;
    }
}

void dfs2(int x, int t) {
    id[x] = ++cnt;
    a[cnt] = w[x];
    top[x] = t;
    if (son[x]) dfs2(son[x], t);
    for (int &i : eg[x]) {
        if (i != fa[x] && i != son[x]) dfs2(i, i); 
    }
}

int lca(int x, int y) {
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]]) {
            swap(x, y);
        }
        x = top[x];
    }
    return dep[x] < dep[y] ? x : y;
}

void push_up(int i) {
    tree[i].sum = (tree[2 * i].sum + tree[2 * i + 1].sum) % mod;
}

void buildTree(int i, int l, int r) {
    tree[i].l = l;
    tree[i].r = r;
    lazy[i] = 0;
    if (l == r) {
        tree[i].sum = a[l];
        return ;
    }
    int mid = (l + r) / 2;
    buildTree(2 * i, l, mid);
    buildTree(2 * i + 1, mid + 1, r);
    push_up(i);
}

void push_down(int i) {
    if (lazy[i]) {
        tree[2 * i].sum += (tree[2 * i].r - tree[2 * i].l + 1) * lazy[i];
        tree[2 * i].sum %= mod;
        tree[2 * i + 1].sum += (tree[2 * i + 1].r - tree[2 * i + 1].l + 1) * lazy[i];
        tree[2 * i + 1].sum %= mod;
        lazy[2 * i] += lazy[i];
        lazy[2 * i + 1] += lazy[i];
        lazy[i] = 0;
    }
}

void upd(int i, int x, int y, ll k) {
    if (x <= tree[i].l && y >= tree[i].r) {
        tree[i].sum += (tree[i].r - tree[i].l + 1) * k;
        tree[i].sum %= mod;
        lazy[i] += k;
        return;
    }
    push_down(i);
    int mid = (tree[i].l + tree[i].r) / 2;
    if (x <= mid) upd(2 * i, x, y, k);
    if (y > mid) upd(2 * i + 1, x, y, k);
    push_up(i);
}

ll query(int i, int x, int y) {
    if (tree[i].l >= x && tree[i].r <= y) return tree[i].sum;
    if (tree[i].l > y || tree[i].r < x) return 0;
    push_down(i);
    int mid = (tree[i].l + tree[i].r) / 2;
    ll ans = 0;
    if (x <= mid) ans += query(2 * i, x, y);
    if (y > mid) ans += query(2 * i + 1, x, y);
    return ans % mod;
}

ll queryRange(int x, int y) {
    ll ans = 0;
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]]) swap(x, y);
        ans += query(1, id[top[x]], id[x]);
        ans %= mod;
        x = fa[top[x]];
    }
    if (dep[x] > dep[y]) swap(x, y);
    ans += query(1, id[x], id[y]);
    return ans % mod;
}

ll querySon(int x) {
    return query(1, id[x], id[x] + sz[x] - 1);
}

void updRange(int x, int y, int k) {
    while (top[x] != top[y]) {
        if (dep[top[x]] < dep[top[y]]) swap(x, y);
        upd(1, id[top[x]], id[x], k);
        x = fa[top[x]];
    }
    if (dep[x] > dep[y]) swap(x, y);
    upd(1, id[x], id[y], k);
}

void updSon(int x, int k) {
    upd(1, id[x], id[x] + sz[x] - 1, k);
}

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    cin >> n >> m >> r >> mod;
    for (int i = 1; i <= n; i++) {
        cin >> w[i];
        fa[i] = i;
        son[i] = 0;
    }
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        eg[u].push_back(v);
        eg[v].push_back(u);
    }
    dep[r] = 1;
    dfs1(r);
    dfs2(r, r);
    buildTree(1, 1, n);
    while (m--) {
        int op, x, y, k;
        cin >> op;
        if (op == 1) {
            cin >> x >> y >> k;
            updRange(x, y, k % mod);
        }
        if (op == 2) {
            cin >> x >> y;
            cout << queryRange(x, y) << "\n";
        }
        if (op == 3) {
            cin >> x >> k;
            updSon(x, k);
        }
        if (op == 4) {
            cin >> x;
            cout << querySon(x) << "\n";
        }
    }
    // cout << endl;
    system("pause");
}
```

### 9.LCT
```c++
const int N = 100005;
struct node {
    int ch[2], fa, val, sum, rev;
} tree[N];

void push_up(int x) {
    tree[x].sum = tree[tree[x].ch[0]].sum ^ tree[tree[x].ch[1]].sum ^ tree[x].val;
}

void push_down(int x) {
    if (tree[x].rev) {
        swap(tree[x].ch[0], tree[x].ch[1]);
        if (tree[x].ch[0]) tree[tree[x].ch[0]].rev ^= 1;
        if (tree[x].ch[1]) tree[tree[x].ch[1]].rev ^= 1;
        tree[x].rev = 0;
    }
}

int id(int x) {
    if (tree[tree[x].fa].ch[0] == x) return 0;
    if (tree[tree[x].fa].ch[1] == x) return 1;
    return -1;
}

void push_all(int x) {
    if (id(x) != -1) push_all(tree[x].fa);
    push_down(x);
}

void connect(int x, int f, int op) {
    tree[x].fa = f;
    if (op == 1) tree[f].ch[1] = x;
    if (op == 0) tree[f].ch[0] = x;
}

void rotate(int x) {
    int y = tree[x].fa;
    int z = tree[y].fa;
    int opy = id(y);
    int opx = id(x);
    int u = 0;
    if (opx == 1) u = tree[x].ch[0];
    if (opx == 0) u = tree[x].ch[1];
    connect(u, y, opx);
    connect(y, x, opx ^ 1);
    connect(x, z, opy);
    push_up(y);
    push_up(x);
}

void splay(int x) {
    push_all(x);
    while (id(x) != -1) {
        int y = tree[x].fa;
        if (id(y) != -1) {
            if (id(x) == id(y)) rotate(y);
            else rotate(x);
        } 
        rotate(x);
    }
}

void access(int x) {
    int y = 0;
    while (x) {
        splay(x);
        tree[x].ch[1] = y;
        push_up(x);
        y = x;
        x = tree[x].fa;
    }
}

void make_root(int x) {
    access(x);
    splay(x);
    swap(tree[x].ch[0], tree[x].ch[1]);
    if (tree[x].ch[0]) tree[tree[x].ch[0]].rev ^= 1;
    if (tree[x].ch[1]) tree[tree[x].ch[1]].rev ^= 1;
}

void split(int x, int y) {
    make_root(x);
    access(y);
    splay(y);
}

int find_root(int x) {
    access(x);
    splay(x);
    while (tree[x].ch[0]) {
        push_down(x);
        x = tree[x].ch[0];
    }
    splay(x);
    return x;
}

bool link(int x, int y) {
    make_root(x);
    if (find_root(y) == x) return 0;
    tree[x].fa = y;
    return 1;
}

bool cut(int x, int y) {
    if (find_root(x) != find_root(y)) return 0;
    split(x, y);
    if (tree[x].fa != y || tree[x].ch[1]) return 0;
    tree[x].fa = tree[y].ch[0] = 0;
    push_up(x);
    return 1;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int n, m;
    cin >> n >> m;
    tree[0].sum = 0;
    for (int i = 1; i <= n; i++) {
        cin >> tree[i].val;
        tree[i].sum = tree[i].val;
    }
    for (int i = 1; i <= m; i++) {
        int op, x, y;
        cin >> op >> x >> y;
        if (op == 0) {
            split(x, y);
            cout << tree[y].sum << "\n";
        }
        if (op == 1) link(x, y);
        if (op == 2) cut(x, y);
        if (op == 3) {
            splay(x);
            tree[x].val = y;
        }
    }
    // cout << endl;
    system("pause");
}
```