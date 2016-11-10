#pragma once
#include "..\include\glm\glm.hpp"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace glm;

class Helper
{
public:
	Helper();
	~Helper();

	static float Distance(vec3 V1, vec3 V2) {
		return sqrt(
			(
				pow((V2.x - V1.x), 2) +
				pow((V2.y - V1.y), 2) +
				pow((V2.z - V1.z), 2)
				)
		);
	}

	static void PrintVec3(vec3 v , string name = "") {
		cout << name << " [" << v.x << ',' << v.y << "," << v.z << "]" << endl;
	}

	static void ClearConsole() {
		cout << string(100, '\n');
	}
};

