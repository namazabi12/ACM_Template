#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define pii pair<int, int>
#define vi vector<int>
const int mod = 1e9 + 7;
// const int mod = 998244353;
const int inf = 1e9;
// 
// Luogu3374 【模板】树状数组 1
// 
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

int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    cin >> n >> m;
    memset(c, 0, sizeof(c));
    for (int i = 1; i <= n; i++) {
        int tmp;
        cin >> tmp;
        upd(i, tmp);
    }
    while (m--) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, k;
            cin >> x >> k;
            upd(x, k);
        }
        if (op == 2) {
            int x, y;
            cin >> x >> y;
            cout << getSum(y) - getSum(x - 1) << "\n";
        }
    }
    system("pause");
}