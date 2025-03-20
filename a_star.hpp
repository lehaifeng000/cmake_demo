
#include<vector>
#include<queue>
#include<iostream>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QGridLayout>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QThread>
#include <QTimer>

// 节点
class Point:public QWidget{
	Q_OBJECT
public:
	int x, y;
	bool isVisited = false;
	// g里保存从起点到当前点的距离，h里保存预估当前点到终点的距离
	double g = std::numeric_limits<double>::max(), h = std::numeric_limits<double>::max();
	Point* parent = nullptr;
	//bool isPath = false;
	QColor backgroundColor;

	// 构造函数
	Point(QWidget* parent, int x, int y) :QWidget(parent), x(x), y(y) {
		setMinimumSize(60, 60);
	}
	Point() {}
	// 获取总代价
	double getCost() const {
		return g + h;
	}
	// 重载==操作符，参数的const确保内部不会修改other的内容，后面的const确保这个函数执行时不会修改本身成员变量
	bool operator==(const Point& other) const {
		return this->x == other.x && this->y == other.y;
	}

	void reset() {
		// 重置
		isVisited = false;
		g = std::numeric_limits<double>::max();
		h = std::numeric_limits<double>::max();
		parent = nullptr;
		//isPath = false;
		backgroundColor = Qt::white;
		update();
	}

	void displayPath() {
		backgroundColor = Qt::red;
		update();
	}
signals:
	void set_point(int x, int y);
	
protected:
	void mousePressEvent(QMouseEvent* event) override {
		if (event) {
			if (event->button() == Qt::LeftButton) {
				backgroundColor = Qt::green;
				update(); // 触发重绘
				emit set_point(x, y);
			}
		}
		
		QWidget::mousePressEvent(event);
	}
	void paintEvent(QPaintEvent* event) override {
		QPainter painter(this);
		painter.fillRect(rect(), backgroundColor);
		painter.setPen(Qt::black);
		painter.drawRect(rect().adjusted(0, 0, -1, -1)); // 绘制边框
	}
};

class PointCostComp {
public:
	bool operator()(const Point* a, const Point* b) const {
		return a->getCost() > b->getCost(); // true代表a排在b后面，所以a>b时返回true
	}
};


class Map :public QWidget {
	Q_OBJECT
public:
	int w, h;
	Point *startPoint, *endPoint;
	std::vector<std::vector<Point*>> arr;
	QWidget* centralWidget;
	QGridLayout* gridLayout;
	std::vector<Point*> path;

	int pointClickCount = 0;

	Map( int h, int w, QWidget* parent = nullptr) :QWidget(parent), w(w), h(h) {
		gridLayout = new QGridLayout(this);

		for (int i = 0; i < h; i++) {
			//h行
			std::vector<Point*> line;
			for (int j = 0; j < w; j++) {
				//w列
				Point *p = new Point(this, i, j);
				line.push_back(p);
				gridLayout->addWidget(p, i, j);
				connect(p, &Point::set_point, this, &Map::onSetPoint);
			}
			arr.push_back(line);
			setWindowTitle("Grid of Widgets");
			resize(1200, 1200);
		}
	}



