/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:45:41 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/13 11:21:54 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

bool	delete_head(t_list **env_list, char **cmd, char *to_delete)
{
	t_list	*temp;

	if (ft_strcmp(cmd[0], to_delete) == 0)
	{
		temp = *env_list;
		*env_list = (*env_list)->next;
		free_2d_array(cmd);
		set_exit_code(EXIT_SUCCESS);
		system("leaks minishell");
		return (true);
	}
	return (false);
}

void	ft_unset(t_list **env_list, char *to_delete)
{
	t_norm	vars;

	vars.first = *env_list;
	vars.cmd = ft_split((*env_list)->content, '=');
	if (vars.cmd[0] == NULL)
		return ;
	if (!delete_head(env_list, vars.cmd, to_delete))
	{
		vars.second = vars.first->next;
		while (vars.second)
		{
			free_2d_array(vars.cmd);
			vars.cmd = ft_split(vars.second->content, '=');
			if (ft_strcmp(vars.cmd[0], to_delete) == 0)
			{
				vars.temp = vars.second;
				free_2d_array(vars.cmd);
				vars.first->next = vars.second->next;
				free(vars.temp->content);
				free(vars.temp);
				return ;
			}
			vars.first = vars.second;
			vars.second = vars.second->next;
		}
		free_2d_array(vars.cmd);
		set_exit_code(EXIT_SUCCESS);
	}
}

bool	run_unset(t_vars vars, t_command *command)
{
	int	i;

	if (!ft_strcmp(command->flags[0], "unset"))
	{
		i = 0;
		while (command->flags[++i])
		{
			ft_unset(&vars.env_list, command->flags[i]);
			ft_unset(&vars.export_list, command->flags[i]);
		}
		return (true);
	}
	return (false);
}
