/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 14:11:06 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_echo(char *s, char flag)
{
	if (s == NULL)
	{
		write(1, "\n", 2);
		return ;
	}
	if (flag == 'n')
		write(1, s, ft_strlen(s));
	else
	{
		write(1, s, ft_strlen(s));
		write(1, "\n", 1);
	}
}

int check_built_in_commands(t_vars vars)
{
	char	*temp;
	int		i;

	i = 0;
	if (vars.command->flags[0] != NULL)
	{
		if (ft_strcmp(vars.command->flags[0], "env") == 0)
		{
			ft_env(vars);
			return (true);
		}
		else if (ft_strcmp(vars.command->flags[0], "exit") == 0)
		{
			if (get_len(vars.command) == 1)
				ft_exit(EXIT_SUCCESS, '\0');
			return (true);
		}
		else if (ft_strcmp(vars.command->flags[0], "cd") == 0)
		{
			ft_cd(vars.command->flags[1], vars.env_list);
			return (true);
		}
		else if (ft_strcmp(vars.command->flags[0], "unset") == 0)
		{
			i = 0;
			while (vars.command->flags[++i])
			{
				ft_unset(&vars.env_list, vars.command->flags[i]);
				ft_unset(&vars.export_list, vars.command->flags[i]);
			}
			return (true);
		}
		else if (ft_strcmp(vars.command->flags[0], "export") == 0 ||\
			ft_strcmp(vars.command->flags[0], "EXPORT") == 0)
		{
			i = 0;
			if (vars.command->flags[1] == NULL)
			{
				ft_export(vars, NULL);
			}
			else
			{
				while (vars.command->flags[++i])
				{
					temp = ft_split(vars.command->flags[i], '=')[0];
					if (!temp)
						break ;
					else if (is_properly_named(temp) == false)
						printf("export: not an identifier: %s\n", temp);
					else if (ft_getenv(vars.env_list, temp) == NULL)
					{
						if (ft_getenv(vars.export_list, temp) == NULL)
						{
							if (is_variable(vars.command->flags[i]) && vars.command->flags[i])
							{
								ft_export(vars, vars.command->flags[i]);
								ft_export(vars, vars.command->flags[i]);
							}
							else
								ft_export(vars, vars.command->flags[i]);
						}
						else
						{
							if (is_variable(vars.command->flags[i]) && vars.command->flags[i])
							{
								ft_unset(&vars.export_list, temp);
								ft_export(vars, vars.command->flags[i]);
								ft_export(vars, vars.command->flags[i]);
							}
						}
					}
					else
					{
						ft_unset(&vars.export_list, temp);
						ft_unset(&vars.env_list, temp);
						ft_lstadd_back(&vars.export_list, \
						ft_lstnew(ft_strdup(vars.command->flags[i])));
						ft_lstadd_back(&vars.env_list, \
						ft_lstnew(ft_strdup(vars.command->flags[i])));
						sort_list(&vars.export_list);
					}
				}
			}
			return (true);
		}
		else if (ft_strcmp(vars.command->flags[0], "echo") == 0 ||\
			ft_strcmp(vars.command->flags[0], "ECHO") == 0)
		{
			if (vars.command->flags[1] == NULL)
				ft_echo(NULL, '0');
			else if ((check_echo_flag(vars.command->flags[1])))
				ft_echo(join_for_echo(vars.env_list, vars.command->flags, 'n'), 'n');
			else
				ft_echo(join_for_echo(vars.env_list, vars.command->flags, '\0'), '\0');
			return (true);
		}
		else
			return (false);
	}
	return (false);
}
