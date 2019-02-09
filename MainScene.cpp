#define _CRT_SECURE_NO_WARNINGS
#include "MainScene.h"
#include "Input.h"
#include "stdio.h"
#include "stdlib.h"


MainScene::MainScene() {
	filename = "sample.mid";
}

// �G���f�B�A���ϊ�
int convertEndian(void *input, size_t s) {
	char *temp;
	temp = new char[s];
	for (int i = 0; i < s; i++) {
		temp[i] = ((char *)input)[i];
	}
	for (int i = 1; i <= s; i++) {
		((char *)input)[i - 1] = temp[s - i];
	}
	delete [] temp;
	return 1;
}

// �f���^�^�C�����o�C�i���ϕ\���ɕϊ�����
int ConvertDelta(int delta) {
	return 0;
}

// ���͂���֐�
int MainScene::Write(void *input, int size) {
	convertEndian(input, size);
	fwrite(input, size, 1, fp);
	return 0;
}

void MainScene::MidiWrite() {
	if ((fp = fopen(filename, "wb")) == NULL) {
		printfDx("�t�@�C���̃I�[�v���Ɏ��s���܂����B\n");
		return;
	}
	
	// �w�b�_�`�����N
	int ctype = 0x4d546864;
	int csize = 6;
	short format = 1;
	short tracks = 2;
	short division = 480;
	Write(&ctype, 4);
	Write(&csize, 4);
	Write(&format, 2);
	Write(&tracks, 2);
	Write(&division, 2);
	
	// �g���b�N�`�����N�iConductor�j
	int type = 0x4d54726b;
	int size = 0x13; // �f�[�^��19Byte
	int tempo = 0x00FF5103; //�f���^�^�C��0�Ƀe���|�ݒ�
	int tempo2 = 0x07A12000; // �e���|��120�i0x07A120�}�C�N���b�j�Ɏw��A�f���^�^�C��0��
	int beat = 0xFF580404;
	int beat2 = 0x02180801; // �Ō��1�o�C�g�͂Ȃ񂾂낤
	char eot1 = 0xFF;
	char eot2 = 0x2F;
	char eot3 = 0x00;
	Write(&type, 4);
	Write(&size, 4);
	Write(&tempo, 4);
	Write(&tempo2, 4);
	Write(&beat, 4);
	Write(&beat2, 4);
	Write(&eot1, 1);
	Write(&eot2, 1);
	Write(&eot3, 1);

	// �g���b�N�`�����N2
	type = 0x4d54726b;
	// �T�C�Y�͂ł���Ύ����v�Z��������
	size = 12;

	// �֐��ɓ���
	int ProgramChage = 0x00C000;
	int note = 0x00903064;
	char delta = 0b10000011;
	char delta2 = 0b01100000;
	short noteoff = 0x3000;
	int EOT = 0x01FF2F00;
	Write(&type, 4);
	Write(&size, 4);
	Write(&note, 4);
	Write(&delta, 1);
	Write(&delta2, 1);
	Write(&noteoff, 2);
	Write(&EOT, 4);
	fclose(fp);

	printfDx("%s��ۑ�\n", filename);
}

void MainScene::update() {
	if (Input::Key(KEY_INPUT_S) == 1) {
		MidiWrite();
	}
	if (Input::Key(KEY_INPUT_A) == 1) {
		PlayMusic(filename, DX_PLAYTYPE_BACK);
		printfDx("%s���Đ�\n", filename);
	}
}

void MainScene::draw(){
	DrawFormatString(180, 120, WHITE, "S�L�[�ŕۑ�");
	DrawFormatString(180, 140, WHITE, "A�L�[�ōĐ�");
}