/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 20:50:00 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/15 22:05:53 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

bool	add_variable(t_command *command, t_vars *vars, char **temp, int i)
{
	if (is_variable(command->flags[i]))
	{
		if (ft_getenv(vars->env_list, temp[0]) == NULL)
		{
			if (ft_getenv(vars->export_list, temp[0]) != NULL)
			{
				ft_unset(&vars->export_list, temp[0]);
				add_unexisted_variable(command, vars, temp, i);
			}
			else
				add_unexisted_variable(command, vars, temp, i);
		}
		else
			add_existed_variable(command, vars, i, temp);
		return (true);
	}
	return (false);
}

void	add_non_variable(t_command *command, t_vars *vars, char **temp, int i)
{
	if (ft_getenv(vars->export_list, temp[0]) == NULL)
	{
		ft_lstadd_front(&(vars)->export_list,
			ft_lstnew(ft_strdup(command->flags[i])));
		sort_list(&vars->export_list);
	}
}

void	show_export_error(int *flag, int i, t_command *command)
{
	if (*flag == 0)
	{
		ft_error(command->flags[i], " :export: not an identifier", 1);
		*flag = 1;
	}
}

void	add_properly_named_word(t_command *command, t_vars *vars, int i)
{
	char	**temp;
	int		r;

	temp = ft_split(command->flags[i], '=');
	if (!add_variable(command, vars, temp, i))
		add_non_variable(command, vars, temp, i);
	r = 0;
	while (temp[r])
	{
		free(temp[r]);
		r++;
	}
	free(temp);
}

int	is_variable(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=' && i - 1 >= 0 && s[i - 1] != '=')
			return (true);
		i++;
	}
	return (false);
}
