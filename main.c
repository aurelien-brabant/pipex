#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int fd = open("/etc/passwd", O_RDONLY);

	if (fd == -1)
		printf("ERROR\n");
}

