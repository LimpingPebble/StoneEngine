#include "config.h"
#include "test_Node.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	testNode();
	testNodeDynamic();

#if STONE_ENGINE_USE_SYSTEM_PAUSE
	system("pause");
#endif

	return 0;
}
