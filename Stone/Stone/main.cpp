#include "Application.h"
#include "Test.h"
#include "StealRubb.h"
#include "SunEarthMoon.h"

int main(int argc, char* argv[])
{
	Application* app = new SunEarthMoon();
	app->exec();

	return 0;
}