#include <cstdio>
#include <cstdarg>

int sumTest(int amount, ...)
{
	int sum = 0;

	va_list args;
	va_start(args, amount);
	for (int i = 0; i < amount; ++i)
	{
		sum += va_arg(args, int);
	}
	va_end(args);
	
	return sum;
}

int main()
{
	printf("%d\n", sumTest(5, 1, 2, 3, 4, 5));

	getchar();
	return 0;
}