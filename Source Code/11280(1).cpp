// https://m.blog.naver.com/kks227/220803009418
// 공부
// 음수 정점 처리
// SCC - 또 짜려니까 까먹었따.. (kosaraju)

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

vector<vector<int>> graph, revGraph;

vector<int> visited;

vector<int> SCCnums; // SCCnums[i] : i가 속한 SCC 번호
int SCCcnts{};

stack<int> S;

void FillStack(int curr) {
	visited[curr] = 1;

	for (int& next : graph[curr]) {
		if (!visited[next]) {
			FillStack(next);
		}
	}

	S.push(curr);
}

void CalculateSCC(int curr) { // SCC 구하는 벡터 생략
	visited[curr] = 1;
	SCCnums[curr] = SCCcnts;

	for (int& next : revGraph[curr]) {
		if (!visited[next]) {
			CalculateSCC(next);
		}
	}

}

int main() {
	cin.tie(nullptr)->sync_with_stdio(false);
	int N, M; // 변수의 개수, 절의 개수
	cin >> N >> M;
	graph = vector<vector<int>>(N * 2); // N = 3 -> 0 ~ 5
	revGraph = vector<vector<int>>(N * 2); // N = 3 -> 0 ~ 5
	
	for (int m{}; m < M; ++m) {
		int a, b;
		cin >> a >> b;
		// -1 -> 0, 1 -> 1
		// -2 -> 2, 2 -> 3
		// -3 -> 4, 3 -> 5
		if (a < 0) a = -2 * (a + 1);
		else a = 2 * a - 1;
		if (b < 0) b = -2 * (b + 1);
		else b = 2 * b - 1;
		// 만들 간선
		int notA = (a & 1) == 0 ? a + 1 : a - 1;
		int notB = (b & 1) == 0 ? b + 1 : b - 1;
		graph[notA].push_back(b); // ~a -> b
		graph[notB].push_back(a); // ~b -> a
		revGraph[b].push_back(notA);
		revGraph[a].push_back(notB);
	}

	SCCnums = vector<int>(N * 2, 0);
	visited = vector<int>(N * 2, 0);
	// SCC 찾기
	for (int n{}; n < 2 * N; ++n) {
		if (visited[n] == 0) {
			FillStack(n);
		}
	}
	vector<int>(N * 2, 0).swap(visited);

	while (!S.empty()) {
		int now = S.top(); S.pop();

		if (!visited[now]) {
			CalculateSCC(now);
			++SCCcnts;
		}
	}

	bool chk = true;
	for (int n{}; n < N; ++n) {
		// 2n, 2n+1 서로 반대
		// 서로 반대 관계인 정점이 하나라도 하나의 SCC에 속하면 절대로 참이 나올 수 없음.
		int curr = n << 1;
		if (SCCnums[curr] == SCCnums[curr + 1]) {
			chk = false;
			break;
		}
	}
	if (chk) cout << "1\n";
	else cout << "0\n";

	return 0;
}