#pragma once
#include "global.h"

class MainScene {
	FILE* fp;
	char* filename;
	int Write(void* input, int size);
	void MidiWrite();
public:
	MainScene();
	void update();
	void draw();
};