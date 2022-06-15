/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniShell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:03:19 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/13 19:30:47 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <signal.h>
# include <stdlib.h>
# include <errno.h>
# include <curses.h>
# include <term.h>
# include <sys/wait.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <sys/stat.h>
# include <limits.h>
# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127
# define SUCCESS 0
# define CNTRL_C 130
# define CNTRL_BACKSLASH 131
# define SYNTAX_ERROR_EXIT 258

typedef enum type
{
	T_WORD,
	T_IN,
	T_OUT,
	T_HERDOC,
	T_APPEND,
	T_PIPE
}	t_type;

typedef struct vars
{
	pid_t	pid;
	int		exit_code;
	int		signal_flag;
}t_vars_g;

typedef struct TOKEN{
	t_type			token;
	char			*value;
	struct TOKEN	*next;
}t_token;

typedef struct t_token_head{
	t_token	*first_token;
}t_token_head;

typedef struct minishellpars{
	char					**flags;
	struct minishellpars	*next_command;
	t_token_head			*redi;
	t_token_head			*herdoc;
}t_command;

typedef struct head
{
	int			taille;
	t_command	*first_c;
}t_head_c;

typedef struct lexer
{
	char	*content;
	char	c;
	size_t	i;
}t_lexer;

typedef struct s_files
{
	int	in;
	int	trunc;
	int	append;
}t_files;

typedef struct s_path_vars{
	char	*path;
	char	*temp;
	char	**command_path;
	int		i;
}t_path_vars;

typedef struct s_cd_vars
{
	char	current_wd[PATH_MAX];
	char	buffer[PATH_MAX];
	char	*temp_path;
	char	**temp;
	t_list	*old_wd;
	int		i;
}t_cd_vars;

typedef struct s_vars
{
	t_list		*env_list;
	t_list		*export_list;
	char		**env;
	t_head_c	*head;
	int			num_of_commands;
	t_command	*command;
}t_vars;

typedef struct s_contex
{
	int		fd_in;
	int		fd_out;
	int		herdoc_fildes;
}t_contex;

typedef struct s_norm
{
	t_list		*first;
	t_list		*second;
	t_list		*temp;
	char		**cmd;
	int			*ids;
	int			i;
	int			id;
	int			size;
	int			temp_fd;
	t_contex	contex;
	int			fd[2];
}t_norm;

extern t_vars_g	g_global_vars;
void	check_export_error(t_vars *vars, t_command *command);
void	check_cd_errors(t_vars *vars, t_command *command);
void	set_exit_code_inside_pipe(t_vars *vars, t_command *command);
t_token		*ft_init_token(int type, char *value);
void		ft_advance(t_lexer	*lexer);
void		ft_skip_spaces(t_lexer	*lexer);
t_lexer		*ft_init_lexer(char *content);
t_token		*ft_get_next_token(t_lexer *lexer, t_list *env_list);
char		*ft_collect_string(t_lexer *lexer, char c, t_list *env_list);
char		*ft_get_value(t_lexer *lexer, t_list *env_list);
t_head_c	*ft_get_for_exec(char *content, t_list *env_list);
void		ft_init_head(t_head_c *head);
void		ft_add_node(t_head_c *head, t_command *commande);
int			ft_add_commande(t_head_c *head, t_lexer *lexer, t_list *env_list);
char		*ft_get_str_without_quote(t_lexer *lexer, t_list *env_list);
void		ft_add_red(t_token_head *head, t_token *t);
char		**ft_replace(char **av, int i, char *value);
int			ft_syntax(char *value, t_token *t, t_head_c *head);
int			ft_rederictions(t_command *re, t_token *token, t_head_c *head);
int			ft_check_pipe(t_lexer *lexer, t_token *token,
				int k, t_head_c *head);
int			ft_check_token(t_token *token, t_command *re,
				int *i, t_head_c *head);
int			ft_fill_node(t_command *re, t_lexer *lexer,
				t_list *env_list, t_head_c *head);
