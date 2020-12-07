#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
const int mod = 1e9 + 7;
//const int mod = 998244353;
const int inf = 1e9;
// 
// Luogu3391 【模板】文艺平衡树
// 
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