/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:52:28 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/04 11:16:43 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	cd_oldwd(t_list *env_list)
{
	char	current_wd[PATH_MAX];
	char	buffer[PATH_MAX];
	char	*temp_path;
	t_list	*old_wd;

	getcwd(current_wd, sizeof(current_wd));
	old_wd = ft_getenv(env_list, "OLDPWD");
	if (old_wd)
	{
		temp_path = ft_split(old_wd->content, '=')[1];
		if (chdir(temp_path) == -1)
		{
			set_exit_code(1);
			perror(temp_path);
		}
		ft_setenv(&env_list, "OLDPWD", current_wd);
		getcwd(buffer, sizeof(buffer));
		ft_setenv(&env_list, "PWD", buffer);
		set_exit_code(0);
	}
	else
	{
		printf("OLDPWD not set\n");
		set_exit_code(1);
	}
}

void	cd_home(t_list *env_list)
{
	char	*home_path;
	char	current_wd[PATH_MAX];

	if (ft_getenv(env_list, "HOME") != NULL)
		home_path = ft_split(ft_getenv(env_list, "HOME")->content, '=')[1];
	else
	{
		write(2, "HOME not set\n", 14);
		set_exit_code(1);
		return ;
	}
	if (home_path != NULL)
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

bool	run_cd(t_vars vars, t_commande *command)
{
	if (!ft_strcmp(command->flags[0], "cd"))
	{
		ft_cd(command->flags[1], vars.env_list);
		return (true);
	}
	return (false);
}
