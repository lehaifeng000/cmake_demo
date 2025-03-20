
#include<vector>
#include<queue>
#include<iostream>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QGridLayout>
#include <QMainWindow>

// �ڵ�
class Point:public QWidget{
	Q_OBJECT
public:
	// ���캯��
	Point(QWidget* parent, int x, int y) :QWidget(parent), x(x), y(y) {
		setMinimumSize(60, 60);
	}
	Point() {}
	// ��ȡ�ܴ���
	double getCost() const {
		return g + h;
	}
	// ����==��������������constȷ���ڲ������޸�other�����ݣ������constȷ���������ִ��ʱ�����޸ı����Ա����
	bool operator==(const Point& other) const {
		return this->x == other.x && this->y == other.y;
	}

	int x, y;
	bool isVisited = false;
	// g�ﱣ�����㵽��ǰ��ľ��룬h�ﱣ��Ԥ����ǰ�㵽�յ�ľ���
	double g = std::numeric_limits<double>::max(), h = std::numeric_limits<double>::max();
	Point* parent = nullptr;
	bool isPath = false;
	QColor backgroundColor;
protected:
	void mousePressEvent(QMouseEvent* event) override {
		if (event)
		if (event->button() == Qt::LeftButton) {
			backgroundColor = Qt::green;
			update(); // �����ػ�
		}
		QWidget::mousePressEvent(event);
	}
	void paintEvent(QPaintEvent* event) override {
		QPainter painter(this);
		painter.fillRect(rect(), backgroundColor);
		painter.setPen(Qt::black);
		painter.drawRect(rect().adjusted(0, 0, -1, -1)); // ���Ʊ߿�
	}
};

class PointCostComp {
public:
	bool operator()(const Point* a, const Point* b) const {
		return a->getCost() > b->getCost(); // true����a����b���棬����a>bʱ����true
	}
};


class Map :public QWidget {
	Q_OBJECT
public:
	int w, h;
	Point startPoint, endPoint;
	std::vector<std::vector<Point*>> arr;
	QWidget* centralWidget;
	QGridLayout* gridLayout;

	Map( int h, int w, QWidget* parent = nullptr) :QWidget(parent), w(w), h(h) {
		gridLayout = new QGridLayout(this);

		for (int i = 0; i < h; i++) {
			//h��
			std::vector<Point*> line;
			for (int j = 0; j < w; j++) {
				//w��
				Point *p = new Point(this, i, j);
				line.push_back(p);
				gridLayout->addWidget(p, i, j);
			}
			arr.push_back(line);
			setWindowTitle("Grid of Widgets");
			resize(1200, 1200);
		}
	}



	// ���Ƶ㵽�յ��h����,����ʹ�������پ���
	double calH(const Point& p) const {
		return std::abs(p.x - endPoint.x) + std::abs(p.y - endPoint.y);
	}

	Point* searchItem(int x, int y, double g) {
		Point* next = arr[x][y];
		if (next->isVisited)
			return nullptr;
		next->g = g;
		next->h = calH(*next);
		return next;
	}

	double searchPath() {
		// ����startPoint�����ܱ���
		std::priority_queue<Point*, std::vector<Point*>, PointCostComp> searchQueue;
		startPoint.g = 0;
		startPoint.h = calH(startPoint);
		searchQueue.push(&startPoint);
		while (!searchQueue.empty()) {
			Point* t = searchQueue.top();
			if (*t == endPoint) {
				// �������յ�
				std::cout << "search end" << std::endl;
				std::vector<Point*> path;
				Point* it = t;
				path.push_back(it);
				while (it->parent) {
					path.push_back(it->parent);
					it = it->parent;
				}
				// ��ӡ·��
				for (int i = path.size() - 1; i >= 0; i--) {
					std::cout << "(" << path[i]->x << "," << path[i]->y << ")" << std::endl;
					path[i]->isPath = true;
				}
				PrintScreen();
				return t->getCost();
			}
			searchQueue.pop();
			t->isVisited = true; // ����Ϊ�ѷ���
			// ��
			if (t->x > 0) {
				Point* next = searchItem(t->x - 1, t->y, t->g + 1);
				if (next) {
					next->parent = t;
					searchQueue.push(next);
				}
			}
			// ��
			if (t->x + 1 < h) {
				Point* next = searchItem(t->x + 1, t->y, t->g + 1);
				if (next) {
					next->parent = t;
					searchQueue.push(next);
				}
			}
			// ��
			if (t->y > 0) {
				Point* next = searchItem(t->x, t->y - 1, t->g + 1);
				if (next) {
					next->parent = t;
					searchQueue.push(next);
				}
			}
			// ��
			if (t->y + 1 < w) {
				Point* next = searchItem(t->x, t->y + 1, t->g + 1);
				if (next) {
					next->parent = t;
					searchQueue.push(next);
				}
			}
		}
	}

	void setStart(int x, int y) {
		startPoint.x = x;
		startPoint.y = y;
	}

	void setEnd(int x, int y) {
		endPoint.x = x;
		endPoint.y = y;
	}
	// ��ӡ·��
	void PrintScreen() {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (arr[i][j]->isPath) {
					//std::cout << "*";
					std::cout << i << "," << j << std::endl;
				}
				else {
					//std::cout << " ";
				}
			}
			std::cout << std::endl;
		}
	}
protected:
	void paintEvent(QPaintEvent* event) {

	}
};