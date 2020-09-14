#include <stdio.h>
#include <omp.h>

long long stepCount = 1000000000;
double precisao = 0.000000001;
double pi;

double calcPI_Parallel(long long steps)
{
	long i;
	pi = 0;
	#pragma omp parallel for reduction(+:pi)
	for (i = 0; i < steps; i++)
	{
		pi += 1.0 / (i * 4.0 + 1.0);
		pi -= 1.0 / (i * 4.0 + 3.0);
	}
	pi = pi * 4.0;
}

double calcPI_Sequential(long long steps)
{
	long i;
	pi = 0;
	for (i = 0; i < steps; i++)
	{
		pi += 1.0 / (i * 4.0 + 1.0);
		pi -= 1.0 / (i * 4.0 + 3.0);
	}
	pi = pi * 4.0;
}

long long calcSteps(double precision)
{
	return (4 - precision) / (2 * precision);
}

int main()
{
	printf("Insira o valor da precisao (Insira 0 para padrao: 0.000000001)\n");
	double input = 0;
	scanf("%f", &input);

	double timeP = 0.0;
	double timeS = 0.0;
	double start;
	double end;

	if (input == 0)
	{
		stepCount = calcSteps(precisao);
	}
	else
	{
		stepCount = calcSteps(input);
	}

	stepCount = calcSteps(0.000001);

	for (int i = 0; i < 11; i++)
	{
		start = omp_get_wtime();
		calcPI_Sequential(stepCount);
		end = omp_get_wtime();
		timeS += start - end;
	}
	timeS = timeS / 11;

	for (int i = 0; i < 11; i++)
	{
		start = omp_get_wtime();
		calcPI_Parallel(stepCount);
		end = omp_get_wtime();
		timeP += start - end;
	}
	timeP = timeP / 11;

	printf("Speedup de %f", timeS / timeP);

	return 0;
}
