/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomUtils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:25:22 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 16:49:48 by ytouate          ###   ########.fr       */
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

void replace_symbol_by_val(char **s, t_list *env_list)
{
	int i;
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

void ft_exit(int exit_code, char flag)
{
	if (flag != 'p')
	{
		printf("exit\n");
		set_exit_code(exit_code);
		exit(exit_code);
	}
}

int	is_properly_named(char *s)
{
	return (ft_isalpha(s[0]) || s[0] == '_');
}

int get_len(t_commande *command)
{
	int count = 0;
	while (command)
	{
		count += 1;
		command = command->next_comande;
	}
	return (count);
}

int open_output_files(t_commande *command)
{
	int fd;
	fd = STDOUT_FILENO;
	while (command->output->first_token != NULL)
	{
		if (command->output->first_token->token == T_OUT)
		{
			fd = open(command->output->first_token->value, O_CREAT | O_RDWR | O_TRUNC , 0644);
			if (fd == -1)
			{
				perror(command->output->first_token->value);
				return (-1);
			}
		}
		else if (command->output->first_token->token == T_APPEND)
		{
			fd = open(command->output->first_token->value, O_CREAT | O_RDWR | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(command->output->first_token->value);
				return (-1);
			}
		}
		command->output->first_token = command->output->first_token->next;
	}
	return (fd);
}

int open_input_files(t_commande *command)
{
	int fd;
	fd = STDIN_FILENO;
	while (command->input->first_token != NULL)
	{
		if (command->input->first_token->token == T_IN)
		{
			fd = open(command->input->first_token->value, O_RDONLY, 0644);
			if (fd == -1)
			{
				perror(command->input->first_token->value);
				return (-1);
			}
		}
		if (command->output )
		command->input->first_token = command->input->first_token->next;
	}
	return (fd);
}
