#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <chrono>

#define ll unsigned long long

const ll INF = INT64_MAX;

struct TItem {
    ll weight;
    int id;

    friend bool operator < (const TItem & lhs, const TItem & rhs) {
        if (lhs.weight != rhs.weight) {
            return lhs.weight > rhs.weight;
        } else {
            return lhs.id < rhs.id;
        }
    }
};

int main() {
    int n, m, start, finish, u, v;
    ll weight;
    using duration_t = std::chrono::microseconds;
    std::chrono::time_point<std::chrono::system_clock> startt, end;
    startt = std::chrono::system_clock::now();
    std::cin >> n >> m >> start >> finish;
    start--, finish--;
    std::vector<std::vector<std::pair<int, ll>>> graph(n);
    for (int i = 0; i < m; ++i) {
        std::cin >> u >> v >> weight;
        u--, v--;
        graph[u].push_back(std::make_pair(v, weight));
        graph[v].push_back(std::make_pair(u, weight));
    }
    std::vector<ll> d(n, INF);
    std::priority_queue<TItem> pq;
    d[start] = 0;
    std::vector<bool> marks(n);
    pq.push({0, start});
    while (!pq.empty()) {
        TItem curr = pq.top();
        pq.pop();
        u = curr.id;
        if (u == finish) {
            break;
        }
        if (!marks[u]) {
            for (size_t i = 0; i < graph[u].size(); ++i) {
                v = graph[u][i].first, weight = graph[u][i].second;
                if (d[u] + weight < d[v]) {
                    d[v] = d[u] + weight;
                    pq.push({d[v],v});
                }
            }
            marks[u] = true;
        }
    }
    if (d[finish] == INF) {
        std::cout << "No solution\n";
    } else {
        std::cout << d[finish] << '\n';
    }
    end = std::chrono::system_clock::now();
    auto time = std::chrono::duration_cast<duration_t>(end - startt).count();
    std::cout << time << "ms";
}


