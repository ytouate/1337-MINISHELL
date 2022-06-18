/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 11:42:17 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 14:06:28 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	ft_exit(char *arg, char flag)
{
	int	exit_code;

	if (arg == NULL)
	{
		set_exit_code(0);
		exit(0);
	}
	if (!is_number(arg))
	{
		if (flag != 'e')
			ft_error(arg, ": numberic argument required\n", 255);
	}
	else
	{
		exit_code = ft_atoi(arg);
		if (flag != '\0')
		{
			set_exit_code(exit_code);
			return ;
		}
		printf("exit\n");
		g_global_vars.exit_code = exit_code;
		set_exit_code(exit_code);
		exit(exit_code);
	}
}
