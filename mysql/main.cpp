#include <iostream>
//#include <mysql/mysql.h>
#include "my_db.hpp"
#include "entity/user.hpp"
#include <vector>

int main()
{
	MyDB* mydb = MyDB::GetInstance();
	std::vector<MyUser> users = mydb->selectAllUser();
    for (auto user : users) {
		std::cout << user.id << " " << user.name << " " << user.email << " " << user.age << std::endl;
    }

    /*MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;

    conn = mysql_init(NULL);

    if (conn == NULL) {
        fprintf(stderr, "mysql_init failed\n");
        return 1;
    }

    if (mysql_real_connect(conn, "localhost", "root", "lhf123", "demo1", 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn, "SELECT * FROM user")) {
        fprintf(stderr, "mysql_query failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    res = mysql_store_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return 0;*/

}