/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomUtils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:25:22 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 20:30:24 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_strcmp(char *s, char *str)
{
	int	i;

	i = 0;
	while (s[i] || str[i])
	{
		if (s[i] != str[i])
			return (s[i] - str[i]);
		i++;
	}
	return (0);
}

char	*get_promt(void)
{
	char	*cmd;

	cmd = readline("Minishell$> :");
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
}

void	replace_symbol_by_val(char **s, t_list *env_list)
{
	int	i;

	i = 0;
	(void)env_list;
	while (s[i])
	{
		if (ft_strcmp(s[i], "~") == 0)
		{
			free(s[i]);
			if (getenv("HOME") != NULL)
				s[i] = ft_strdup(getenv("HOME"));
		}
		i++;
	}
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

int	check_echo_flag(char *s)
{
	int	i;

	i = 0;
	if (s[i++] != '-')
		return (false);
	while (s[i])
	{
		if (s[i] != 'n')
			return (false);
		i += 1;
	}
	return (true);
}

int	is_properly_named(char *s)
{
	return (ft_isalpha(s[0]) || s[0] == '_');
}

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

int open_append_mode_files(t_token_head redi)
{
	int	fd_out;

	if (redi.first_token->token == T_APPEND)
	{
		fd_out = open(redi.first_token->value,
				O_CREAT | O_APPEND | O_RDWR, 0644);
		if (fd_out == -1)
			perror(redi.first_token->value);
		return (fd_out);
	}
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

void	init_contex(t_contex *contex)
{
	contex->fd_in = STDIN_FILENO;
	contex->fd_out = STDOUT_FILENO;
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
