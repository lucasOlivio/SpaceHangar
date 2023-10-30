#include "Engine.h"

int main()
{
	Engine engine = Engine();

	bool isInitialized = engine.Initialize("scene");
	if (!isInitialized)
	{
		printf("Exiting with error");
		exit(EXIT_FAILURE);
	}

	engine.Run();

	engine.Exit();

	return 0;
}