#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
const int mod = 1e9 + 7;
//const int mod = 998244353;
const int inf = 1e9;
const int maxn = 1e5 + 5;
int fa[maxn];

int ff(int x) {
    return fa[x] == x ? x : fa[x] = ff(fa[x]);
}

int uni(int x, int y) {
    int rx = ff(x);
    int ry = ff(y);
    if (rx == ry) return 0;
    else {
        fa[rx] = ry;
        return 1;
    }
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) fa[i] = i;
    while (m--) {
        int op, x, y;
        cin >> op >> x >> y;
        if (op == 1) {
            uni(x, y);
        }
        if (op == 2) {
            if (ff(x) == ff(y)) puts("Y");
            else puts("N");
        }
    }
    system("pause");
}