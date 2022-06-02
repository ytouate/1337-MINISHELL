/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:55:08 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/02 16:13:54 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_export(t_commande *command, t_list *env, char *arg)
{
	int fd;
	fd = open_files(command).fd_out;
	if (fd == -1)
		return ;
	if (arg == NULL)
	{
		sort_list(&env);
		while (env)
		{
			ft_putstr_fd("declare -x\t", fd);
			ft_putendl_fd(env->content, fd);
			env = env->next;
		}
	}
	else
	{
		ft_lstadd_front(&env, ft_lstnew(arg));
		sort_list(&env);
	}
}

bool run_export(t_commande *command, t_list *env_list, t_list *export_list)
{
	char *temp;
	int i = 0;
	if (ft_strcmp(command->flags[0], "export") == 0 || ft_strcmp(command->flags[0], "EXPORT") == 0)
	{
		i = 0;
		if (command->flags[1] == NULL)
			ft_export(command, export_list, NULL);
		else
		{
			while (command->flags[++i])
			{
				temp = ft_split(command->flags[i], '=')[0];
				if (!temp)
					break ;
				else if (is_properly_named(temp) == false)
					printf("export: not an identifier: %s\n", temp);
				else if (ft_getenv(env_list, temp) == NULL)
				{
					if (ft_getenv(export_list, temp) == NULL)
					{
						if (is_variable(command->flags[i]) && command->flags[i])
						{
							ft_export(command, export_list, command->flags[i]);
							ft_export(command, env_list, command->flags[i]);
						}
						else
							ft_export(command, export_list, command->flags[i]);
					}
					else
					{
						if (is_variable(command->flags[i]) && command->flags[i])
						{
							ft_unset(&export_list, temp);
							ft_export(command, export_list, command->flags[i]);
							ft_export(command, env_list, command->flags[i]);
						}
					}
				}
				else
				{
					ft_unset(&export_list, temp);
					ft_unset(&env_list, temp);
					ft_lstadd_back(&export_list, \
					ft_lstnew(ft_strdup(command->flags[i])));
					ft_lstadd_back(&env_list, \
					ft_lstnew(ft_strdup(command->flags[i])));
					sort_list(&export_list);
				}
			}
		}
		return (true);
	}
	return (false);
}