#include "Application.h"
#include "Bloom.h"
#include "Breakout.h"
#include "DeferedRender.h"
#include "HDR.h"
#include "NormalMap.h"
#include "ParallaxMap.h"
#include "PBR.h"
#include "SunEarthMoon.h"
#include "StelaRubb.h"

int main(int argc, char* argv[])
{
	Application* app = new Breakout();
	app->exec();

	return 0;
}