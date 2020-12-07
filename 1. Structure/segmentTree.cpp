#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
const int mod = 1e9 + 7;
//const int mod = 998244353;
const int inf = 1e9;
// 
// Luogu3372【模板】线段树 1
// 
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
// ll query(int i, int x, int y) {
//     if (tree[i].l >= x && tree[i].r <= y) return tree[i].mx;
//     push_down(i);
//     int mid = (tree[i].l + tree[i].r) / 2;
//     ll ans = -1e18;
//     if (x <= mid) ans = max(ans, query(2 * i, x, y));
//     if (y > mid) ans = max(ans, query(2 * i + 1, x, y));
// }

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    buildTree(1, 1, n);
    while (m--) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, y, k;
            cin >> x >> y >> k;
            upd(1, x, y, k);
        }
        if (op == 2) {
            int x, y;
            cin >> x >> y;
            cout << query(1, x, y) << "\n";
        }
    }
    cout << endl;
    system("pause");
}