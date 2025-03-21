
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

// �ڵ�
class Point:public QWidget{
	Q_OBJECT
public:
	int x, y;
	bool isVisited = false;
	bool isWall = false;
	// g�ﱣ�����㵽��ǰ��ľ��룬h�ﱣ��Ԥ����ǰ�㵽�յ�ľ���
	double g = std::numeric_limits<double>::max(), h = std::numeric_limits<double>::max();
	Point* parent = nullptr;
	//bool isPath = false;
	QColor backgroundColor;

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

	void reset() {
		// ����
		isVisited = false;
		isWall = false;
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
				//backgroundColor = Qt::green;
				//update(); // �����ػ�
				emit set_point(x, y);
			}
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
	Point *startPoint, *endPoint;
	std::vector<std::vector<Point*>> arr;
	QWidget* centralWidget;
	QGridLayout* gridLayout;
	std::vector<Point*> path, searchPath;
	bool wallFlag = false;
	int pointClickCount = 0;


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
				connect(p, &Point::set_point, this, &Map::onSetPoint);
			}
			arr.push_back(line);
			setWindowTitle("Grid of Widgets");
			resize(1200, 1200);
		}
	}



	// ���Ƶ㵽�յ��h����,����ʹ�������پ���
	double calH(const Point& p) const {
		return std::abs(p.x - endPoint->x) + std::abs(p.y - endPoint->y);
	}

	Point* searchItem(int x, int y, double g) {
		Point* next = arr[x][y];
		if (next->isVisited || next->isWall)
			return nullptr;
		next->g = g;
		next->h = calH(*next);
		return next;
	}

	double search() {
		// ����startPoint�����ܱ���
		std::priority_queue<Point*, std::vector<Point*>, PointCostComp> searchQueue;
		startPoint->g = 0;
		startPoint->h = calH(*startPoint);
		searchQueue.push(startPoint);
		while (!searchQueue.empty()) {
			Point* t = searchQueue.top();
			searchQueue.pop();
			searchPath.push_back(t);
			if (*t == *endPoint) {
				// �������յ�
				std::cout << "search end" << std::endl;
				Point* it = t;
				path.push_back(it);
				while (it->parent) {
					path.insert(path.begin(), it->parent);
					it = it->parent;
				}
				// ��ӡ·��
				for (int i = 0; i < path.size() - 1; i++) {
					std::cout << "(" << path[i]->x << "," << path[i]->y << ")" << std::endl;
					//path[i]->isPath = true;
				}
				//PrintScreen();
				return t->getCost();
			}
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
		//startPoint.x = x;
		//startPoint.y = y;
		startPoint = arr[x][y];
		startPoint->backgroundColor = Qt::green;
		startPoint->update();
	}

	void setEnd(int x, int y) {
		//endPoint.x = x;
		//endPoint.y = y;
		endPoint = arr[x][y];
		endPoint->backgroundColor = Qt::green;
		endPoint->update();
	}

	// ����
	void reset() {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				// ����
				arr[i][j]->reset();
			}
		}
		path.clear();
		searchPath.clear();
		pointClickCount = 0;
	}
protected:
	void paintEvent(QPaintEvent* event) {

	}
private slots:
	// ����point�����
	void onSetPoint(int x, int y) {
		if (wallFlag) {
			arr[x][y]->backgroundColor = Qt::black;
			arr[x][y]->isWall = true;
			arr[x][y]->update();
		}
		else if (pointClickCount == 0) {
			setStart(x, y);
			pointClickCount++;
		}
		else if (pointClickCount == 1) {
			setEnd(x, y);
			pointClickCount++;
		}
	}
};

class AStar :public QMainWindow {
	Q_OBJECT
public:
	QVBoxLayout* mainLayout;
	QHBoxLayout* topLayout;
	QPushButton* startButton, *resetButton, *wallButton;
	QWidget* centralWidget;
	Map* map;
	int printIndex = 0;

	AStar(QWidget* parent = nullptr) :QMainWindow(parent) {
		centralWidget = new QWidget(this);
		centralWidget->resize(1000, 1000);
		setCentralWidget(centralWidget);
		mainLayout = new QVBoxLayout(centralWidget);
		// ��Ӱ�ť��
		topLayout = new QHBoxLayout();
		startButton = new QPushButton("Start", centralWidget);
		startButton->setFixedWidth(100);
		connect(startButton, &QPushButton::clicked, this, &AStar::onStartButtonClicked);
		topLayout->addWidget(startButton);
		resetButton = new QPushButton("Reset", centralWidget);
		resetButton->setFixedWidth(100);
		connect(resetButton, &QPushButton::clicked, this, &AStar::onResetButtonClicked);
		topLayout->addWidget(resetButton);
		// ���ǽ��ť
		wallButton = new QPushButton("Wall", centralWidget);
		wallButton->setFixedWidth(100);
		topLayout->addWidget(wallButton);
		connect(wallButton, &QPushButton::clicked, this, &AStar::onWallButtonClicked);
		mainLayout->addLayout(topLayout);
		// ��ӵ�ͼ
		map = new Map(10, 10, centralWidget);
		mainLayout->addWidget(map);
		map->resize(800, 800);
		resize(1000, 1000);
		map->reset();

	}

	void updatePathStep() {
		map->path[printIndex++]->displayPath();
	}
	// չʾ���̣�ÿ��0.3���start��ʼչʾ·��
	void printSearchPath() {
		QTimer* timer = new QTimer(this);
		int count = 0;
		connect(timer, &QTimer::timeout, this, [=]() mutable {
			if (count == map->searchPath.size()) {
				timer->stop();  // ֹͣ��ʱ��
				clearSearchPathDisplay();
				printPath();
				return;
			}
			map->searchPath[count]->displayPath();
			count++;  // ��������
			});
		timer->start(300);  // ÿ 0.3 �봥��һ��
	}
	void clearSearchPathDisplay() {
		for (int i = 0; i < map->searchPath.size(); i++) {
			map->searchPath[i]->backgroundColor = Qt::white;
			map->searchPath[i]->update();
		}
	}
	// չʾ�����ÿ��0.3���start��ʼչʾ·��
	void printPath() {
		QTimer* timer = new QTimer(this);
		int count = 0;
		connect(timer, &QTimer::timeout, this, [=]() mutable {
			if (count == map->path.size()) {
				timer->stop();  // ֹͣ��ʱ��
				return;
			}
			map->path[count]->displayPath();
			count++;  // ��������
			});
		timer->start(100);  // ÿ 0.3 �봥��һ��
	}
	// �źŲ�
private slots:
	// ��ʼ��ť
	void onStartButtonClicked() {
		map->search();
		printSearchPath();
	}
	// ���ð�ť
	void onResetButtonClicked() {
		map->reset();
	}
	// ǽ��ť
	void onWallButtonClicked() {
		map->wallFlag = !map->wallFlag;
		if (map->wallFlag) {
			wallButton->setStyleSheet("background-color: red; color: white;");
		}
		else {
			wallButton->setStyleSheet("");
		}
	}
};