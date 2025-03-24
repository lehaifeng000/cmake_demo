#include <iostream>
#include <vector>

template<typename T>
void fastSort(std::vector<T> &arr) {
	// ���������������䣬����ȳ�
	std::vector<std::pair<int, int>> stk;// ʹ��vectorģ��ջ��Ҳ����ֱ����stack
	stk.push_back(std::make_pair(0, arr.size() - 1));
	while (!stk.empty()) {
		// ȡ�����һ������
		std::pair<int, int> p = stk.back();
		stk.pop_back();
		int l = p.first, r = p.second;// ��������Ҷ˵�
		T pivot = arr[r];// ѡȡ��������һ��Ԫ����Ϊ��׼
		int i = l - 1; // i�����Ѿ�ȷ��С�ڻ�׼����������һ��Ԫ��
		for (int j = l; j < r; j++) {
			if (arr[j] < pivot) {
				i++;
				std::swap(arr[i], arr[j]);
			}
		}
		// �ѻ�׼�ŵ���ȷ��λ��
		std::swap(arr[i + 1], arr[r]);
		// ��������������ֱ���ջ
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
	// �����󶥶�
	for (int i = arr.size() / 2 - 1; i >= 0; i--) {
		// �����һ����Ҷ�ӽڵ㿪ʼ����
		// �ж��Ƿ���������ӽڵ�
		adjustHeap(arr, i, arr.size());
	}
	for (int i = arr.size() - 1; i > 0; i--) {
		std::swap(arr[0], arr[i]);// �ѵ�ǰ���ķŵ����
		adjustHeap(arr, 0, i);// ����Ϊi,��i֮���Ԫ�ز��ٲ������
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