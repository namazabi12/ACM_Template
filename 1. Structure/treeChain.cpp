#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
// const int mod = 1e9 + 7;
// const int mod = 998244353;
const int inf = 1e9;
// 
// Luogu3384 【模板】轻重链剖分
// 
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
    cout << endl;
    system("pause");
}