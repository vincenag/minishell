#include "minishell.h"

//! AQUÍ hay un leak, Agustín
static void	replace_env_variable(char **data, char *prefix, char *sufix,
		char *env_value)
{
	char	*tmp;

	ft_memfree(*data);
	if (env_value)
	{
		tmp = ft_strjoin(prefix, env_value);
		ft_memfree(prefix);
		*data = ft_strjoin(tmp, sufix);
		ft_memfree(tmp);
	}
	else
	{
		*data = ft_strjoin(prefix, sufix);
		ft_memfree(prefix);
	}
	if (sufix[0] != '\0')
		ft_memfree(sufix);
}


static void	process_env_variable(char **data, char **dollar_pos, int *exit_status)
{
	char	*prefix;
	char	*sufix;
	char	*env_value;
	char	*str;
	char	*next_dollar_pos;

	env_value = NULL;
	str = NULL;
	next_dollar_pos = ft_strchr(*dollar_pos + 1, '$');
	prefix = ft_substr(*data, 0, *dollar_pos - *data);
	if (*((*dollar_pos) + 1) != '?') //El rremplazo de $? deberiamos incluirlo al momento de ejecutar el comando
	{
		process_env_substring(dollar_pos, &str, &sufix, &env_value);
		replace_env_variable(data, prefix, sufix, env_value);
	}
	else
	{
		if (*((*dollar_pos) + 2) != '\0')
			sufix = ft_strdup(*dollar_pos + 2);
		else
			sufix = "";
		env_value = ft_itoa(*exit_status);
		replace_env_variable(data, prefix, sufix, env_value);
		ft_memfree(env_value);
	}
	if (next_dollar_pos)
		*dollar_pos = ft_strchr(*data, '$');
	else
		*dollar_pos = NULL;
}

static void	process_quotes(t_lexer *tmp, int *exit_status)
{
	char	*str;
	int		aux;
	char	*dollar_pos;

	aux = 0;
	if (tmp->data[0] == '\"')
	{
		str = ft_strtrim(tmp->data, "\"");
		ft_memfree(tmp->data);
		tmp->data = ft_strdup(str);
		ft_memfree(str);
		aux = 1;
	}
	dollar_pos = ft_strchr(tmp->data, '$');
	while (tmp->data[0] != '\'' && dollar_pos)
		process_env_variable(&(tmp->data), &dollar_pos, exit_status);
	if (aux == 1)
	{
		str = ft_strjoin("\"", tmp->data);
		ft_memfree(tmp->data);
		tmp->data = ft_strjoin(str, "\"");
		ft_memfree(str);
	}
}

void	ft_replace(t_lexer **lexer, int *exit_status)
{
	t_lexer	*tmp;

	if (!lexer)
		return ;
	tmp = *lexer;
	while (tmp)
	{
		if (tmp->type == T_CMD || tmp->type == T_INFILE
			|| tmp->type == T_OUTFILE)
		{
			process_quotes(tmp, exit_status);
		}
		tmp = tmp->next;
	}
	ft_erase_node(lexer);
}
