// https://m.blog.naver.com/kks227/221028710658 공부
// Suffix Array(SA), Longest Common Prefix(LCP)
// O(NlogNlogN)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
vector<int> sa, pos, lcp;

void ConstructSA(string& str) {
	int sLen = static_cast<int>(str.length());
	sa = vector<int>(sLen, 0);
	pos = vector<int>(sLen, 0);
	// 전처리 - 제일 첫 번째 루프에서는 제자리 문자로 비교
	for (int i{}; i < sLen; ++i) {
		sa[i] = i;
		pos[i] = str[i];
	}

	int d{ 1 };
	auto cmp = [&sLen, &d](int i, int j) -> bool {
		// 1. 먼저 자신의 위치의 문자를 비교.
		if (pos[i] != pos[j]) return pos[i] < pos[j];

		// 문자가 같으면 d칸 뒤의 문자끼리 비교
		i += d, j += d;
		return (i < sLen && j < sLen) ? (pos[i] < pos[j]) : (i > j);
	};

	// d를 2배씩 늘려가면서 매번 앞에서부터 d*2글자만 보고 접미사 정렬
	for (;; d <<= 1) {
		sort(begin(sa), end(sa), cmp);

		// temp : 새로운 그룹 번호
		vector<int> temp(sLen, 0);
		// 앞에서부터 훑으면서 각 접미사가 서로 다른 그룹에 속할 때마다 그룹 번호 증가.
		for (int i{}; i < sLen - 1; ++i) {
			temp[i + 1] = temp[i] + static_cast<int>(cmp(sa[i], sa[i + 1]));
		}
		// pos 배열을 temp 배열로 대체.
		for (int i{}; i < sLen; ++i) {
			pos[sa[i]] = temp[i];
		}

		// 모든 접미사가 다른 그룹으로 나뉘어졌다면 종료
		if (temp[sLen - 1] == sLen - 1) break;
	}
}

void ConstructLCP(string& str) {
	int sLen = static_cast<int>(str.length());
	lcp = vector<int>(sLen, 0);

	// sa[i]는 사전순으로 i번째 위치의 접미사가 원래 문자열의 몇 번째 문자부터 시작하냐를 의미
	// pos[i] = str[i:]가 sa의 몇 번째에 있는가 (pos[sa[i]] = i)
	// 제일 긴 접미사(str)부터 시작함.
	// 매 루프마다 k > 0이면 k--
	for (int i{}, k{}; i < sLen; ++i, k = max(k - 1, 0)) {
		// 마지막 접미사(사전 순으로 마지막인 접미사)면 아무것도 안 함.
		if (pos[i] == sLen - 1) {
			lcp[pos[i]] = -1; // X 표시
			continue;
		}

		// 바로 아래 인접한 접미사와 비교하여 앞에서부터 몇 개의 글자가 일치하는지 카운팅
		// pos[i] + 1 (사전순으로 현재 문자열 다음 문자열의 sa index)
		int j = sa[pos[i] + 1];
		while (str[i + k] == str[j + k]) ++k;
		lcp[pos[i]] = k;
	}
}

int main() {
	cin.tie(nullptr), cout.tie(nullptr), ios::sync_with_stdio(false);
	string str;
	cin >> str;

	// sa[i]는 사전순으로 i번째 위치의 접미사가 원래 문자열의 몇 번째 문자부터 시작하냐를 의미함.
	ConstructSA(str);

	// LCP : 정렬된 접미사들끼리, 인접한 접미사와 몇 칸까지 겹치는지.
	// 9248번 문제에서는 사전순으로 바로 앞의 접미사와 몇 자리까지 겹치느냐로 얘기하는데, 사실 바로 앞, 뒤 여부는 별로 관계 없음.
	// 코드 포스의 기준대로, 바로 뒤의 접미사와 몇 자리까지 겹치느냐로 설명함.
	// 여기서의 LCP[k] = k번째 접미사와 (k+1)번째 접미사가 앞에서부터 몇 글자가 같은지를 의미함.
	ConstructLCP(str);

	/*
	// Print
	cout << "suffix \t\t sa \t\t lcp\n";
	for (int i{}; i < static_cast<int>(str.length()); ++i) {
		// cout << i + 1 << ": " << str.substr(i) << '\n';
		cout << str.substr(sa[i]) << "\t\t" << sa[i] << "\t\t" << lcp[i] << '\n';
	}
	*/

	for (int& v : sa) cout << v + 1 << ' ';
	cout << "\nx ";
	for (int i{}; i < static_cast<int>(str.length()) - 1; ++i) cout << lcp[i] << ' ';
	cout << '\n';

	return 0;
}