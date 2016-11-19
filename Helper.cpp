#include "Helper.h"
#include "Main.h"


Helper::Helper()
{
}


Helper::~Helper()
{
}

int Helper::RandomRange(int max) {
	srand(glfwGetTimerValue());
	int R = rand() % max;
	//cout << R << endl;
	return R;
}
