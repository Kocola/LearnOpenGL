#include "Application.h"
#include "HDR.h"
#include "NormalMap.h"
#include "ParallaxMap.h"
#include "SunEarthMoon.h"

int main(int argc, char* argv[])
{
	Application* app = new HDR();
	app->exec();

	return 0;
}