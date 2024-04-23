#include "test_Node.hpp"
#include "config.h"

#ifdef _WIN32
#include <Windows.h>
#endif

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	testNodeDynamic();

#if STONE_ENGINE_USE_SYSTEM_PAUSE
    system("pause");
#endif

	return 0;
}
