/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 22:04:35 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 12:59:15 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

bool	is_number(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	run_exit(t_vars vars, t_command *command)
{
	if (!ft_strcmp(command->flags[0], "exit"))
	{
		if (vars.num_of_commands == 1)
		{
			ft_exit(command->flags[1], '\0');
		}
		else
			ft_exit(command->flags[1], 'p');
		return (true);
	}
	return (false);
}

void	check_export_error(t_vars *vars, t_command *command)
{
	int	i;

	(void)vars;
	i = 0;
	while (command->flags[++i])
		if (!is_properly_named(command->flags[i]))
			set_exit_code(1);
}

void	check_cd_errors(t_vars *vars, t_command *command)
{
	if (command->flags[1] == NULL
		|| ft_strcmp("~", command->flags[1]) == 0)
		if (ft_getenv(vars->env_list, "HOME") == NULL)
			set_exit_code(1);
}
