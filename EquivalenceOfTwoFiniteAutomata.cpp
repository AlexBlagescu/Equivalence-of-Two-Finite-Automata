#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
ifstream f("lfa.in");
ofstream g("lfa.out");
struct Automat {
	int nr_stari, nr_litere, st_initiala, nr_st_finale, nr_tranzitii, nr_cuvinte;
	vector < int > stari, stari_finale;
	map < pair < int, char >, vector < int > > T;
	vector < string > cuvinte;
	vector < char > litere;
};
struct Automat_intersectat {
	int nr_stari, nr_litere, nr_st_finale, nr_tranzitii;
	pair <int, int> st_initiala;
	vector < pair < int, int > > stari, stari_finale;
	map < pair < pair < int, int >, char >, pair < int, int > > T;
	vector < char > litere;

};
void citire_automat(Automat& A) {
	
	f >> A.nr_stari;
	A.stari.push_back(0);
	for (int i = 0; i < A.nr_stari; i++) {
		int x;
		f >> x;
		A.stari.push_back(x);
	}
	A.nr_stari++;
	f >> A.nr_litere;
	for (int i = 0; i < A.nr_litere; i++) {
		char c;
		f >> c;
		A.litere.push_back(c);
	}
	f >> A.st_initiala;
	f >> A.nr_st_finale;
	for (int i = 0; i < A.nr_st_finale; i++) {
		int x;
		f >> x;
		A.stari_finale.push_back(x);
	}
	f >> A.nr_tranzitii;
	for (int i = 0; i < A.nr_tranzitii; i++) {
		int x, y;
		char c;
		f >> x >> c >> y;
		A.T[make_pair(x, c)].push_back(y);
	}
	for (auto i : A.stari) {
		for (auto j : A.litere) {
			if (A.T[make_pair(i, j)].empty()) {
				A.T[make_pair(i, j)].push_back(0);
			}
		}
	}
	f >> A.nr_cuvinte;
	for (int i = 0; i < A.nr_cuvinte; i++) {
		string s;
		f >> s;
		A.cuvinte.push_back(s);
	}
}
void afisare_automat(Automat A) {

	g << A.nr_stari << '\n';
	for (int i = 0; i < A.nr_stari; i++) {
		g << A.stari[i] << ' ';
	}
	g << '\n' << A.nr_litere << '\n';
	for (int i = 0; i < A.nr_litere; i++) {
		g << A.litere[i] << ' ';
	}
	g << '\n' << A.st_initiala << '\n' << A.nr_st_finale << '\n';
	for (int i = 0; i < A.nr_st_finale; i++) {
		g << A.stari_finale[i] << ' ';
	}
	g << '\n' << A.nr_stari*A.nr_litere << '\n';
	for (auto i : A.T) {
		g << i.first.first << ' ' << i.first.second << ' ';
		for (auto j : i.second) {
			g << j << ' ';
		}
		g << '\n';
	}
}
void complementare_automat(Automat& A) {

	vector < int > Aux;
	for (int i = 0; i < A.nr_stari; i++) {
		bool ok = 1;
		int stare_curenta = A.stari[i];
		for (int j = 0; j < A.nr_st_finale; j++) {
			if (stare_curenta == A.stari_finale[j]) {
				ok = 0;
			}
		}
		if (ok == 1) {
			Aux.push_back(stare_curenta);
		}
	}
	A.stari_finale = Aux;
	A.nr_st_finale = A.nr_stari - A.nr_st_finale;
}
void intersectie_automate(Automat_intersectat& R, Automat A, Automat B) {

	R.nr_stari = A.nr_stari * B.nr_stari;
	R.nr_litere = A.nr_litere;
	for (int i = 0; i < B.nr_litere; i++) {
		char litera_curenta = B.litere[i];
		bool ok = 1;
		for (int j = 0; j < A.nr_litere; j++) {
			if (litera_curenta == A.litere[j]) {
				ok = 0;
			}
		}
		if (ok == 1) {
			R.nr_litere++;
		}
	}
	R.nr_st_finale = A.nr_st_finale * B.nr_st_finale;
	R.st_initiala.first = A.st_initiala;
	R.st_initiala.second = B.st_initiala;
	for (int i = 0; i < A.nr_stari; i++) {
		for (int j = 0; j < B.nr_stari; j++) {
			R.stari.push_back(make_pair(A.stari[i], B.stari[j]));
		}
	}
	for (int i = 0; i < A.nr_st_finale; i++) {
		for (int j = 0; j < B.nr_st_finale; j++) {
			R.stari_finale.push_back(make_pair(A.stari_finale[i], B.stari_finale[j]));
		}
	}
	R.litere = A.litere;
	for (int i = 0; i < B.nr_litere; i++) {
		char litera_curenta = B.litere[i];
		bool ok = 1;
		for (int j = 0; j < A.nr_litere; j++) {
			if (litera_curenta == A.litere[j]) {
				ok = 0;
			}
		}
		if (ok == 1)	R.litere.push_back(litera_curenta);
	}
	for (auto i : A.stari) {
		for (auto j : A.litere) {
			if (A.T[make_pair(i, j)].empty()) {
				A.T[make_pair(i, j)].push_back(0);
			}
		}
	}
	for (auto i : B.stari) {
		for (auto j : B.litere) {
			if (B.T[make_pair(i, j)].empty()) {
				B.T[make_pair(i, j)].push_back(0);
			}
		}
	}
	for (auto x : A.stari) {
		for (auto y : B.stari) {
			for (auto e : B.litere) {
				vector< char > ::iterator it = find(A.litere.begin(), A.litere.end(), e);
				if (it == A.litere.end())	continue;
				else {
					if (A.T[make_pair(x, e)].front()==0 || B.T[make_pair(y, e)].front()==0)	
						continue;
					R.T[make_pair(make_pair(x, y),e)] = make_pair(A.T[make_pair(x, e)].front(), B.T[make_pair(y, e)].front());
					R.nr_tranzitii++;
				}
			}
		}
	}
	for (auto x : A.stari) {
		for (auto y : B.stari) {
			for (auto e : A.litere) {
				vector< char > ::iterator it = find(B.litere.begin(), B.litere.end(), e);
				if (it == B.litere.end())	continue;
				else {
					if (A.T[make_pair(x, e)].front()==0 || B.T[make_pair(y, e)].front()==0)	continue;
					R.T[make_pair(make_pair(x, y), e)] = make_pair(A.T[make_pair(x, e)].front(), B.T[make_pair(y, e)].front());
					R.nr_tranzitii++;
				}
			}
		}
	}
}
void recunoastere_limbaj_vid(Automat_intersectat A, pair < int, int > stare, vector < pair <int, int > > viz, int& ok) {

	if (ok == 0)	return;
	viz.push_back(stare);
	vector< pair < int, int > >::iterator it = find(A.stari_finale.begin(), A.stari_finale.end(), stare);
	if (it != A.stari_finale.end()) {
		ok = 0;
	}
	else {
		for (auto i : A.T) {
			if (stare == i.first.first) {
				vector < pair < int, int > >::iterator s = find(viz.begin(), viz.end(), i.second);
				if (s == viz.end())	recunoastere_limbaj_vid(A, i.second, viz, ok);
			}

		}
	}
}
int main()
{
	Automat X, Y, cX, cY;
	Automat_intersectat cXY, XcY;
	int ok1, ok2;
	ok1 = ok2 = 1;
	vector < pair < int, int > > viz;

	citire_automat(X);
	citire_automat(Y);
	cX = X;
	cY = Y;
	complementare_automat(cX);
	complementare_automat(cY);
	intersectie_automate(cXY, cX, Y);
	intersectie_automate(XcY, X, cY);
	recunoastere_limbaj_vid(cXY, cXY.st_initiala, viz, ok1);
	viz.clear();
	recunoastere_limbaj_vid(XcY, XcY.st_initiala, viz, ok2);
	if (ok1 == 1 && ok2 == 1) {
		cout << "AUTOMATELE RECUNOSC ACELASI LIMBAJ!";
	}
	else {
		cout << "AUTOMATELE NU RECUNOSC ACELASI LIMBAJ!";
	}
	return 0;
}
