#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

#define ilosc_bitow 20
#define ilosc_osobnikow 300
#define ilosc_krokow 150
#define precyzja 6

int rand_bit(){   //zwraca 0 i 1
	
	return rand() % 2;
}

double fRand(double fMin, double fMax)
{
	
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}



class Osobnik{   
public:
	int dna[ilosc_bitow];    //ciag bitow(22) 1 i 0 
	double fitness; //przystosowanie
	double sector;

	Osobnik(){   //konst. domyslny
		int a = rand() % 2;   //znak            
		
		dna[0] = a;

		for (int i = 1; i < ilosc_bitow; i++){
			dna[i] = rand_bit();
		}

		fitness = 0;
	}
};

vector <Osobnik> wek;  //wektor osobnikow
vector <Osobnik> wek2;

void inicjalizuj2(){   //funkcja inicjalizujaca wektor
	for (int i = 0; i < ilosc_osobnikow; i++){
		Osobnik a;
		wek2.push_back(a);
	}
}

void inicjalizuj(){   //funkcja inicjalizujaca wektor
	for (int i = 0; i < ilosc_osobnikow; i++){
		Osobnik a;
		wek.push_back(a);
	}
}

double bin(int n) {   //binarny w dziesiatkowy
	double sum = 0, sum2 = 0;

	
	for (int i = 1; i < ilosc_bitow; i++){
		sum += wek[n].dna[i] * 1 / pow(2, i);
	}
	if (wek[n].dna[0] == 1) sum *= -1;
	return sum;

}

void ocena_przys(){   //liczenie oceny przystosowania
	double x;
	for (int j = 0; j < ilosc_osobnikow; j++){
		x = bin(j);
		//cout << x << "   ";
		wek[j].fitness = -x + sin(5 * x) - cos(13 * x);  //wzor na przystosowanie
	  // cout << wek[j].fitness << endl;
	}
}


double wypisz_najl(double cos){
	double max = wek[0].fitness;
	int z = 0;
	double lol = bin(0);
	for (int j = 1; j < ilosc_osobnikow; j++){
		if (wek[j].fitness > max){
			max = wek[j].fitness;
			lol = bin(j);
			z = j;
		}
	}
	cout <<   "i= " <<setw(3)<< z << "   ";
	cout << "y = " << fixed << setprecision(precyzja) << max << "     ";
	cout <<"x = "<< lol ;
	cout << "   rozn " << max - cos << endl;

	if (max>cos)return max;
	return cos;
}

bool equal(Osobnik a, Osobnik b)
{
	return a.fitness > b.fitness ? true : false;
}

//funkcja sortuj?ca
void sortwek()
{
	sort(wek.begin(), wek.end(), equal);
}

double sector() {
	double sum = 0;
	for (int i = 0; i <ilosc_osobnikow; i++) {
		wek[i].fitness = wek[i].fitness + 3;
		sum += abs(wek[i].fitness); 
	}
	for (int i = 0; i < ilosc_osobnikow; i++) {
		wek[i].sector = wek[i].fitness / sum; //prawdopodobienstwo
	}
	return sum;
}


void kolo_ruletki()
{

	double sum = sector();
	for (int j = 0; j < ilosc_osobnikow; j++) {
		
		double rrand = (double)rand() / RAND_MAX * (1);

		double tempsum = 0;
		for (int i = 0; i < ilosc_osobnikow; i++) {
			tempsum += wek[i].sector;
			if (tempsum >= rrand) {
				wek2[j] = wek[i];
				break;
			}
		}	
	}
}

void mutacja(){
	int a = rand() % ilosc_bitow;
	int b = rand() % ilosc_osobnikow;
	if(wek[b].dna[a]==0)
		wek[b].dna[a] = 1;
	else wek[b].dna[a] = 0;
}

void krzyzowanie(){
	int punkt1,punkt2;
	int o1, o2;
	vector<Osobnik>temp = wek;
	vector<Osobnik>temp2;
	for (int i = 1; i <= ilosc_osobnikow / 20; i++){
		temp2.push_back(wek[i]);
	}
	for (int i = 1; i < ilosc_osobnikow/4; i++){
		o1 = rand() % (ilosc_osobnikow );
		o2 = rand() % (ilosc_osobnikow /4);
		punkt1 = rand() % ilosc_bitow;
		punkt2 = rand() % ilosc_bitow;
		if (punkt2>punkt1){
			int punkz = punkt1;
			punkt1 = punkt2;
			punkt2 = punkz;
		}
		while (punkt2 == punkt1){
			punkt2 = rand() % ilosc_bitow;
		}
		for (int j = 0; j < punkt1; j++) {
			temp[o1].dna[j] = wek[o1].dna[j];
			wek[o1].dna[j] = wek2[o2].dna[j];
			wek2[o1].dna[j] = temp[o1].dna[j];
	        }
		for (int j = punkt1; j < punkt2; j++) {
			temp[o1].dna[j] = wek[o1].dna[j];
			wek[o1].dna[j] = wek2[o2].dna[j];
			wek2[o1].dna[j] = temp[o1].dna[j];
		}
		for (int j = punkt2; j < ilosc_bitow; j++) {
			temp[o1].dna[j] = wek[o1].dna[j];
			wek[o1].dna[j] = wek2[o2].dna[j];
			wek2[o1].dna[j] = temp[o1].dna[j];
		}
		
	}
	for (int i = ilosc_osobnikow - 1 - ilosc_osobnikow / 20; i < ilosc_osobnikow-1; i++){
		wek2[i] = temp2[i - (ilosc_osobnikow - 1 - ilosc_osobnikow / 20)];
	}
}

void najlepsze_osobniki(){
	vector<Osobnik>temp = wek;
	for (int i = ilosc_osobnikow; i < ilosc_osobnikow * 2; i++){
		temp.push_back(wek2[i-ilosc_osobnikow]);
	}
	for (int z = 0; z < ilosc_osobnikow*0.1; z++){
		Osobnik a;
		temp.push_back(a);
	}
	sort(temp.begin(), temp.end(), equal);
	for (int j = 0; j < ilosc_osobnikow; j++){
		wek2[0] = temp[0];
	}

	
}


void zamiana()
{
	vector<Osobnik>temp = wek;
	wek = wek2;
	wek2 = temp;
}

int main(){
	double max = 0;
	srand(time(0));
	inicjalizuj();
	inicjalizuj2();
	ocena_przys();
	max = wypisz_najl(max);

	

	for (int i = 0; i < ilosc_krokow; i++){
	kolo_ruletki();
	krzyzowanie();

		for (int j = 0; j < ilosc_osobnikow*0.1; j++){
		mutacja();
		}
	najlepsze_osobniki();
	zamiana();
	ocena_przys();
	max = wypisz_najl(max);
	}
	cout << "Maksymalny punkt to: " << max;


	
	system("pause");
	return(0);
}
