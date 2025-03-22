/*
* 封装mysql数据库操作
* 
*/

#pragma once

#include <mysql/mysql.h>
#include <iostream>
#include "entity/user.hpp"
#include <vector>


class MyDB
{
	// 单例模式
public:
	static MyDB* GetInstance() {
		static std::unique_ptr<MyDB> instance(new MyDB()); // 由于构造函数是私有的，所以不能使用std::make_unique
		return instance.get();
	}


	static std::vector<MyUser> selectAllUser() {
		std::vector<MyUser> users;
		if (!  connect()) {
			// 连接数据库失败
			return users;
		}
		// 返回非0值代表查询失败
		if (mysql_query(conn, "SELECT * FROM user")) {
			fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			conn = nullptr;
			return users;
		}
		MYSQL_RES* res = mysql_store_result(conn);
		// 遍历结果集
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(res)) != NULL) {
			MyUser user;
			user.id = atoi(row[0]);
			user.name = row[1];
			user.email = row[2];
			user.age = atoi(row[3]);
			users.push_back(user);
		}
		mysql_free_result(res);
		closeConn();
		return users;
	}


private:
	MyDB() {};
	MyDB(const MyDB&) = delete; // 禁止拷贝构造函数
	MyDB& operator=(const MyDB&) = delete; // 禁止赋值构造函数
	//static MyDB *instance; // 静态成员变量
	static MYSQL* conn;

	static bool connect() {
		conn = mysql_init(NULL);
		if (conn == NULL) {
			fprintf(stderr, "mysql_init failed\n");
			return false;
		}
		if (mysql_real_connect(conn, "localhost", "root", "lhf123", "demo1", 0, NULL, 0) == NULL) {
			fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
			mysql_close(conn);
			return false;
		}
		return true;
	}
	static void closeConn() {
		if (conn != nullptr) {
			mysql_close(conn);
			conn = nullptr;
		}
	}
};

MYSQL* MyDB::conn = nullptr;