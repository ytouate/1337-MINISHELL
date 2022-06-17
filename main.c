/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:17:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/16 20:44:50 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniShell.h"

t_vars_g	g_global_vars;

void	sig_child_handler(int sig)
{
	if (!kill(g_global_vars.pid, sig))
	{
		if (SIGQUIT == sig)
			set_exit_code(131);
		else
			set_exit_code(130);
	}
	if (sig == SIGQUIT)
		g_global_vars.exit_code = CNTRL_C;
	else
		g_global_vars.exit_code = CNTRL_BACKSLASH;
}

void	sig_handler(int sig)
{
	if ((sig == SIGINT || sig == SIGQUIT) && g_global_vars.pid != -1)
	{
		sig_child_handler(sig);
		set_exit_code(130);
	}
	else
	{
		if (sig == SIGINT)
		{
			ft_putchar_fd('\n', 1);
			set_exit_code(1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else if (sig == SIGQUIT)
		{
			ft_putchar_fd('\r', 1);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	minishell_routine(t_vars *vars)
{
	char	*cmd;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	cmd = get_promt();
	if (cmd == NULL)
	{
		free(cmd);
		exit(EXIT_SUCCESS);
	}
	else if (*cmd)
	{
		vars->head = ft_get_for_exec(cmd, vars->env_list);
		if (vars->head != NULL)
		{
			vars->command = vars->head->first_c;
			vars->num_of_commands = get_len(vars->command);
			if (vars->command != NULL)
				ft_pipe(vars);
			ft_free_all(vars->head);
		}
	}
	free(cmd);
}

int	main(int ac, char **av, char **env)
{
	t_vars	*vars;

	(void)ac;
	(void)av;
	vars = malloc(sizeof(t_vars));
	vars->env = env;
	vars->env_list = get_env_list(vars->env);
	vars->export_list = get_env_list(vars->env);
	g_global_vars.pid = -1;
	g_global_vars.signal_flag = 0;
	g_global_vars.exit_code = 0;
	while (true)
		minishell_routine(vars);
}
