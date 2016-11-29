#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	unsigned int value;
	int i = open("/dev/dice", O_RDONLY);
	int n = read(i, &value, sizeof(int));

	printf("%d\n", value);
	return 0;
}
