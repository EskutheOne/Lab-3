#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include "winbgi2.h"
#include "nonlin.h"
#include <stdlib.h>

double fun(double x);
void wyczyscbufor();
double newton(double x, double (*pf)(double), double eps, int* i_iter);
double siecz(double x1, double x2, double (*pf)(double), double eps, int* i_iter);
double w = 1.0;

void main()
{
	double a, b, eps, bi, ne, si;
	int iter;
	int* i_iter = &iter;
	FILE* g;

	g = fopen("plik.txt", "w");
	if (!g)
	{
		printf("Wystapil blad przy otwieraniu pliku!");
		exit(1);
	}
	fprintf(g,"Dokladnos\tIteracja-B\tIteracja-N\tIteracja-S\n");

	printf("Podaj dolna granice przedzialu - a \n");
	while(scanf("%lf", &a) != 1)
		{
		printf("Nieprawidlowy format danych, sprobuj jeszcze raz! \n");
		wyczyscbufor();
		}

	printf("Podaj gorna granice przedzialu - b \n");
	while (scanf("%lf", &b) != 1) 
		{
		printf("Nieprawidlowy format danych, sprobuj jeszcze raz! \n");
		wyczyscbufor();
		}
	
	if (a == b)
	{
		printf("To nie jest przedzial! \n");
		exit(1);
	}



	if ((fun(a) * fun(b)) < 0)
	{
		graphics(700, 600);
		log_scale(0.0, 0.0, 30.0, 0.125);
		
		for (int i = -20; i < -2; i++)
		{
			eps = pow(2.0, i);

			bi = bisec(a, b, fun, eps, i_iter);
			printf("Bisec[%d] = %lf \t Ilosc iteracji - %d \t Dokladnosc - %lf  \n\n", i, bi, iter, eps);
			fprintf(g,"%lf\t%d\t", eps, iter);
			setcolor(0.1); //niebieski
			point(iter,eps);

			ne = newton(a, fun, eps, i_iter);
			printf("Newton[%d] = %lf \t Ilosc iteracji - %d \n\n", i,ne, iter);
			fprintf(g, "%d\t", iter);
			setcolor(0.7); //¿ó³ty
			point(iter, eps);

			si = siecz(a, b, fun, eps, i_iter);
			printf("Sieczne[%d] = %lf \t Ilosc iteracji - %d \n\n", i, si, iter);
			fprintf(g, "%d\n", iter);
			setcolor(1); //czerwony
			point(iter,eps);
		}
	}
	else printf("Zadany przedzial nie spelnia f(a)*f(b)<0 \n");
	

	printf("\n\n Rozwiazanie dla funkcji cos(x)=w*x \n\n");
	eps = pow(10.0, -6);
	w = 0.5;

	for (int j = 0; j < 146; j++)
	{
		ne = newton(a, fun, eps, i_iter);
		si = siecz(a, b, fun, eps, i_iter);
		bi = bisec(a, b, fun, eps, i_iter);
		printf("dla w = %.1f \t Newton = %lf \t Sieczne = %lf \t", w, ne, si);
		if (iter == -1) printf(" Zadany przedzial nie spelnia f(a)*f(b)<0 \n");
		else printf(" Bisec = %lf \n", bi);
		w += 0.1;
	}


	fclose(g);
	wait();
}

void wyczyscbufor()      //funkcja czysczaca bufor, potrzebna do obslugi bledow np gdy ktos wpisze znak zamiast liczby

{
	int n;
	while ((n = getchar()) != EOF && n != '\n');
}

double fun(double x)
{
	return cos(x) - w*x;
}

double newton(double x, double (*pf)(double), double eps, int* i_iter)
{

	for (int i = 0; i < 1000; i++)
	{
		x = x - pf(x) / (-sin(x) - w);
		*i_iter = i;
		if (fabs(pf(x)) < eps)	break;
		if (i == 999) 
		{
			printf("Przedzial zbyt duzy dla zakladanej dokladnosci metoda Newtona! "); 
			return 0;
		}
	}
	
	return x;
}

double siecz(double x1, double x2, double (*pf)(double), double eps, int* i_iter)
{
	double x;

	for (int i = 0; i < 1000; i++)
	{
		x = x2 - pf(x2) *(x2-x1)/(pf(x2)-pf(x1));
		x1 = x2;
		x2 = x;
		*i_iter = i;
		if (fabs(pf(x)) < eps)	break;
	}
	
	return x;
}
