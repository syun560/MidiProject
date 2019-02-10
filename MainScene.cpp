#define _CRT_SECURE_NO_WARNINGS
#include "MainScene.h"
#include "Input.h"
#include "stdio.h"
#include "stdlib.h"

MainScene::MainScene() {
	filename = "sample.mid";
	int check = 1;
	if (*(char *)&check) IsLittleEndian = true;
	else IsLittleEndian = false;
}

// �f���^�^�C�����o�C�i���ϕ\���ɕϊ�����
int Delta(int delta) {
	return 0;
}

void MainScene::ConvertEndian(UIntInByte input, size_t s) {
	char *temp;
	temp = new char[s];
	for (int i = 0; i < s; ++i) {
		temp[i] = input.c[i];
	}
	for (int i = 1; i <= s; ++i) {
		input.c[i - 1] = temp[s - i];
	}
	delete[] temp;
}

void MainScene::MidiRead(){
	if ((fp = fopen(filename, "rb")) == NULL) {
		printfDx("�t�@�C���̃I�[�v���Ɏ��s���܂����B\n");
		return;
	}
    fread(&header.type, 4, 1, fp);
    fread(&header.size, 4, 1, fp);
    fread(&header.format, 2, 1, fp);
    fread(&header.track, 2, 1, fp);
    fread(&header.division, 2, 1, fp);

    if (IsLittleEndian) {
		ConvertEndian(header.type, 4);
        ConvertEndian(header.size, 4);
        ConvertEndian(header.format, 2);
        ConvertEndian(header.track, 2);
        ConvertEndian(header.division, 2);
    }

    printfDx("�w�b�_�`�����N���\n"); 
    printfDx("�^�C�v : %c%c%c%c\n", header.type.c[0], header.type.c[1], header.type.c[2], header.type.c[3]);
    printfDx("�T�C�Y : %d\n", header.size.i);
    printfDx("�t�H�[�}�b�g : %hd\n", header.format.i);
    printfDx("�g���b�N�� : %hd\n", header.track.i);
    printfDx("����\ : %hd\n", header.division.i);


	track = new TrackChunk[header.track.i];
    for(int i = 0; i < header.track.i; ++i) {
        fread(&track[i].type, 4, 1, fp);
        fread(&track[i].size.i, 4, 1, fp);
		if (IsLittleEndian) {
			ConvertEndian(track[i].type, 4);
			ConvertEndian(track[i].size, 4);
		}
		track[i].data = new char[track[i].size.i];
        fread(track[i].data, track[i].size.i, sizeof(char), fp);
    }

    // �ǂݎ�����g���b�N�`�����N�����o��
    for(int i = 0; i < header.track.i; ++i){   // �g���b�N�������J�Ԃ�
        printfDx("�g���b�N[%02d]\n", i);
        printfDx("�g���b�N[%d].type : %c%c%c%c\n", i, track[i].type.c[0], track[i].type.c[1], track[i].type.c[2], track[i].type.c[3]);
        printfDx("�g���b�N[%d].size : %d\n", i, track[i].size.i);
        printfDx("�g���b�N[%d].data : \n", i);
        for(int j = 0; j < track[i].size.i; ++j){
            printfDx("%02x ", (unsigned char)track[i].data[j]);   // 16�i�\�L�ŏo��
            if(!((j+1)%10)) printf("\n : ");   // 10�o�C�g�o�͂��邽�тɉ��s
        }
        printfDx("\n");
    }
	fclose(fp);
}

// ���͂���֐�
int MainScene::Write(UIntInByte input, int size) {
	ConvertEndian(input, size);
	fwrite(input.c, size, 1, fp);
	return 0;
}

void MainScene::MidiWrite() {
	if ((fp = fopen(filename, "wb")) == NULL) {
		printfDx("�t�@�C���̃I�[�v���Ɏ��s���܂����B\n");
		return;
	}
	
	//// �w�b�_�`�����N
	//UIntInByte ctype, csize, format, track, division;
	//ctype.i = 0x4d546864; //MThd
	//csize.i = 6;
	//format.i = 1;
	//track.i = 2;
	//division.i = 480;
	//Write(ctype, 4);
	//Write(csize, 4);
	//Write(format, 2);
	//Write(track, 2);
	//Write(division, 2);
	//
	//// �g���b�N�`�����N�iConductor�j
	//UIntInByte ttype, tsize, tempo, beat, beat2;
	//ttype.i = 0x4d54726b; // MTrk
	//tsize.i = 19; // �f�[�^��19Byte
	//int tempo = 0x00FF5103; //�f���^�^�C��0�Ƀe���|�ݒ�
	//int tempo2 = 0x07A12000; // �e���|��120�i0x07A120�}�C�N���b�j�Ɏw��A�f���^�^�C��0��
	//int beat = 0xFF580404;
	//int beat2 = 0x02180801; // �Ō��1�o�C�g�͂Ȃ񂾂낤
	//char eot1 = 0xFF;
	//char eot2 = 0x2F;
	//char eot3 = 0x00;
	//Write(ttype, 4);
	//Write(tsize, 4);
	//Write(&tempo, 4);
	//Write(&tempo2, 4);
	//Write(&beat, 4);
	//Write(&beat2, 4);
	//Write(&eot1, 1);
	//Write(&eot2, 1);
	//Write(&eot3, 1);

	//// �g���b�N�`�����N2
	//type = 0x4d54726b;
	//// �T�C�Y�͂ł���Ύ����v�Z��������
	//size = 12;

	//// �֐��ɓ���
	//int ProgramChage = 0x00C000;
	//int note = 0x00903064;
	//char delta = 0b10000011;
	//char delta2 = 0b01100000;
	//short noteoff = 0x3000;
	//int EOT = 0x01FF2F00;
	//Write(&type, 4);
	//Write(&size, 4);
	//Write(&note, 4);
	//Write(&delta, 1);
	//Write(&delta2, 1);
	//Write(&noteoff, 2);
	//Write(&EOT, 4);
	//fclose(fp);

	//printfDx("%s��ۑ�\n", filename);
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
		PlayMusic(filename, DX_PLAYTYPE_BACK);
		printfDx("%s���Đ�\n", filename);
	}
}

void MainScene::Draw(){
	if (IsLittleEndian) DrawFormatString(180, 0, WHITE, "���g���G���f�B�A���ł��B");
	else DrawFormatString(180, 0, WHITE, "�r�b�O�G���f�B�A���ł��B");
	DrawFormatString(180, 100, WHITE, "O�L�[�œǍ�");
	DrawFormatString(180, 120, WHITE, "S�L�[�ŕۑ�");
	DrawFormatString(180, 140, WHITE, "A�L�[�ōĐ�");
}