#ifndef MINISHELL_H
# define MINISHELL_H

/*═════════════════════════════ [  LIBS  ] ═══════════════════════════════════*/
//personal libs
# include "libft/inc/libft.h"
//global libs
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <libgen.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

/*═══════════════════════════ [  MACROS  ] ═══════════════════════════════════*/
//errors

# define ERR_CMD "Command not found\n"
# define ERR_PIPE "Pipe error\n"

# define READLINE_MSG "\033[1;36mminishell\033[34m$> \033[0m"

/*═══════════════════════════ [  ENUMS  ] ════════════════════════════════════*/

typedef enum e_token
{
	T_GENERAL,
	T_CMD,       // comando
	T_PIPE,      // | pipe
	T_REDIR_IN,  // < REDIR_IN
	T_INFILE,    // infile
	T_HEREDOC,   // << HEREDOC
	T_LIMITER,   // (str) para terminar de introducir datos
	T_REDIR_OUT, // > REDIR_OUT
	T_OUTFILE,   // outfile
	T_APPEND,    // >> APPEND
	T_SIZE
}					t_token;

/*══════════════════════════ [  STRUCTS  ] ═══════════════════════════════════*/

typedef struct s_lexer
{
	int index;            //  indice del token
	char *data;           //  contenido (valor literal)
	int type;             //  numero equivalente al token (enum)
	struct s_lexer *next; //  siguiente elemento en la lista
}					t_lexer;

typedef struct s_parser
{
	char *cmd;     // comando que será ejecutado
	int redir_in;  // hay que cambiar el name // redireccionamiento de entrada
	int redir_out; // hay que cambiar el name // redireccionamiento de salida
	//char *heredoc; // limitador de entrada
	struct s_parser	*next;
}					t_parser;

// estructura sujeta a cambios
typedef struct s_shell
{
	char			**envp;
	char			**paths;
	char			**cmd_args;
	char			*cmd;
	int				exit_status;
	t_lexer			*lexer;
	t_parser		*parser;
}					t_shell;

/*═════════════════════════ [  FUNCTIONS  ] ══════════════════════════════════*/
//bultins

int					is_builting(t_shell *msh);
void				ft_cd(t_shell *msh);
void				ft_echo(t_shell *msh);
void				ft_pwd(void);
void				ft_env(t_shell *shell);
void				ft_export(t_shell *msh);
void				ft_exit(t_shell *shell);
void				ft_unset(t_shell *msh);

//global utils

int					ft_foundenv(char *var, char **env);

//exec
//signal.c

void				sigint_handler(int sig);

//ft_executer.c
void				ft_executer(t_shell *msh, char **envp);
void				ft_exec_cmd(t_shell *msh);

char				*get_cmd(char **paths, char *cmd);
char				**get_paths(char **envp);

//parser
void				ft_index(t_lexer *lex);
int					ft_count_pipes(t_lexer *lex);
int					get_last(t_lexer *lex, int start);
void				fill_redir(t_lexer *lex, t_parser **cmd_node, int *start,
						int end);
void				fill_cmd(t_lexer *tmp, t_parser **cmd_node);
void				ft_fill_node(t_lexer *lex, t_parser **cmd_node, int start,
						int end);
void				ft_free_parserlist(t_parser **parser);

//lexer_utils.c

int					get_type(char *str, int i);
void				lexer_add_type(t_lexer **ls, int type);
void				ft_add_token(t_lexer **lx, char *input, int i, int size);

//lexer.c

void				ft_lexer(char *input, t_lexer **lx);
void	ft_free_tokenlist(t_lexer **lx); //no va aqui

//parser.c

void				ft_parser(t_parser **parser, t_lexer *lex);
void				ft_free_parserlist(t_parser **parser);

//treat_tokens.c

void				treat_special(char *input, t_lexer **lx, int *i, int type);
int					treat_quotes(char *input, t_lexer **lx, int *i);
void				treat_general(char *input, t_lexer **lx, int *i);

#endif
