/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 11:42:17 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 11:44:14 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniShell.h"

void	ft_exit(int exit_code, char *arg, char flag)
{
	if (arg != NULL)
	{
		if (!is_number(arg))
		{
			if (flag != 'e')
				printf("exit %s: numeric argument required\n", arg);
			exit_code = 255;
		}
		else
			exit_code = cut_exit_code(arg);
	}
	if (flag == '\0')
	{
		printf("exit\n");
		g_global_vars.exit_code = exit_code;
		exit(exit_code);
	}
	else if (flag == 'e')
		set_exit_code(exit_code);
}
