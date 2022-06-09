/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 04:54:13 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/09 12:30:40 by ytouate          ###   ########.fr       */
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

typedef struct s_vars
{
	t_list		*env_list;
	t_list		*export_list;
	char		**env;
	int			num_of_commands;
	t_command	*command;
}t_vars;


typedef struct s_contex
{
	int	fd_in;
	int	fd_out;
	int herdoc_fildes;
}t_contex;

typedef struct s_norm
{
	t_list	*first;
	t_list	*second;
	t_list	*temp;
	char	**cmd;
	int		*ids;
	int		i;
	int		id;
	int		size;
	int		temp_fd;
	t_contex	contex;
	int		fd[2];
}t_norm;

t_list		*ft_getenv(t_list *env_list, char *var_name);
t_list		*ft_getenv(t_list *env_list, char *var_name);
t_list		*get_env_list(char **env);

t_contex	open_files(t_token_head redi);
bool check_redirection(t_vars *vars, t_command *command);
void		exec_pipe(t_vars *vars);
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
void		ft_export(t_command *command, t_list *env, char *arg);
void		ft_exec(t_vars *vars, t_command *command, t_contex contex);
void		exec_heredoc(t_vars *vars, t_command *command, t_contex contex);

// int			ft_heredoc(t_vars *vars, t_command *command, t_contex contex);
int			check_for_redirection(t_command *command);
int			ft_strcmp(char *s, char *str);
int			get_len(t_command *command);
int			get_exit_code(void);
int			get_parts(char	*s, char c);
int			is_variable(char *s);
int			check_echo_flag(char *s);
int			is_properly_named(char *s);
int			check_built_in_commands(t_vars *vars, t_command *command);

int			ft_heredoc(t_vars *vars, t_command *command, t_contex contex);
char		*get_promt(void);
char		*join_for_echo(t_list *env_list, char **s, char flag);
char		*get_path(t_list *env_list, char *cmd);
char		*get_path(t_list *env_list, char *cmd);
char		*ft_get_env_val(t_list *env_list, char *var_name);

bool		heredoc_outside_pipe(t_vars *vars, t_command *command);
bool		run_exit(t_vars vars, t_command *command);
bool		run_unset(t_vars vars, t_command *command);
bool		run_cd(t_vars vars, t_command *command);
bool		run_env(t_vars vars, t_command *command);
bool		run_export(t_command *command, t_vars *vars);
bool		run_pwd(t_vars vars, t_command *command);
bool		exec_echo(t_vars vars, t_command *command);

#endif