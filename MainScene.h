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

	void MidiRead();
	void MidiWrite();
	void ConvertEndian(UIntInByte& input, size_t s);
public:
	MainScene();
	void Update();
	void Draw();
};