t_token		*ft_red(t_lexer *lexer, t_list *env_list);
t_token		*ft_her_app(t_lexer *lexer, t_list *env_list);
char		*ft_str_for_join(t_lexer *lexer, t_list *env_list);
char		*ft_after_dollar(t_lexer *lexer, t_list *env_list);
int			ft_check_after_dollar(t_lexer *lexer);
char		*ft_get_str(t_lexer *lexer, t_list *env_list);
void		ft_free_all(t_head_c *head);
char		*ft_help_collect_str(t_lexer *lexer, t_list *env_list, char c);
char		*ft_join_and_clean(char *str, char *s);
bool		run_unset(t_vars *vars, t_command *command);
t_list		*ft_getenv(t_list *env_list, char *var_name);
t_list		*get_env_list(char **env);
t_contex	open_files(t_token_head redi);
void		read_for_heredoc(char *line, t_command *command, int fd_in);
void		open_heredoc(t_command **command);
void		set_signal_flag(int num);
void		free_list(t_list *list);
void		ft_error(char *arg, char *msg, int exit_code);
void		wait_for_child(int *ids, int i, int temp_fd);
void		exec_commands_before_heredoc(t_vars *vars);
void		check_cmd(t_command *command, t_vars *vars, t_contex contex);
void		exec_node(t_vars *vars, t_command *command, t_contex contex);
void		ft_execute(t_command *command, t_vars *vars, t_contex contex);
void		ft_pipe(t_vars *vars);
void		replace_symbol_by_val(char **s, t_list *env_list);
void		ft_env(t_vars vars, t_command *command);
void		free_2d_array(char **a);
void		ft_echo(t_command *command, char *s, char flag);
void		sig_handler(int sig);
void		ft_pwd(t_vars vars, t_command *command);
void		sort_list(t_list **env_list);
void		ft_unset(t_list **env_list, char *to_delete);
void		ft_cd(char *path, t_list *env_list);
void		ft_setenv(t_list **env_list, char *var_name, char *var_val);
void		set_exit_code(int num);
void		ft_redirect_output_append_mode(t_command *command, t_vars *vars);
void		ft_redirect_output_trunc_mode(t_vars *vars, t_command *command);
void		redirect_input(t_vars *vars, t_command *command);
void		ft_exit(int exit_code, char *arg, char flag);
void		exec_last_node(t_vars *vars, t_norm data);
void		exec_first_node(t_vars *vars, t_norm data);
void		exec_other_node(t_vars *vars, t_norm data);
void		ft_export(t_command *command, t_list *env, char *arg);
void		exec_first_command_before_heredoc(t_vars *vars, t_norm data);
void		exec_last_command_before_heredoc(t_vars *vars, t_norm data);
void		exec_other_command_before_heredoc(t_vars *vars, t_norm data);
void		add_properly_named_word(t_command *command, t_vars *vars, int i);
void		show_export_error(int *flag, int i, t_command *command);
void		add_existed_variable(t_command *command, t_vars *vars,
				int i, char **temp);
void		add_non_variable(t_command *command,
				t_vars *vars, char **temp, int i);
void		add_unexisted_variable(t_command *command, t_vars *vars,
				char **temp, int i);
void		exec_command(t_command *command, t_vars *vars,
				t_contex contex, char *command_path);
void		set_exit_code_inside_pipe(t_vars *vars, t_command *command);
void		init_contex(t_contex *contex);
int			ft_strcmp(char *s, char *str);
int			get_len(t_command *command);
int			get_exit_code(void);
int			get_parts(char	*s, char c);
int			is_variable(char *s);
int			check_echo_flag(char *s);
int			is_properly_named(char *s);
int			check_built_in_commands(t_vars *vars, t_command *command);
int			ft_heredoc(t_vars *vars, t_command *command, t_contex contex);
int			count_commands_before_heredoc(t_command *command);
int			get_signal_flag(void);
char		*get_promt(void);
char		*join_for_echo(t_list *env_list, char **s, char flag);
char		*get_path(t_list *env_list, char *cmd);
char		*ft_get_env_val(t_list *env_list, char *var_name);
bool		add_variable(t_command *command, t_vars *vars, char **temp, int i);
bool		check_redirection(t_vars *vars, t_command *command);
bool		heredoc_outside_pipe(t_vars *vars, t_command *command);
bool		run_exit(t_vars vars, t_command *command);
bool		run_cd(t_vars vars, t_command *command);
bool		run_env(t_vars vars, t_command *command);
bool		run_export(t_command *command, t_vars *vars);
bool		run_pwd(t_vars vars, t_command *command);
bool		exec_echo(t_vars vars, t_command *command);

#endif