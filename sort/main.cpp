#include <iostream>
#include <vector>


void fastSort(std::vector<int> &arr) {
	// ���������������䣬����ȳ�
	std::vector<std::pair<int, int>> stk;// ʹ��vectorģ��ջ��Ҳ����ֱ����stack
	stk.push_back(std::make_pair(0, arr.size() - 1));
	while (!stk.empty()) {
		// ȡ�����һ������
		std::pair<int, int> p = stk.back();
		stk.pop_back();
		int l = p.first, r = p.second;// ��������Ҷ˵�
		int pivot = arr[r];// ѡȡ��������һ��Ԫ����Ϊ��׼
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