	// 估计点到终点的h距离,这里使用曼哈顿距离
	double calH(const Point& p) const {
		return std::abs(p.x - endPoint->x) + std::abs(p.y - endPoint->y);
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
		// 根据startPoint像四周遍历
		std::priority_queue<Point*, std::vector<Point*>, PointCostComp> searchQueue;
		startPoint->g = 0;
		startPoint->h = calH(*startPoint);
		searchQueue.push(startPoint);
		while (!searchQueue.empty()) {
			Point* t = searchQueue.top();
			searchQueue.pop();
			if (*t == *endPoint) {
				// 遍历到终点
				std::cout << "search end" << std::endl;
				Point* it = t;
				path.push_back(it);
				while (it->parent) {
					path.insert(path.begin(), it->parent);
					it = it->parent;
				}
				// 打印路径
				for (int i = 0; i < path.size() - 1; i++) {
					std::cout << "(" << path[i]->x << "," << path[i]->y << ")" << std::endl;
					//path[i]->isPath = true;
				}
				//PrintScreen();
				return t->getCost();
			}
			t->isVisited = true; // 设置为已访问
			// 上
			if (t->x > 0) {
				Point* next = searchItem(t->x - 1, t->y, t->g + 1);
				if (next) {
					next->parent = t;
					searchQueue.push(next);
				}
			}
			// 下
			if (t->x + 1 < h) {
				Point* next = searchItem(t->x + 1, t->y, t->g + 1);
				if (next) {
					next->parent = t;
					searchQueue.push(next);
				}
			}
			// 左
			if (t->y > 0) {
				Point* next = searchItem(t->x, t->y - 1, t->g + 1);
				if (next) {
					next->parent = t;
					searchQueue.push(next);
				}
			}
			// 右
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
		//startPoint.x = x;
		//startPoint.y = y;
		startPoint = arr[x][y];
	}

	void setEnd(int x, int y) {
		//endPoint.x = x;
		//endPoint.y = y;
		endPoint = arr[x][y];
	}
	//// 打印路径
	//void PrintScreen() {

	//	for (int i = 0; i < h; i++) {
	//		for (int j = 0; j < w; j++) {
	//			if (arr[i][j]->isPath) {
	//				//std::cout << "*";
	//				std::cout << i << "," << j << std::endl;
	//			}
	//			else {
	//				//std::cout << " ";
	//			}
	//		}
	//		std::cout << std::endl;
	//	}
	//}
	// 重置
	void reset() {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				// 重置
				arr[i][j]->reset();
			}
		}
	}
protected:
	void paintEvent(QPaintEvent* event) {

	}
private slots:
	// 处理point被点击
	void onSetPoint(int x, int y) {
		if (pointClickCount == 0) {
			setStart(x, y);
		}
		else if (pointClickCount == 1) {
			setEnd(x, y);
		}
		pointClickCount++;
	}
};

class AStar :public QMainWindow {
	Q_OBJECT
public:
	QVBoxLayout* mainLayout;
	QHBoxLayout* topLayout;
	QPushButton* startButton, *resetButton;
	QWidget* centralWidget;
	Map* map;
	int printIndex = 0;

	AStar(QWidget* parent = nullptr) :QMainWindow(parent) {
		centralWidget = new QWidget(this);
		centralWidget->resize(1000, 1000);
		setCentralWidget(centralWidget);
		mainLayout = new QVBoxLayout(centralWidget);
		// 添加按钮组
		topLayout = new QHBoxLayout();
		startButton = new QPushButton("Start", centralWidget);
		startButton->setFixedWidth(100);
		connect(startButton, &QPushButton::clicked, this, &AStar::onStartButtonClicked);
		topLayout->addWidget(startButton);
		resetButton = new QPushButton("Reset", centralWidget);
		resetButton->setFixedWidth(100);
		connect(resetButton, &QPushButton::clicked, this, &AStar::onResetButtonClicked);
		topLayout->addWidget(resetButton);
		mainLayout->addLayout(topLayout);
		// 添加地图
		map = new Map(10, 10, centralWidget);
		mainLayout->addWidget(map);
		map->resize(800, 800);
		resize(1000, 1000);
		map->reset();

	}

	void updatePathStep() {
		map->path[printIndex++]->displayPath();
	}
	// 展示结果，每隔0.3秒从start开始展示路径
	void printPath() {
		QTimer* timer = new QTimer(this);
		int count = 0;
		connect(timer, &QTimer::timeout, this, [=]() mutable {
			if (count == map->path.size()) {
				timer->stop();  // 停止定时器
				return;
			}
			map->path[count]->displayPath();
			count++;  // 递增计数
			});

		timer->start(300);  // 每 0.3 秒触发一次
		//for (int i = 0; i < map->path.size(); i++) {
		//	
		//	// 等待0.3秒
		//	QThread::msleep(300);
		//}
	}
	// 信号槽
private slots:
	// 开始按钮
	void onStartButtonClicked() {
		map->searchPath();
		printPath();
	}
	// 重置按钮
	void onResetButtonClicked() {
		map->reset();
	}
	
};