#include <iostream>
#include <vector>

template<typename T>
void fastSort(std::vector<T> &arr) {
	// 用来存待排序的区间，后进先出
	std::vector<std::pair<int, int>> stk;// 使用vector模拟栈，也可以直接用stack
	stk.push_back(std::make_pair(0, arr.size() - 1));
	while (!stk.empty()) {
		// 取出最后一个区间
		std::pair<int, int> p = stk.back();
		stk.pop_back();
		int l = p.first, r = p.second;// 区间的左右端点
		T pivot = arr[r];// 选取区间的最后一个元素作为基准
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

template<typename T>
void adjustHeap(std::vector<T>& arr, int ind, int length) {
	T temp = arr[ind];
	for (int i = ind * 2 + 1; i < length; i = i * 2 + 1) {
		if (i + 1 < length && arr[i] < arr[i + 1]) {
			i++;
		}
		if (arr[i] > temp) {
			arr[ind] = arr[i];
			ind = i;
		}
		else {
			break;
		}
	}
	arr[ind] = temp;
}
template<typename T>
void heapSort(std::vector<T>& arr) {
	// 构建大顶堆
	for (int i = arr.size() / 2 - 1; i >= 0; i--) {
		// 从最后一个非叶子节点开始调整
		// 判断是否大于它的子节点
		adjustHeap(arr, i, arr.size());
	}
	for (int i = arr.size() - 1; i > 0; i--) {
		std::swap(arr[0], arr[i]);// 把当前最大的放到最后
		adjustHeap(arr, 0, i);// 长度为i,即i之后的元素不再参与调整
	}
}


int main()
{
	//std::vector<int> arr = { 1,8,4,2,6,5,0,9,3,7,2};
	std::vector<float> arr = { 1.2,8.3,4.1,2.2,6,5,0,9,3,7,2 };
	for (auto i : arr) {
		std::cout << i << " ";
	}
	//fastSort(arr);
	heapSort(arr);
	std::cout << std::endl;
	for (auto i : arr) {
		std::cout << i << " ";
	}
	return 0;
}