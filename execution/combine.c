/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:06:57 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 21:53:02 by ytouate          ###   ########.fr       */
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
                ft_heredoc(vars, command, contex);
			else
				redirect_input(vars, command);
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

int run_pwd(t_vars vars, t_commande *command)
{
	if (!ft_strcmp(command->flags[0], "pwd"))
	{
		ft_pwd(vars, command);
		return (true);
	}
	return (false);
}

bool run_env(t_vars vars, t_commande *command)
{
	if (!ft_strcmp(command->flags[0], "env"))
	{
		ft_env(vars, command);
		return (true);
	}
	return (false);
}
bool run_exit(t_vars vars, t_commande *command)
{
	if (!ft_strcmp(command->flags[0], "exit"))
	{
		if (vars.num_of_commands == 1)
			ft_exit(0, '\0');
		else
			ft_exit(0, 'p');
		return (true);	
	}
	return (false);
}

bool run_cd(t_vars vars, t_commande *command)
{
	if (!ft_strcmp(command->flags[0], "cd"))
	{
		ft_cd(command->flags[1], vars.env_list);
		return (true);
	}
	return (false);
}
bool run_unset(t_vars vars, t_commande *command)
{
	int i;
	if (!ft_strcmp(command->flags[0], "unset"))
	{
		i = 0;
		while (command->flags[++i])
		{
			ft_unset(&vars.env_list, command->flags[i]);
			ft_unset(&vars.export_list, command->flags[i]);
		}
		return (true);
	}
	return (false);
}

bool exec_echo(t_vars vars, t_commande *command)
{
	if (ft_strcmp(command->flags[0], "echo") == 0 || \
		ft_strcmp(command->flags[0], "ECHO") == 0)
	{
		if (command->flags[1] == NULL)
        	ft_echo(command, NULL, '0');
    	else if ((check_echo_flag(command->flags[1])))
    	    ft_echo(command, join_for_echo(vars.env_list, command->flags, 'n'), 'n');
    	else
        	ft_echo(command, join_for_echo(vars.env_list, command->flags, '\0'), '\0');
		return (true);
	}
	return (false);
}

bool show_export_list(t_vars vars, t_commande *command)
{
	if (command->flags[1] == NULL)
	{
		ft_export(vars, command, NULL);
		return (true);
	}
	return (false);
}

bool run_export(t_vars vars, t_commande *command)
{
	int i;
	char **temp;
	i = 0;
	if (ft_strcmp(command->flags[0], "export") == 0 ||\
		ft_strcmp(command->flags[0], "EXPORT") == 0)
	{
		{
			if (!show_export_list(vars, command))
			{	
				while (command->flags[++i])
				{
					temp = ft_split(command->flags[i], '=');
					if (!is_properly_named(temp[0]))
						printf("export: not an identifier: %s\n", temp[0]);
					else if (!add_unexisted_variable(vars, command, temp, &i))
					{
						add_existed_variable(vars, command, temp, &i);
					}
				}
			}
		}
		return (true);
	}
	return (false);
}

bool	add_unexisted_variable(t_vars vars, t_commande *command, char **var_data, int *i)
{
	if (ft_getenv(vars.env_list, var_data[0]) == NULL)
	{
		if (ft_getenv(vars.export_list, var_data[0]) == NULL)
		{
			if (is_variable(command->flags[*i]) && command->flags[*i])
			{
				ft_export(vars, command, command->flags[*i]);
				ft_export(vars, command, command->flags[*i]);
			}
			else
				ft_export(vars,command, command->flags[*i]);
		}
		else
		{
			if (is_variable(command->flags[*i]) && command->flags[*i])
			{
				ft_unset(&vars.export_list, var_data[0]);
				ft_export(vars, command, command->flags[*i]);
				ft_export(vars, command, command->flags[*i]);
			}
		}
		return (true);
	}
	return (false);
}

bool add_existed_variable(t_vars vars, t_commande *command, char **var_data, int *i)
{
	ft_unset(&vars.export_list, var_data[0]);
	ft_unset(&vars.env_list, var_data[0]);
	ft_lstadd_back(&vars.export_list, \
	ft_lstnew(ft_strdup(command->flags[*i])));
	ft_lstadd_back(&vars.env_list, \
	ft_lstnew(ft_strdup(command->flags[*i])));
	sort_list(&vars.export_list);
	return (true);
}
int check_built_in_commands(t_vars vars, t_commande *command)
{
	if (command->flags[0] != NULL)
	{
		if (run_pwd(vars, command))
			return (true);
		else if (run_env(vars, command))
			return (true);
		else if (run_exit(vars, command))
			return (true);
		else if (run_cd(vars, command))
			return (true);
		else if (run_unset(vars, command))
			return (true);
		else if (run_export(vars, command))
			return (true);
		else if (exec_echo(vars, command))
			return (true);
		else
			return (false);
	}
	return (false);
}