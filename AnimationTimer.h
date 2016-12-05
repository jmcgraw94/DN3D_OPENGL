#pragma once

class AnimationTimer
{

public:
	AnimationTimer(int _SourceFrames, float _AnimFrameRate = 12);
	~AnimationTimer();

	void Update();

	float AnimationFrameRate;
	float ActiveFrameTimer;
	int CurrentFrame;
	int SourceFrames;
};

