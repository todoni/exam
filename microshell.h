#ifndef MICROSHELL_H
# define MICROSHELL_H

# define READ 0
# define WRITE 1

# define ERROR_FATAL "error: fatal\n"
# define ERROR_CD_ARG "error: cd: bad arguments\n"
# define ERROR_CD_PATH "error: cd: cannot change directory to "
# define ERROR_EXEC "error: cannot execute "

typedef struct s_node t_node;

typedef struct s_shell
{
	char	**argv;
	char	**envp;
	t_node	*cmd_list;
	int		cp_stdout;
	int		start;
}	t_shell;

struct s_node
{
	int		type;
	char	**cmd;
	t_node	*next;
};

enum	e_node_type
{
	T_CMD,
	T_PIPE,
	T_SEMI,
	T_END,
};

#endif
