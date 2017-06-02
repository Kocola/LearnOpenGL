#include "Application.h"
#include "Bloom.h"
#include "DeferedRender.h"
#include "HDR.h"
#include "NormalMap.h"
#include "ParallaxMap.h"
#include "SunEarthMoon.h"

int main(int argc, char* argv[])
{
	Application* app = new DenderedRender();
	app->exec();

	return 0;
}