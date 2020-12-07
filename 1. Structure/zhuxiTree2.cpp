#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
const int mod = 1e9 + 7;
// const int mod = 998244353;
const int inf = 1e9;
// 
// Luogu3834 【模板】可持久化线段树 2（主席树）
// 
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
    // cout << endl;
    system("pause");
}