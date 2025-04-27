#include "config.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "ScopEnv.hpp"

int main(int argc, char **argv) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	int retCode;
	{
		std::optional<std::string> path = std::nullopt;
		if (argc >= 2)
			path = std::string(argv[1]);
		retCode = Scop::Env(path).run();
	}

#if STONE_ENGINE_USE_SYSTEM_PAUSE
	system("pause");
#endif

	return retCode;
}
