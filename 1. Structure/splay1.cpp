#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
const int mod = 1e9 + 7;
//const int mod = 998244353;
const int inf = 1e9;
// 
// Luogu3369【模板】普通平衡树
// 
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

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    ins(inf);
    ins(-inf);
    cin >> n;
    while (n--) {
        int op, x;
        cin >> op >> x;
        if (op == 1) ins(x);
        if (op == 2) del(x);
        if (op == 3) { find(x); cout << tree[tree[root].ch[0]].sz << "\n"; }
        if (op == 4) cout << kth(x + 1) << "\n";
        if (op == 5) cout << tree[nxt(x, 0)].val << "\n";
        if (op == 6) cout << tree[nxt(x, 1)].val << "\n";
    }
    system("pause");
}