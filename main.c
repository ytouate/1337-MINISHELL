/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:17:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/11/26 10:41:20 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniShell.h"

t_vars_g	g_global_vars;

void	sig_handler(int sig)
{
	g_global_vars.sig_type = sig;
	if ((sig == SIGINT || sig == SIGQUIT) && g_global_vars.pid != -1)
	{
		if (!kill(g_global_vars.pid, sig))
			g_global_vars.signal_flag = 1;
	}
	else
	{
		g_global_vars.signal_flag = 0;
		if (sig == SIGINT)
		{
			ft_putchar_fd('\n', STDOUT_FILENO);
			set_exit_code(1);
			rl_on_new_line();
			// rl_replace_line("", 0);
			rl_redisplay();
		}
		else if (sig == SIGQUIT)
		{
			ft_putchar_fd('\r', STDOUT_FILENO);
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

void	set_signals_exit_code(void)
{
	if (g_global_vars.signal_flag == 1)
	{
		if (g_global_vars.sig_type == SIGQUIT)
		{
			ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
			g_global_vars.exit_code = CNTRL_BACKSLASH;
		}
		else
		{
			ft_putchar_fd('\n', 1);
			g_global_vars.exit_code = CNTRL_C;
		}
		g_global_vars.signal_flag = 0;
	}
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
	{
		set_signals_exit_code();
		minishell_routine(vars);
	}
}
