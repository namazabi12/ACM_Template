#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
const int mod = 1e9 + 7;
// const int mod = 998244353;
const int inf = 1e9;
// 
// Luogu3690 【模板】Link Cut Tree （动态树）
// 
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
    cout << endl;
    system("pause");
}