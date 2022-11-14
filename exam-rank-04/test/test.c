#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
	char *cmd[3] = {"/bin/echo", "HI", NULL};
	int ret = execve("/bin/echo", cmd, envp);
	printf("%d why?\n", ret);
}
