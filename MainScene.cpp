#define _CRT_SECURE_NO_WARNINGS
#include "MainScene.h"
#include "Input.h"
#include "stdio.h"
#include <string.h>

static unsigned char rest[5] = {0x8F, 0x00, 0x80, 0x30, 0x00};

MainScene::MainScene() {
	int check = 1;
	if (*(char *)&check) IsLittleEndian = true;
	else IsLittleEndian = false;
	inputFile = "kaeru.mid";
	outputFile = "output.mid";
}

void MainScene::ConvertEndian(UIntInByte& input, size_t s) {
	char *tmp;
	tmp = new char[s];
	for (int i = 0; i < s; ++i) {
		tmp[i] = input.c[i];
	}
	for (int i = 1; i <= s; ++i) {
		input.c[i - 1] = tmp[s - i];
	}
	delete[] tmp;
}

void MainScene::Print() {
	printfDx("�w�b�_�`�����N���\n");
	printfDx("�^�C�v:%c%c%c%c", header.type.c[0], header.type.c[1], header.type.c[2], header.type.c[3]);
	printfDx(", �T�C�Y:%d", header.size.i);
	printfDx(", �t�H�[�}�b�g:%d", header.format.i);
	printfDx(", �g���b�N��:%d", header.track.i);
	printfDx(", ����\:%d\n", header.division.i);

	for (int i = 0; i < header.track.i; ++i) {
		printfDx("�g���b�N[%02d]", i);
		printfDx(", �^�C�v:%c%c%c%c", track[i].type.c[0], track[i].type.c[1], track[i].type.c[2], track[i].type.c[3]);
		printfDx(", �T�C�Y:%d", track[i].size.i);
		printfDx("\n");
	}
}

void MainScene::MidiRead(){
	FILE* fp;
	if ((fp = fopen(inputFile, "rb")) == NULL) {
		printfDx("�t�@�C���̃I�[�v���Ɏ��s���܂����B\n");
		return;
	}

	// �w�b�_�`�����N�ǂݎ��
    fread(&header.type, 4, 1, fp);
    fread(&header.size, 4, 1, fp);
    fread(&header.format, 2, 1, fp);
    fread(&header.track, 2, 1, fp);
    fread(&header.division, 2, 1, fp);

    if (IsLittleEndian) {
        ConvertEndian(header.size, 4);
        ConvertEndian(header.format, 2);
        ConvertEndian(header.track, 2);
        ConvertEndian(header.division, 2);
    }

	// �g���b�N�`�����N�ǂݎ��
	track = new TrackChunk[16];
    for(int i = 0; i < header.track.i; ++i) {
        fread(&track[i].type, 4, 1, fp);
        fread(&track[i].size, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size, 4);
		}
		track[i].data = new char[track[i].size.i];
        fread(track[i].data, track[i].size.i, sizeof(char), fp);
    }

	Print();
	fclose(fp);
}

void MainScene::Troll(int num){
	if (num < 1) return;
	header.track.i += 1;
	memcpy(&track[2], &track[1], sizeof(TrackChunk));
	int size = 5 * num;
	track[2].size.i += size;
	track[2].data = new char[track[2].size.i];
	for (int i = 0; i < num; i++) {
		memcpy(&track[2].data[i * 5], rest, 5);
	}
	memcpy(&track[2].data[size], track[1].data, track[1].size.i);
	printfDx("�g���b�N���R�s�[���܂���\n");
}

void MainScene::MidiWrite() {
	FILE* fp;
	if ((fp = fopen(outputFile, "wb")) == NULL) {
		printfDx("�t�@�C���̃I�[�v���Ɏ��s���܂����B\n");
		return;
	}
	
	// �w�b�_�`�����N��������
	if (IsLittleEndian) {
		ConvertEndian(header.size, 4);
		ConvertEndian(header.format, 2);
		ConvertEndian(header.track, 2);
		ConvertEndian(header.division, 2);
	}
	fwrite(&header.type, 4, 1, fp);
	fwrite(&header.size, 4, 1, fp);
	fwrite(&header.format, 2, 1, fp);
	fwrite(&header.track, 2, 1, fp);
	fwrite(&header.division, 2, 1, fp);
	if (IsLittleEndian) {
		ConvertEndian(header.size, 4);
		ConvertEndian(header.format, 2);
		ConvertEndian(header.track, 2);
		ConvertEndian(header.division, 2);
	}
	
	// �g���b�N�`�����N��������
	for (int i = 0; i < header.track.i; ++i) {
		fwrite(&track[i].type, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size, 4);
		}
		fwrite(&track[i].size, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].size, 4);
		}
		fwrite(track[i].data, 1, track[i].size.i, fp);
	}
	
	printfDx("%s��ۑ�\n", outputFile);
	fclose(fp);
}

void MainScene::Update() {
	if (Input::Key(KEY_INPUT_O) == 1) {
		MidiRead();
	}
	if (Input::Key(KEY_INPUT_S) == 1) {
		MidiWrite();
	}
	if (Input::Key(KEY_INPUT_A) == 1) {
		PlayMusic("output.mid", DX_PLAYTYPE_BACK);
		printfDx("%s���Đ�\n", outputFile);
	}
	if (Input::Key(KEY_INPUT_C) == 1) {
		clsDx();
	}
	if (Input::Key(KEY_INPUT_T) == 1) {
		Troll(2);
	}
	if (Input::Key(KEY_INPUT_D) == 1) {
		StopMusic();
	}
	if (Input::Key(KEY_INPUT_P) == 1) {
		Print();
	}
}

void MainScene::Draw(){
	if (IsLittleEndian) DrawFormatString(300, 0, WHITE, "���g���G���f�B�A���ł��B");
	else DrawFormatString(300, 0, WHITE, "�r�b�O�G���f�B�A���ł��B");
	static const int INFO_X = 400;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,125);
	DrawBox(INFO_X - 20, 80, FMX - 20, 240, WHITE,TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawFormatString(INFO_X, 80, WHITE, "������@\nO�L�[�œǍ�\nS�L�[�ŕۑ�\nA�L�[�ōĐ�\nD�L�[�Œ�~\nC�L�[�ŃN���A\nT�L�[�ŗ֏�\n\nESC�ŏI��");
}