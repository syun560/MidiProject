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

// デルタタイムをバイナリ可変表現に変換する
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
		printfDx("ファイルのオープンに失敗しました。\n");
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

    printfDx("ヘッダチャンク情報\n"); 
    printfDx("タイプ : %c%c%c%c\n", header.type.c[0], header.type.c[1], header.type.c[2], header.type.c[3]);
    printfDx("サイズ : %d\n", header.size.i);
    printfDx("フォーマット : %hd\n", header.format.i);
    printfDx("トラック数 : %hd\n", header.track.i);
    printfDx("分解能 : %hd\n", header.division.i);


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

    // 読み取ったトラックチャンク情報を出力
    for(int i = 0; i < header.track.i; ++i){   // トラック数だけ繰返し
        printfDx("トラック[%02d]\n", i);
        printfDx("トラック[%d].type : %c%c%c%c\n", i, track[i].type.c[0], track[i].type.c[1], track[i].type.c[2], track[i].type.c[3]);
        printfDx("トラック[%d].size : %d\n", i, track[i].size.i);
        printfDx("トラック[%d].data : \n", i);
        for(int j = 0; j < track[i].size.i; ++j){
            printfDx("%02x ", (unsigned char)track[i].data[j]);   // 16進表記で出力
            if(!((j+1)%10)) printf("\n : ");   // 10バイト出力するたびに改行
        }
        printfDx("\n");
    }
	fclose(fp);
}

// 入力する関数
int MainScene::Write(UIntInByte input, int size) {
	ConvertEndian(input, size);
	fwrite(input.c, size, 1, fp);
	return 0;
}

void MainScene::MidiWrite() {
	if ((fp = fopen(filename, "wb")) == NULL) {
		printfDx("ファイルのオープンに失敗しました。\n");
		return;
	}
	
	//// ヘッダチャンク
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
	//// トラックチャンク（Conductor）
	//UIntInByte ttype, tsize, tempo, beat, beat2;
	//ttype.i = 0x4d54726b; // MTrk
	//tsize.i = 19; // データ長19Byte
	//int tempo = 0x00FF5103; //デルタタイム0にテンポ設定
	//int tempo2 = 0x07A12000; // テンポを120（0x07A120マイクロ秒）に指定、デルタタイム0に
	//int beat = 0xFF580404;
	//int beat2 = 0x02180801; // 最後の1バイトはなんだろう
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

	//// トラックチャンク2
	//type = 0x4d54726b;
	//// サイズはできれば自動計算させたい
	//size = 12;

	//// 関数に入力
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

	//printfDx("%sを保存\n", filename);
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
		printfDx("%sを再生\n", filename);
	}
}

void MainScene::Draw(){
	if (IsLittleEndian) DrawFormatString(180, 0, WHITE, "リトルエンディアンです。");
	else DrawFormatString(180, 0, WHITE, "ビッグエンディアンです。");
	DrawFormatString(180, 100, WHITE, "Oキーで読込");
	DrawFormatString(180, 120, WHITE, "Sキーで保存");
	DrawFormatString(180, 140, WHITE, "Aキーで再生");
}