/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:55:08 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 22:27:33 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_export(t_vars vars, t_commande *command, char *arg)
{
	int	fd;

	fd = STDOUT_FILENO;
	fd = open_output_files(command);
	if (arg == NULL)
	{
		sort_list(&vars.env_list);
		while (vars.env_list)
		{
			ft_putstr_fd("declare -x  ", fd);
			ft_putendl_fd(vars.env_list->content, fd);
			vars.env_list = vars.env_list->next;
		}
	}
	else
	{
		ft_lstadd_front(&vars.env_list, ft_lstnew(arg));
		sort_list(&vars.env_list);
	}
}

bool	show_export_list(t_vars vars, t_commande *command)
{
	if (command->flags[1] == NULL)
	{
		ft_export(vars, command, NULL);
		return (true);
	}
	return (false);
}

bool	run_export(t_vars vars, t_commande *command)
{
	int		i;
	char	**temp;

	i = 0;
	if (ft_strcmp(command->flags[0], "export") == 0 || \
		ft_strcmp(command->flags[0], "EXPORT") == 0)
	{
		{
			if (!show_export_list(vars, command))
			{	
				while (command->flags[++i])
				{
					temp = ft_split(command->flags[i], '=');
					if (!is_properly_named(temp[0]))
						printf("export: not an identifier: %s\n", temp[0]);
					else if (!add_unexisted_variable(vars, command, temp, &i))
					{
						add_existed_variable(vars, command, temp, &i);
					}
				}
			}
		}
		return (true);
	}
	return (false);
}

bool	add_unexisted_variable(t_vars vars, t_commande *command, \
	char **var_data, int *i)
{
	if (ft_getenv(vars.env_list, var_data[0]) == NULL)
	{
		if (ft_getenv(vars.export_list, var_data[0]) == NULL)
		{
			if (is_variable(command->flags[*i]) && command->flags[*i])
			{
				ft_export(vars, command, command->flags[*i]);
				ft_export(vars, command, command->flags[*i]);
			}
			else
				ft_export(vars, command, command->flags[*i]);
		}
		else
		{
			if (is_variable(command->flags[*i]) && command->flags[*i])
			{
				ft_unset(&vars.export_list, var_data[0]);
				ft_export(vars, command, command->flags[*i]);
				ft_export(vars, command, command->flags[*i]);
			}
		}
		return (true);
	}
	return (false);
}

bool	add_existed_variable(t_vars vars, t_commande *command, \
	char **var_data, int *i)
{
	ft_unset(&vars.export_list, var_data[0]);
	ft_unset(&vars.env_list, var_data[0]);
	ft_lstadd_back(&vars.export_list, \
	ft_lstnew(ft_strdup(command->flags[*i])));
	ft_lstadd_back(&vars.env_list, \
	ft_lstnew(ft_strdup(command->flags[*i])));
	sort_list(&vars.export_list);
	return (true);
}
