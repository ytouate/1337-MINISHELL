/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:52:28 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/14 13:18:57 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

typedef struct s_cd_vars
{
	char	current_wd[PATH_MAX];
	char	buffer[PATH_MAX];
	char	*temp_path;
	char	**temp;
	t_list	*old_wd;
	int		i;
}t_cd_vars;

void	cd_oldwd(t_list *env_list)
{
	t_cd_vars	vars;

	getcwd(vars.current_wd, sizeof(vars.current_wd));
	vars.old_wd = ft_getenv(env_list, "OLDPWD");
	if (!vars.old_wd)
	{
		printf("OLDPWD not set\n");
		set_exit_code(1);
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
	getcwd(vars.buffer, sizeof(vars.buffer));
	ft_setenv(&env_list, "PWD", vars.buffer);
	free(vars.temp_path);
	set_exit_code(0);
}

void	cd_home(t_list *env_list)
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
			set_exit_code(0);
		}
		else
		{
			ft_setenv(&env_list, "OLDPWD", current_wd);
			set_exit_code(0);
		}
	}
}

void	ft_cd(char *path, t_list *env_list)
{
	char	current_wd[PATH_MAX];

	if (path == NULL)
		path = ft_strdup("~");
	if (ft_strcmp("-", path) == 0)
		cd_oldwd(env_list);
	else if (ft_strcmp("~", path) == 0)
		cd_home(env_list);
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
		ft_cd(command->flags[1], vars.env_list);
		return (true);
	}
	return (false);
}
