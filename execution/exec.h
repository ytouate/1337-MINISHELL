/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 04:54:13 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/14 13:21:56 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../parsing/parsing.h"
# include <stdio.h>
# include "../libft/libft.h"
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

typedef struct s_files
{
	int	in;
	int	trunc;
	int	append;
}t_files;

# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127
# define SUCCESS 0
# define CNTRL_C 130
# define CNTRL_BACKSLASH 131

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

t_list		*ft_getenv(t_list *env_list, char *var_name);
t_list		*get_env_list(char **env);
t_contex	open_files(t_token_head redi);

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
bool		run_unset(t_vars vars, t_command *command);
bool		run_cd(t_vars vars, t_command *command);
bool		run_env(t_vars vars, t_command *command);
bool		run_export(t_command *command, t_vars *vars);
bool		run_pwd(t_vars vars, t_command *command);
bool		exec_echo(t_vars vars, t_command *command);

#endif