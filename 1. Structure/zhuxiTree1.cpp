#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
const int mod = 1e9 + 7;
// const int mod = 998244353;
const int inf = 1e9;
// 
// Luogu3919 【模板】可持久化线段树 1（可持久化数组）
// 
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