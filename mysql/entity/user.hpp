#pragma once

#include <string>

class MyUser {
public:
	int id;
	std::string name;
	std::string email;
	int age;

	MyUser() = default;
};