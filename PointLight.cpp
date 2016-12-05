#include "PointLight.h"
#include "Main.h"


PointLight::PointLight()
{
}

PointLight::PointLight(vec3 _pos, vec3 _color, float _range, float _brightness)
{
	Position = _pos;
	Color = _color;
	Range = _range;
	Brightness = _brightness;
}

PointLight::~PointLight()
{
}

void PointLight::Update() {

	Range += sin(Main::TotalTime * 5) / 12;

	/*Position.y += sin(Main::FrameCount / 10.0f) / 140;
	Position.x += cos(Main::FrameCount / 20.0f) / 200;
	Position.z += sin(Main::FrameCount / 20.0f) / 200;*/
}
