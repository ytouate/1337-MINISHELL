/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:52:28 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/17 17:29:18 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	cd_oldwd(t_list *env_list, t_list *export_list)
{
	t_cd_vars	vars;

	getcwd(vars.current_wd, sizeof(vars.current_wd));
	vars.old_wd = ft_getenv(env_list, "OLDPWD");
	if (!vars.old_wd)
	{
		ft_error("cd", ": OLDPWD not set\n", 1);
		return ;
	}
	vars.temp = ft_split(vars.old_wd->content, '=');
	vars.temp_path = ft_strdup(vars.temp[1]);
	vars.i = 0;
	free_2d_array(vars.temp);
	if (chdir(vars.temp_path) == -1)
	{
		set_exit_code(1);
		perror(vars.temp_path);
	}
	ft_setenv(&env_list, "OLDPWD", vars.current_wd);
	ft_setenv(&export_list, "OLDPWD", vars.current_wd);
	getcwd(vars.buffer, sizeof(vars.buffer));
	ft_setenv(&env_list, "PWD", vars.buffer);
	ft_setenv(&export_list, "PWD", vars.buffer);
	free(vars.temp_path);
	set_exit_code(0);
}

void	cd_home(t_list *env_list, t_list *export_list)
{
	char	*home_path;
	char	current_wd[PATH_MAX];

	home_path = ft_get_env_val(env_list, "HOME");
	if (home_path == NULL)
		ft_error("cd", " :HOME not set", 1);
	else
	{
		getcwd(current_wd, sizeof(current_wd));
		if (chdir(home_path) == -1)
		{
			perror(home_path);
			set_exit_code(1);
		}
		else
		{
			ft_setenv(&env_list, "OLDPWD", current_wd);
			ft_setenv(&export_list, "OLDPWD", current_wd);
			set_exit_code(0);
		}
	}
	free(home_path);
}

void	ft_cd(char *path, t_list *env_list, t_list *export_list)
{
	char	current_wd[PATH_MAX];

	if (path == NULL)
		goto home;
	if (ft_strcmp("-", path) == 0)
		cd_oldwd(env_list, export_list);
	else if (ft_strcmp("~", path) == 0)
	{
		home:
		cd_home(env_list, export_list);
	}
	else
	{
		getcwd(current_wd, sizeof(current_wd));
		if (chdir(path) == -1)
		{
			set_exit_code(1);
			perror(path);
		}
		else
		{
			set_exit_code(0);
			ft_setenv(&env_list, "OLDPWD", current_wd);
		}
	}
}

bool	run_cd(t_vars vars, t_command *command)
{
	if (!ft_strcmp(command->flags[0], "cd"))
	{
		ft_cd(command->flags[1], vars.env_list, vars.export_list);
		return (true);
	}
	return (false);
}
