#include "Application.h"
#include "Bloom.h"
#include "DeferedRender.h"
#include "HDR.h"
#include "NormalMap.h"
#include "ParallaxMap.h"
#include "PBR.h"
#include "SunEarthMoon.h"
#include "StelaRubb.h"

int main(int argc, char* argv[])
{
	Application* app = new SunEarthMoon();
	app->exec();

	return 0;
}