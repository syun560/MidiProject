#define _CRT_SECURE_NO_WARNINGS
#include "MainScene.h"
#include "Input.h"
#include "stdio.h"
#include "stdlib.h"


MainScene::MainScene() {
	filename = "sample.mid";
}

// エンディアン変換
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

// デルタタイムをバイナリ可変表現に変換する
int ConvertDelta(int delta) {
	return 0;
}

// 入力する関数
int MainScene::Write(void *input, int size) {
	convertEndian(input, size);
	fwrite(input, size, 1, fp);
	return 0;
}

void MainScene::MidiWrite() {
	if ((fp = fopen(filename, "wb")) == NULL) {
		printfDx("ファイルのオープンに失敗しました。\n");
		return;
	}
	
	// ヘッダチャンク
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
	
	// トラックチャンク（Conductor）
	int type = 0x4d54726b;
	int size = 0x13; // データ長19Byte
	int tempo = 0x00FF5103; //デルタタイム0にテンポ設定
	int tempo2 = 0x07A12000; // テンポを120（0x07A120マイクロ秒）に指定、デルタタイム0に
	int beat = 0xFF580404;
	int beat2 = 0x02180801; // 最後の1バイトはなんだろう
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

	// トラックチャンク2
	type = 0x4d54726b;
	// サイズはできれば自動計算させたい
	size = 12;

	// 関数に入力
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

	printfDx("%sを保存\n", filename);
}

void MainScene::update() {
	if (Input::Key(KEY_INPUT_S) == 1) {
		MidiWrite();
	}
	if (Input::Key(KEY_INPUT_A) == 1) {
		PlayMusic(filename, DX_PLAYTYPE_BACK);
		printfDx("%sを再生\n", filename);
	}
}

void MainScene::draw(){
	DrawFormatString(180, 120, WHITE, "Sキーで保存");
	DrawFormatString(180, 140, WHITE, "Aキーで再生");
}