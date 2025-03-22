#include <iostream>
#include <vector>


void fastSort(std::vector<int> &arr) {
	// 用来存待排序的区间，后进先出
	std::vector<std::pair<int, int>> stk;// 使用vector模拟栈，也可以直接用stack
	stk.push_back(std::make_pair(0, arr.size() - 1));
	while (!stk.empty()) {
		// 取出最后一个区间
		std::pair<int, int> p = stk.back();
		stk.pop_back();
		int l = p.first, r = p.second;// 区间的左右端点
		int pivot = arr[r];// 选取区间的最后一个元素作为基准
		int i = l - 1; // i代表已经确认小于基准的区间的最后一个元素
		for (int j = l; j < r; j++) {
			if (arr[j] < pivot) {
				i++;
				std::swap(arr[i], arr[j]);
			}
		}
		// 把基准放到正确的位置
		std::swap(arr[i + 1], arr[r]);
		// 把左右两个区间分别入栈
		if (i > l) {
			stk.push_back(std::make_pair(l, i));
		}
		if (i + 2 < r) {
			stk.push_back(std::make_pair(i + 2, r));
		}
	}
}


int main()
{
	std::vector<int> arr = { 1,8,4,2,6,5,0,9,3,7,2};
	for (auto i : arr) {
		std::cout << i << " ";
	}
	fastSort(arr);
	std::cout << std::endl;
	for (auto i : arr) {
		std::cout << i << " ";
	}
	return 0;
}