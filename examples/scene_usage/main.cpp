#include "test_Node.hpp"
#include "config.h"

#if STONE_ENGINE_OS_NAME == Windows
#include <Windows.h>
#endif

int main() {
#if STONE_ENGINE_OS_NAME == Windows
    SetConsoleOutputCP(CP_UTF8);
#endif

    testNodeDynamic();

#if STONE_ENGINE_USE_SYSTEM_PAUSE
    system("pause");
#endif

    return 0;
}
