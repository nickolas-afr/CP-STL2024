#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <map>
#include <set>
#include <stack>

using namespace std;

struct Oferta {
	string produs;
	float pret;
	float cantitate;
	int zi;
};

#define dataexp second.first
#define stoc second.second

void solve() {
	map<string, float> produsPret;
	vector<Oferta> oferteA, oferteB;
	stack<string> preparate;
	set<string> setPreparate;
	map < string, map<string, float>> preparatProdCant, preparatProdCant2;
	multimap<string, pair<int, float>> produsExpCantA, produsExpCantB;

	int nrProd, exp;
	string produs;
	float pret, cantitate;
	cin >> nrProd;
	for (int i = 0; i < nrProd; i++) {
		cin >> produs >> pret >> cantitate >> exp;
		produsExpCantA.emplace(make_pair(produs, make_pair(exp, cantitate)));
		produsPret.emplace(make_pair(produs, pret));
	}
	produsExpCantB = produsExpCantA;

	int nrOf, data;
	cin >> nrOf;
	for (int i = 0; i < nrOf; i++) {
		cin >> data >> nrProd;
		Oferta aux;
		for (int j = 0; j < nrProd; j++) {
			cin >> aux.produs >> aux.pret >> aux.cantitate;
			aux.zi = data;
			oferteA.push_back(aux);
		}
	}
	oferteB = oferteA;


	int nrPrep;
	float cantIngr;
	string produs2;
	cin >> nrPrep;
	for (int i = 0; i < nrPrep; i++) {
		cin >> produs >> nrProd;
		map<string, float> prodCant;
		for (int j = 0; j < nrProd; j++) {
			cin >> produs2 >> cantIngr;
			prodCant.emplace(make_pair(produs2, cantIngr));
		}
		preparatProdCant2.emplace(make_pair(produs, prodCant));
		produsExpCantB.emplace(make_pair(produs, make_pair(0, 0)));
		setPreparate.emplace(produs);
		preparate.push(produs);
	}
	while (!preparate.empty()) {
		float auxpret = 0;
		for (auto& ingredient : preparatProdCant2[preparate.top()]) {
			auxpret += produsPret[ingredient.first] * ingredient.second;
			if (setPreparate.count(ingredient.first)) {
				for (auto& prod : preparatProdCant[ingredient.first]) {//ingr first
						preparatProdCant[preparate.top()].emplace(make_pair(prod.first, prod.second * ingredient.second));
				}
			}
			else {
				preparatProdCant[preparate.top()].emplace(make_pair(ingredient.first, ingredient.second));
			}
		}
		produsPret.emplace(make_pair(preparate.top(), auxpret));

		preparate.pop();
	}


	//subpunctul A
	float incasari = 0;
	for (auto& of : oferteA) {						
		auto i = produsExpCantA.find(of.produs);	
		int stocVandut = 0;
		while (i != produsExpCantA.end() && i->first == of.produs && of.cantitate) {
			if (i->dataexp <= of.zi || i->stoc == 0) {
				i++;
			}
			else {
				if (of.pret >= produsPret[of.produs]) {
					if (of.cantitate >= i->stoc) {
						stocVandut += i->stoc;
						incasari += i->stoc * of.pret;
						of.cantitate -= i->stoc;
						i->stoc = 0;
						i++;
					}
					else {
						stocVandut += of.cantitate;
						incasari += of.cantitate * of.pret;
						i->stoc -= of.cantitate;
						of.cantitate = 0;
					}
				}
				else {
					i++;
				}
			}
		}
		if(stocVandut)
			cout << of.produs << " " << stocVandut << "\n";
	}
	cout << incasari << "\n\n\n\n";

	//subpunctul B
	
	/*
	for (auto& i : preparatProdCant2) {
		cout << i.first << " ";
		for (auto& j : i.second) {
			cout << j.first << " " << j.second << " ";
		}
		cout << "\n";
	}
	cout << "\n\n\n";
	for (auto& i : preparatProdCant) {
		cout << i.first << " ";
		for (auto& j : i.second) {
			cout << j.first << " " << j.second << " ";
		}
		cout << "\n";
	}
	cout << "\n\n\n";
	*/

	incasari = 0;
	for (auto& of : oferteB) {
		if (setPreparate.count(of.produs) && produsPret[of.produs] <= of.pret) {
			auto prep = preparatProdCant.find(of.produs);
			int stocVandut = 0;
			while (prep != preparatProdCant.end()) {
				bool canSell = true;
				for (auto& ingr : prep->second) {
					auto i = produsExpCantB.find(ingr.first);
					float auxCantitate = ingr.second,
						auxStoc = i->stoc,
						totalStoc = 0;
					while (i != produsExpCantB.end() && i->first == ingr.first && auxCantitate) {
						if (i->dataexp <= of.zi || auxStoc == 0) {
							i++;
						}
						else {
							if (auxCantitate >= auxStoc) {
								totalStoc += auxStoc;
								auxCantitate -= auxStoc;
								auxStoc = 0;
								i++;
							}
							else {
								totalStoc += auxCantitate;
								auxStoc -= auxCantitate;
								auxCantitate = 0;
							}
						}
					}
					if (totalStoc < ingr.second)
						canSell = false;
				}
				
				if (canSell) {
					for (auto& ingr : prep->second) {
						auto i = produsExpCantB.find(ingr.first);
						float sv = 0;
						while (i != produsExpCantB.end() && i->first == ingr.first && ingr.second) {
							if (i->dataexp <= of.zi || i->stoc == 0) {
								i++;
							}
							else {
								if (ingr.second >= i->stoc) {
									//sv += i->stoc;
									ingr.second -= i->stoc;
									i->stoc = 0;
									i++;
								}
								else {
									//sv += ingr.second;
									i->stoc -= of.cantitate;
									ingr.second = 0;
								}
							}
						}
					}
					stocVandut++;
					incasari += of.pret;
					prep++;
				}
				else {
					prep++;
				}
			}
			cout << of.produs << " " << stocVandut << "\n";
		}
		else {
			auto i = produsExpCantB.find(of.produs);
			int stocVandut = 0;
			while (i != produsExpCantB.end() && i->first == of.produs && of.cantitate) {
				if (i->dataexp <= of.zi || i->stoc == 0) {
					i++;
				}
				else {
					if (of.pret >= produsPret[of.produs]) {
						if (of.cantitate >= i->stoc) {
							stocVandut += i->stoc;
							incasari += i->stoc * of.pret;
							of.cantitate -= i->stoc;
							i->stoc = 0;
							i++;
						}
						else {
							stocVandut += of.cantitate;
							incasari += of.cantitate * of.pret;
							i->stoc -= of.cantitate;
							of.cantitate = 0;
						}
					}
					else {
						i++;
					}
				}
			}
			if (stocVandut)
				cout << of.produs << " " << stocVandut << "\n";
		}
	}
}

int main() {
	solve();
}