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

void	exit_cd_arg(void)
{
	write(STDERR_FILENO, ERROR_CD_ARG, ft_strlen(ERROR_CD_ARG));
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
	exit(EXIT_FAILURE);
}

void	free_array(char **array)
{
	int i = 0;

	while (array[i])
	{
		free(array[i]);
		++i;
	}
	free(array);
}

void	free_all_node(t_node *list)
{
	t_node *temp;
	t_node *cur;

	cur = list;
	while (cur)
	{
		temp = cur;
		free_array(cur->cmd);
		free(temp);
		cur = cur->next;
	}
}

void	get_cmd_list(t_shell *shell)
{
	int	i = shell->start;
	int j = 0;
	t_node	*new;
	char	**argv;
	int		cmd_count = 0;

	argv = shell->argv;
	while (1)
	{
		new = malloc(sizeof(t_node));
		if (!new)
			exit_fatal();
		new->cmd = NULL;
		new->next = NULL;
		new->type = -1;
		cmd_count = 0;
		while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
		{	
			++i;
			++cmd_count;
		}
		new->cmd = malloc((cmd_count + 1) * sizeof(char *));
		if (!new->cmd)
			exit_fatal();
		j = 0;
		while (j < cmd_count)
		{
			new->cmd[j] = ft_strdup(argv[j + shell->start]);
			++j;
		}
		new->cmd[j] = NULL;
		if (argv[i])
		{
			if (strcmp(argv[i], "|") == 0)
				new->type = T_PIPE;
			/*else if (strcmp(argv[i], ";") == 0)
				new->type = T_SEMI;
			else
				new->type = T_CMD;*/
			++i;
		}
		else
			new->type = T_END;
		list_add_back(&shell->cmd_list, new);
		if (new->type != T_PIPE)
			break ;
		shell->start = i;
	}
	shell->start = i;
}

void	execute_cd(char **arg)
{
	int i = 0;

	while (arg[i])
		++i;
	if (i != 2)
	{
		exit_cd_arg();
		return ;
	}
	if (chdir(arg[1]) == -1)
		exit_cd_path(arg[1]);
}

void	execute_other(t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		fd[2];
	t_node	*cur;

	cur = shell->cmd_list;
	while (cur)
	{
		if (cur->type == T_PIPE)
		{
			if (pipe(fd) == -1)
				exit_fatal();
		}
		pid = fork();
		if (pid > 0)
		{
			if (cur->type == T_PIPE)
			{
				if (close(fd[WRITE]) == -1)
					exit_fatal();
				if (dup2(fd[READ], STDIN_FILENO) == -1)
					exit_fatal();
				if (close(fd[READ]) == -1)
					exit_fatal();	
			}
			if (waitpid(pid, &status, 0) == -1)
				exit_fatal();
		}
		else if (pid == 0)
		{
			if (cur->type == T_PIPE)
			{
				if (close(fd[READ]) == -1)
					exit_fatal();
				if (dup2(fd[WRITE], STDOUT_FILENO) == -1)
					exit_fatal();
				if (close(fd[WRITE]) == -1)
					exit_fatal();
			}
			else
			{
				if (dup2(shell->cp_stdout, STDOUT_FILENO) == -1)
					exit_fatal();
			}
			if (execve(cur->cmd[0], cur->cmd, shell->envp) == -1)
				exit_exec(cur->cmd[0]);
		}
		else
			exit_fatal();
		cur = cur->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	
	shell.argv = argv;
	shell.envp = envp;
	shell.start = 1;
	shell.cmd_list = NULL;
	shell.cp_stdout = dup(STDOUT_FILENO);
	if (shell.cp_stdout == -1)
		exit_fatal();
	if (argc < 2)
		return (0);
	while(argv[shell.start])
	{
		if (strcmp(argv[shell.start], ";") == 0)
		{
			++shell.start;
			continue ;
		}
		get_cmd_list(&shell);
		if (strcmp(shell.cmd_list->cmd[0], "cd") == 0)
			execute_cd(shell.cmd_list->cmd);
		else
			execute_other(&shell);
		free_all_node(shell.cmd_list);
		shell.cmd_list = NULL;
	}
	return (0);
}
