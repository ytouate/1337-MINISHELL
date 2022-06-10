/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtInUtils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:41:23 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 21:02:07 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_list	*get_env_list(char **env)
{
	t_list	*env_list;
	int		i;

	i = 0;
	env_list = ft_lstnew(ft_strdup(env[i++]));
	while (env[i])
		ft_lstadd_back(&env_list, ft_lstnew(ft_strdup(env[i++])));
	return (env_list);
}

void	sort_list(t_list **env_list)
{
	t_list	*p;
	t_list	*q;
	char	*val;

	p = *env_list;
	while (p)
	{
		q = *env_list;
		while (q->next)
		{
			if (ft_strcmp(q->content, q->next->content) > 0)
			{
				val = q->content;
				q->content = q->next->content;
				q->next->content = val;
			}
			q = q->next;
		}
		p = p ->next;
	}
}

char	*join_var(char **temp)
{
	int		i;
	char	*result;

	i = 0;
	result = "";
	while (temp[++i])
	{
		result = ft_strjoin(result, temp[i]);
		if (temp[i + 1])
			result = ft_strjoin(result, "=");
	}
	return (result);
}

char	*ft_get_env_val(t_list *env_list, char *var_name)
{
	char	*temp;

	temp = NULL;
	while (env_list)
	{
		temp = ft_split(env_list->content, '=')[0];
		if (!temp || !*temp)
			return (NULL);
		if (ft_strcmp(temp, var_name) == 0)
			return (join_var(ft_split(env_list->content, '=')));
		env_list = env_list ->next;
	}
	return (NULL);
}
