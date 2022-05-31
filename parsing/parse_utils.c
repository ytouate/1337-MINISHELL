/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:54:26 by ilefhail          #+#    #+#             */
/*   Updated: 2022/05/19 12:05:29 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*convert_to_str(char **av)
{
    int		i;
	char	*av_join;
	char	*temp;

	i = 1;
	av_join = "";
	while (av[i])
	{
		if (i > 1)
		{
			temp = av_join;
			av_join = ft_strjoin(av_join, " ");
			free(temp);
		}
		temp = av_join;
		av_join = ft_strjoin(av_join, av[i]);
		if (i > 1)
			free(temp);
		i++;
	}
	return (av_join);
}
