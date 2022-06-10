/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 20:55:52 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 20:56:38 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	get_len(t_command *command)
{
	int	count;

	count = 0;
	while (command)
	{
		count += 1;
		command = command->next_command;
	}
	return (count);
}
