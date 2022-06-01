/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:06:57 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 14:36:45 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
extern int exit_code;
int exit_code = 0;
void set_exit_code(int num)
{
	while (num > 255)
		num -= 256;
	exit_code = num;
}

int get_exit_code()
{
	return (exit_code);
}

void	exec_node(t_vars *vars, t_commande *command, t_contex contex)
{
	int		i;

	i = 0;
	if (check_built_in_commands(*vars, command) == false)
	{
		if (command->input->first_token != NULL)
		{
			if (command->input->first_token->token == T_HERDOC)
            {
                ;
				// ft_herdoc(vars, fd);
            }
			else
			{
				redirect_input(vars, command);
			}
		}
		else if(command->output->first_token != NULL)
		{
			if (command->output->first_token->token == T_OUT)
				ft_redirect_output_trunc_mode(vars, command);
			else
				ft_redirect_output_append_mode(command,vars);
		}
		else
			ft_execute(command, vars, contex);
	}
}

int check_built_in_commands(t_vars vars, t_commande *command)
{
	char	*temp;
	int		i;

	i = 0;
	if (command->flags[0] != NULL)
	{
		if (ft_strcmp(command->flags[0], "pwd") == 0)
		{
			ft_pwd(vars, command);
			return (true);
		}
		else if (ft_strcmp(command->flags[0], "env") == 0)
		{
			ft_env(vars, command);
			return (true);
		}
		else if (ft_strcmp(command->flags[0], "exit") == 0)
		{
			if (vars.num_of_commands == 1)
			{
				ft_exit(EXIT_SUCCESS, '\0');
			}
			else
			{
				ft_exit(EXIT_SUCCESS, 'p');
			}
			return (true);
		}
		else if (ft_strcmp(command->flags[0], "cd") == 0)
		{
			ft_cd(command->flags[1], vars.env_list);
			return (true);
		}
		else if (ft_strcmp(command->flags[0], "unset") == 0)
		{
			i = 0;
			while (command->flags[++i])
			{
				ft_unset(&vars.env_list, command->flags[i]);
				ft_unset(&vars.export_list, command->flags[i]);
			}
			return (true);
		}
		else if (ft_strcmp(command->flags[0], "export") == 0 ||\
			ft_strcmp(command->flags[0], "EXPORT") == 0)
		{
			i = 0;
			if (command->flags[1] == NULL)
				ft_export(vars, NULL);
			else
			{
				while (command->flags[++i])
				{
					temp = ft_split(command->flags[i], '=')[0];
					if (!temp)
						break ;
					else if (is_properly_named(temp) == false)
						printf("export: not an identifier: %s\n", temp);
					else if (ft_getenv(vars.env_list, temp) == NULL)
					{
						if (ft_getenv(vars.export_list, temp) == NULL)
						{
							if (is_variable(command->flags[i]) && command->flags[i])
							{
								ft_export(vars, command->flags[i]);
								ft_export(vars, command->flags[i]);
							}
							else
								ft_export(vars, command->flags[i]);
						}
						else
						{
							if (is_variable(command->flags[i]) && command->flags[i])
							{
								ft_unset(&vars.export_list, temp);
								ft_export(vars, command->flags[i]);
								ft_export(vars, command->flags[i]);
							}
						}
					}
					else
					{
						ft_unset(&vars.export_list, temp);
						ft_unset(&vars.env_list, temp);
						ft_lstadd_back(&vars.export_list, \
						ft_lstnew(ft_strdup(command->flags[i])));
						ft_lstadd_back(&vars.env_list, \
						ft_lstnew(ft_strdup(command->flags[i])));
						sort_list(&vars.export_list);
					}
				}
			}
			return (true);
		}
		else if (ft_strcmp(command->flags[0], "echo") == 0 ||\
			ft_strcmp(command->flags[0], "ECHO") == 0)
		{
			exec_echo(vars, command);
			return (true);
		}
		else
			return (false);
	}
	return (false);
}

void exec_echo(t_vars vars, t_commande *command)
{

    if (command->flags[1] == NULL)
        ft_echo(command, NULL, '0');
    else if ((check_echo_flag(command->flags[1])))
        ft_echo(command, join_for_echo(vars.env_list, command->flags, 'n'), 'n');
    else
        ft_echo(command, join_for_echo(vars.env_list, command->flags, '\0'), '\0');
}