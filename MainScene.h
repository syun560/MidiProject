#pragma once
#include "global.h"

union UIntInByte {
	int i;
	unsigned char c[4];
};

class MainScene {
	FILE* fp;
	char* filename;
	int Write(UIntInByte* input, int size);
	void MidiWrite();
public:
	MainScene();
	void update();
	void draw();
};