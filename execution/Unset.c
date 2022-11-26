/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:45:41 by ytouate           #+#    #+#             */
/*   Updated: 2022/11/26 20:25:37 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

t_list	*delete_head(t_list **env_list, char **cmd, char *to_delete)
{
	t_list	*temp;

	if (ft_strcmp(cmd[0], to_delete) == 0)
	{
		temp = *env_list;
		*env_list = (*env_list)->next;
		free(temp->content);
		free(temp);
		free_2d_array(cmd);
		set_exit_code(EXIT_SUCCESS);
		return (*env_list);
	}
	return (NULL);
}

void	delete_body(t_norm *vars)
{
	vars->temp = vars->second;
	free_2d_array(vars->cmd);
	vars->first->next = vars->second->next;
	free(vars->temp->content);
	free(vars->temp);
}

void	ft_unset(t_list **env_list, char *to_delete)
{
	t_norm	vars;
	t_list	*head;

	if (!*env_list)
		return ;
	vars.first = *env_list;
	vars.cmd = ft_split((*env_list)->content, '=');
	if (vars.cmd[0] == NULL)
		return ;
	head = delete_head(env_list, vars.cmd, to_delete);
	if (head != NULL)
		return ;
	if (vars.first == NULL)
		vars.second = vars.first->next;
	else
	{
		ft_putendl_fd("am here", 2);
		return ;
	}
	while (vars.second)
	{
		free_2d_array(vars.cmd);
		vars.cmd = ft_split(vars.second->content, '=');
		if (ft_strcmp(vars.cmd[0], to_delete) == 0)
		{
			delete_body(&vars);
			return ;
		}
		vars.first = vars.second;
		vars.second = vars.second->next;
	}
	free_2d_array(vars.cmd);
	set_exit_code(EXIT_SUCCESS);
}

bool	run_unset(t_vars *vars, t_command *command)
{
	int	i;

	if (!ft_strcmp(command->flags[0], "unset"))
	{
		i = 0;
		while (command->flags[++i])
		{
			ft_unset(&vars->env_list, command->flags[i]);
			ft_unset(&vars->export_list, command->flags[i]);
		}
		return (true);
	}
	return (false);
}
