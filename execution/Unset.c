/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:45:41 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 12:59:46 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
void	delete_head(t_list **env_list, char **cmd)
{
	t_list	*temp;

	temp = *env_list;
	*env_list = (*env_list)->next;
	free(temp);
	free_2d_array(cmd);
}

void	ft_unset(t_list **env_list, char *to_delete)
{
	t_list	*first;
	t_list	*second;
	t_list	*temp;
	char	**cmd;

	first = *env_list;
	cmd = ft_split((*env_list)->content, '=');
	if (cmd[0] == NULL)
		return ;
	else if (ft_strcmp(cmd[0], to_delete) == 0)
	{
		delete_head(env_list, cmd);
		return ;
	}
	else
	{
		second = first->next;
		while (second)
		{
			cmd = ft_split(second->content, '=');
			if (ft_strcmp(cmd[0], to_delete) == 0)
			{
				temp = second;
				first->next = second->next;
				free(temp);
				free_2d_array(cmd);
				return ;
			}
			free_2d_array(cmd);
			first = second;
			second = second->next;
		}
	}
}