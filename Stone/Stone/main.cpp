#include "Application.h"
#include "NormalMap.h"
#include "Test.h"
#include "StealRubb.h"
#include "SunEarthMoon.h"

int main(int argc, char* argv[])
{
	Application* app = new NormalMap();
	app->exec();

	return 0;
}