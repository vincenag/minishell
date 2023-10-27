#include "minishell.h"

// void ft_leaks(void)
// {
// system("leaks -q minishell");
// }

void	ft_getpath(t_shell *shell) //no va aqui
{
	int i;

	i = 0;
	while (shell->envp[i] && ft_strnstr(shell->envp[i], "PATH=", 5) == NULL)
		i++;
	if (i == ft_arraylen(shell->envp))
		shell->paths = 0;
	else
		shell->paths = ft_split(shell->envp[i] + 5, ':');
}

static void	ft_getinput(t_shell *msh)
{
	char	*input;
	char	*tmp;

	signal(SIGINT, sigint_handler); //crear funcion para manejar ctrl+c
	signal(SIGQUIT, SIG_IGN);       // SIG_IGN ignora la señal SIGQUIT (ctrl+\)
	while (1)
	{
		ft_getpath(msh);
		input = readline(READLINE_MSG);
		tmp = ft_strtrim(input, " ");
		if (!input)
			break ;
		add_history(tmp);
		ft_lexer(input, &msh->token_list); // Creacion de tokens
		if (!check_lexer(msh->token_list)) //comprobacion de syntaxis
		{
			printf("syntaxis mal\n"); //test
		}
		else
			printf("syntaxis bien\n"); //test
		//ft_parser(); //no existe aun
		ft_memfree(input);
		ft_memfree(tmp);
		ft_free_tokenlist(&msh->token_list);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	msh;

	// atexit(ft_leaks);
	if (argc != 1 || argv[1])
		return (EXIT_FAILURE);
	//* printf(HEADER); //imprime el header
	msh.envp = ft_arraydup(envp); // se puede mejorar
	ft_getinput(&msh);
	return (EXIT_SUCCESS);
}
