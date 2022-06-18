/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 20:58:20 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 15:32:04 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

int	open_input_files(t_token_head redi)
{
	int	fd_in;

	if (redi.first_token->token == T_IN)
	{
		fd_in = open(redi.first_token->value, O_RDONLY);
		if (fd_in == -1)
			perror(redi.first_token->value);
		return (fd_in);
	}
	return (INT_MIN);
}

int	open_trunc_mode_files(t_token_head redi)
{
	int	fd_out;

	if (redi.first_token->token == T_OUT)
	{
		fd_out = open(redi.first_token->value, \
			O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (fd_out == -1)
			perror(redi.first_token->value);
		return (fd_out);
	}
	return (INT_MIN);
}

int	open_append_mode_files(t_token_head redi)
{
	int		fd_out;
	char	*file;

	file = ft_strdup(redi.first_token->value);
	if (redi.first_token->token == T_APPEND)
	{
		fd_out = open(file,
				O_CREAT | O_APPEND | O_RDWR, 0644);
		if (fd_out == -1)
			perror(file);
		free(file);
		return (fd_out);
	}
	free(file);
	return (INT_MIN);
}

t_files	init_files(t_token_head redi)
{
	t_files	files;

	files.in = open_input_files(redi);
	files.trunc = open_trunc_mode_files(redi);
	files.append = open_append_mode_files(redi);
	return (files);
}

t_contex	open_files(t_token_head redi)
{
	t_contex	contex;
	t_files		files;

	init_contex(&contex);
	while (redi.first_token)
	{
		files = init_files(redi);
		if (files.in == -1 || files.trunc == -1 || files.append == -1)
		{
			contex.fd_in = -1;
			contex.fd_out = -1;
			return (contex);
		}
		else
		{
			if (files.in != INT_MIN)
				contex.fd_in = files.in;
			if (files.trunc != INT_MIN)
				contex.fd_out = files.trunc;
			if (files.append != INT_MIN)
				contex.fd_out = files.append;
		}
		redi.first_token = redi.first_token->next;
	}
	return (contex);
}
