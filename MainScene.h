#pragma once
#include "global.h"

union UIntInByte {
	int i;
	char c[4];
};

struct HeaderChunk {
	UIntInByte type, size, format, track, division;
};

struct TrackChunk {
	UIntInByte type, size;
	char *data;
};

class MainScene {
	HeaderChunk header;
	TrackChunk* track;
	bool IsLittleEndian;
	char* inputFile;
	char* outputFile;

	void MidiRead();
	void Troll(int num);
	void MidiWrite();
	void ConvertEndian(UIntInByte& input, size_t s);
	void Print();
public:
	MainScene();
	void Update();
	void Draw();
};