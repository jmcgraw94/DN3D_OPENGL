#include "AnimationTimer.h"
#include "Main.h"
#include "Helper.h"

AnimationTimer::AnimationTimer(int _SourceFrames, float _AnimFrameRate)
{
	SourceFrames = _SourceFrames;
	AnimationFrameRate = _AnimFrameRate;

	CurrentFrame = Helper::RandomRange(SourceFrames);
}

void AnimationTimer::Update() {
	ActiveFrameTimer -= Main::DeltaTime;

	if (ActiveFrameTimer < 0) {
		CurrentFrame++;
		ActiveFrameTimer = (1 / AnimationFrameRate);
		if (CurrentFrame >= SourceFrames)
			CurrentFrame = 0;
	}
}

AnimationTimer::~AnimationTimer()
{
}
