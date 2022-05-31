/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtIn.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:35:37 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 12:58:10 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
void	ft_pwd(void)
{
	char	working_directory[PATH_MAX];

	getcwd(working_directory, sizeof(working_directory));
	printf("%s\n", working_directory);
}

