#include "microshell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include <stdio.h>

void	list_add_back(t_node **node, t_node *new)
{
	t_node	*cur;

	if (!(*node))
		*node = new;
	else
	{
		cur = *node;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

int	ft_strlen(char *str)
{
	int	i = 0;

	while (str[i])
		++i;
	return (i);
}

void	exit_fatal(void)
{
	write(STDERR_FILENO, ERROR_FATAL, ft_strlen(ERROR_FATAL));
	exit(1);
}

char	*ft_strdup(char *str)
{
	int i = 0;
	int		size = 0;
	char	*dup;

	size = ft_strlen(str);
	dup = malloc(size + 1);
	if (!dup)
		exit_fatal();
	while (str[i])
	{
		dup[i] = str[i];
		++i;
	}
	dup[i] = '\0';
	return (dup);
}

void	get_cmd_list(t_shell *shell)
{
	int	i = shell->start;
	int j = 0;
	t_node	*tmp;
	char	**argv;
	int		cmd_size = 0;

	argv = shell->argv;
	while (1)
	{
		tmp = malloc(sizeof(t_node));
		if (!tmp)
			exit_fatal();
		tmp->cmd = NULL;
		tmp->next = NULL;
		tmp->is_piped = 0;
		tmp->type = -1;
		cmd_size = 0;
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
		{	
			++i;
			++cmd_size;
		}
		if (cmd_size)
		{
			tmp->cmd = malloc((cmd_size + 1) * sizeof(char *));
			j = 0;
			while (j < cmd_size)
			{
				tmp->cmd[j] = ft_strdup(argv[j + shell->start]);
				++j;
			}
			tmp->cmd[j] = NULL;
			list_add_back(&shell->cmd_list, tmp);
		}
		if (argv[i])
		{
			if (strcmp(argv[i], "|") == 0)
				tmp->type = T_PIPE;
			else if (strcmp(argv[i], ";") == 0)
				tmp->type = T_SEMI;
			else
				tmp->type = T_CMD;
			++i;
		}
		else
			tmp->type = T_END;
		if (tmp->type != T_PIPE)
			break ;
		shell->start = i;
	}
	shell->start = i;
}

void	print(t_node *list)
{
	t_node *cur;
	int		i = 0;

	cur = list;
	while (cur)
	{
		i = 0;
		while (cur->cmd[i])
		{	
			printf("%s ", cur->cmd[i]);
			++i;
		}
		if (cur->type == T_PIPE)
			printf("| ");
		cur = cur->next;
	}
	printf("\n");
}

void	exit_cd_arg(void)
{
	write(STDERR_FILENO, ERROR_CD_ARG, ft_strlen(ERROR_CD_ARG));
	exit (1);
}

void	exit_cd_path(char *path_to_change)
{
	write(STDERR_FILENO, ERROR_CD_PATH, ft_strlen(ERROR_CD_PATH));
	write(STDERR_FILENO, path_to_change, ft_strlen(path_to_change));
	write(STDERR_FILENO, "\n", 1);
}

void	exit_exec(char *executable_that_failed)
{
	write(STDERR_FILENO, ERROR_EXEC, ft_strlen(ERROR_EXEC));
	write(STDERR_FILENO, executable_that_failed, ft_strlen(executable_that_failed));
	write(STDERR_FILENO, "\n", 1);
}

void	execute_cd(char **arg)
{
	int i = 0;

	while (arg[i])
		++i;
	if (i != 2)
		exit_cd_arg();
	if (chdir(arg[1]) == -1)
		exit_cd_path(arg[1]);
}

void	execute_other(t_shell *shell)
{
	char	**cmd;
	pid_t	pid;
	int		status;
	t_node	*cur;

	cur = shell->cmd_list;
	while (cur)
	{
		if (pipe(shell->pipe_fd) == -1)
			exit_fatal();
		cmd = cur->cmd;
		if (cur->type == T_PIPE)
		{	
			pid = fork();
			if (pid > 0)
			{
				if (close(shell->pipe_fd[WRITE]) == -1)
					exit_fatal();
				if (dup2(shell->pipe_fd[READ], STDIN_FILENO) == -1)
					exit_fatal();
				if (close(shell->pipe_fd[READ]) == -1)
					exit_fatal();
				waitpid(pid, &status, 0);
			}
			else if (pid == 0)
			{
				if (close(shell->pipe_fd[READ]) == -1)
					exit_fatal();
				if (dup2(shell->pipe_fd[WRITE], STDOUT_FILENO) == -1)
					exit_fatal();
				if (close(shell->pipe_fd[WRITE]) == -1)
					exit_fatal();
				if (execve(cmd[0], cmd, shell->envp) == -1)
					exit_exec(cmd[0]);
				exit(0);
			}
			else
				exit_fatal();
		}
		else
		{
			if (execve(cmd[0], cmd, shell->envp) == -1)
				exit_exec(cmd[0]);
		}
		cur = cur->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	//pid_t	pid;
	//int		status;
	
	shell.argv = argv;
	shell.envp = envp;
	shell.start = 1;
	shell.cmd_list = NULL;
	if (argc < 2)
		exit_fatal();
	while(argv[shell.start])
	{
		shell.cmd_list = NULL;
		if (strcmp(argv[shell.start], ";") == 0)
		{
			++shell.start;
			continue ;
		}
		get_cmd_list(&shell);
		//print(shell.cmd_list);
		if (strcmp(shell.cmd_list->cmd[0], "cd") == 0)
			execute_cd(shell.cmd_list->cmd);
		else
			execute_other(&shell);
	}
	return (0);
}
