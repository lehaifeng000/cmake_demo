// cmake_demo.cpp: 定义应用程序的入口点。
//

#include "cmake_demo.h"
#include "a_star.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QWidget>



int main(int argc, char* argv[]) {
   /* Map map(10, 10);
    map.setStart(1, 1);
    map.setEnd(6, 6);
    map.searchPath();*/

    QApplication a(argc, argv);
    QMainWindow m;
    Map *map = new Map(10, 10);
    map->resize(1000, 1000);
    m.setCentralWidget(map);
    m.show();
    
    return a.exec();
}