#include <iostream>
#include <windows.h>

using namespace std;

struct pozisyon {
	int x, y;
};


class alan {
	static const int yukseklik;
	static const int genislik;
	char ** alanim;
	alan(const alan &);
	alan operator = (const alan &);
public:
	alan() {
		alanim = new char*[alan::yukseklik];
		for (int c = 0; c < alan::yukseklik; ++c) {
			alanim[c] = new char[alan::genislik];
		}
	}
	~alan() {
		for (int c = 0; c < alan::yukseklik; ++c) {
			delete[] alanim[c];
		}
		delete[] alanim;
	}

	void yazdir() {
		for (int c = 0; c < yukseklik; ++c) {
			for (int r = 0; r < genislik; ++r) {
				cout << alanim[c][r];
			}
			cout << endl;
		}
	}

	void temizle() {
		for (int c = 0; c < yukseklik; ++c) {
			for (int r = 0; r < genislik; ++r) {
				alanim[c][r] = ' ';
			}
		}
	}

	int genislikAl() const { return genislik; }
	int yukseklikAl() const { return yukseklik; }


	void ciz(int y, int x, char z) {
		//y = (y < 0) ? 0 : (y >= height ? height - 1 : y);
		//x = (x < 0) ? 0 : (x >= width ? width - 1 : x);

		alanim[y][x] = z;
	}

} alanim;


class yemek {
	pozisyon poz;
	char sembol;
public:
	yemek() : sembol('X'), poz() {
		//poz.x = poz.y = -1;
		//poz.x = poz.y;
	}
	
	void set_pos(int x, int y) {
		poz.x = x;
		poz.y = y;
	}

	void repozisyon(const alan & alanim) {
		poz.x = rand() % alanim.genislikAl();
		poz.y = rand() % alanim.yukseklikAl();
	}

	int get_x() const { return poz.x; }
	int get_y() const { return poz.y; }
	char get_sembol() const { return sembol; }
} yemegi;

class yilan {
	enum { YUKARI, ASAGI, SOL, SAG } yon;
	char sembol, yilan_kafasi;
	pozisyon poz[100];
	pozisyon & kafa;
	int hiz;
	int boyut;
	bool donebilir;
	int puan = 0;
public:
	yilan(int x, int y) :
		sembol('O'), yilan_kafasi('@'), poz(),
		hiz(1), boyut(1), yon(SAG),
		kafa(poz[0]), donebilir(true)
	{
		poz[0].x = x;
		poz[0].y = y;
	}

	bool yemekYE(const yemek & yemekG) {
		if (yemekG.get_x() == kafa.x && yemekG.get_y() == kafa.y) {
			boyut += 1;
			puan += 5;
			return true;
		}
		return false;
	}

	void get_input(const alan & alanim) {
		if (GetAsyncKeyState(VK_UP) && yon != ASAGI) {
			yon = YUKARI;
		}
		if (GetAsyncKeyState(VK_DOWN) && yon != YUKARI) {
			yon = ASAGI;
		}
		if (GetAsyncKeyState(VK_LEFT) && yon != SAG) {
			yon = SOL;
		}
		if (GetAsyncKeyState(VK_RIGHT) && yon != SOL) {
			yon = SAG;
		}
	}

	void haraket(const alan & alanim) {
		pozisyon siradaki = { 0, 0 };
		switch (yon) {
		case YUKARI:
			siradaki.y = -hiz;
			break;
		case ASAGI:
			siradaki.y = hiz;
			break;
		case SOL:
			siradaki.x = -hiz;
			break;
		case SAG:
			siradaki.x = hiz;
		}
		for (int c = boyut - 1; c > 0; --c) {
			poz[c] = poz[c - 1];
		}
		kafa.x += siradaki.x;
		kafa.y += siradaki.y;

		if (kafa.x < 0 || kafa.y < 0 || kafa.x >= alanim.genislikAl() || kafa.y >= alanim.yukseklikAl()) {
			cout << "Puanin:" <<puan << endl;
			throw "OLDUN!!!";
			
		}
	}

	void ciz(alan & alanim) {
		for (int c = 0; c < boyut; ++c) {
			if (c == 0) {
				alanim.ciz(poz[c].y, poz[c].x, yilan_kafasi);
			}
			else {
				alanim.ciz(poz[c].y, poz[c].x, sembol);
			}
		}
	}

	int get_x() const { return kafa.x; }
	int get_y() const { return kafa.y; }
	char get_sembol() const { return sembol; }
} yilanim(1, 1);


const int alan::yukseklik = 25;
const int alan::genislik = 50;


int main() {
	alanim.temizle();

	yemegi.set_pos(5, 5);

	while (1) {
		alanim.temizle();

		yilanim.get_input(alanim);
		try {
			yilanim.haraket(alanim);
		}
		catch (const char * er) {
			alanim.temizle();
			cerr << er << endl;
			system("pause");
			return -1;
		}
		yilanim.ciz(alanim);
		alanim.ciz(yemegi.get_y(), yemegi.get_x(), yemegi.get_sembol());


		if (yilanim.yemekYE(yemegi)) {
			yemegi.repozisyon(alanim);
		}

		alanim.yazdir();

		Sleep(1000/30);
		system("cls");
	}

	return 0;
}