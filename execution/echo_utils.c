/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 11:37:39 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 11:41:02 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../MiniShell.h"

char	*join_for_echo(char **s, char flag)
{
	int		i;
	char	*result;
	char	*temp;

	if (flag == 'n')
		i = 2;
	else
		i = 1;
	result = ft_strdup("");
	while (check_echo_flag(s[i]))
		i++;
	while (s[i])
	{
		temp = result;
		result = ft_strjoin(result, s[i]);
		free(temp);
		result = check_for_space(s, result, i);
		i++;
	}
	return (result);
}
