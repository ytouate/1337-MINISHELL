/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 04:54:13 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 14:37:39 by ytouate          ###   ########.fr       */
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
    t_list *env_list;
    t_list *export_list;
    char **env;
    t_commande *command;
}t_vars;

int	ft_strcmp(char *s, char *str);
int check_built_in_commands(t_vars vars);
void	exec_node(t_vars *vars, int fd);
void ft_pipe(t_vars *vars);
void replace_symbol_by_val(char **s, t_list *env_list);
void ft_exit(int exit_code, char flag);
int get_len(t_commande *command);
void	ft_env(t_vars vars);
void	ft_export(t_vars vars, char *arg);
void ft_redirect_output_append_mode(t_vars *vars);
void ft_redirect_output_trunc_mode(t_vars *vars);
int	check_cmd(t_vars *vars);
void	redirect_input(t_vars *vars);
int get_exit_code();
void set_exit_code(int num);
t_list	*ft_getenv(t_list *env_list, char *var_name);
t_list	*ft_getenv(t_list *env_list, char *var_name);
t_list	*get_env_list(char **env);
void	free_2d_array(char **a);
void	ft_echo(char *s, char flag);
void	sig_handler(int sig);
void	ft_pwd(void);
void	sort_list(t_list **env_list);
void	ft_unset(t_list **env_list, char *to_delete);
void	ft_cd(char *path, t_list *env_list);
void	ft_setenv(t_list **env_list, char *var_name, char *var_val);
void	ft_setenv(t_list **env_list, char *var_name, char *var_val);
void	sort_list(t_list **env_list);
char	*get_promt(void);
char	*join_for_echo(t_list *env_list, char **s, char flag);
char	*get_path(t_list *env_list, char *cmd);
char	*get_path(t_list *env_list, char *cmd);
int		get_parts(char	*s, char c);
int		ft_strcmp(char *s, char *str);
char *ft_get_env_val(t_list *env_list, char *var_name);
void	ft_execute(t_vars *vars);
int	is_variable(char *s);
int	check_echo_flag(char *s);
int	is_properly_named(char *s);
void ft_exit(int exit_code, char flag);
int open_output_files(t_commande *command);
int open_input_files(t_commande *command);
int get_len(t_commande *command);
char	*join_for_echo(t_list *env_list, char **s, char flag);
#endif