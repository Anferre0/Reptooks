/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anferre <anferre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 13:19:02 by anferre           #+#    #+#             */
/*   Updated: 2024/03/20 11:23:04 by anferre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

int	ft_pipex_childs(int pipe_fd[2][2], char** env, t_cmd *cmd, int i)
{
	if (i > 0)
	{
		close(pipe_fd[i % 2][1]);
		dup2(pipe_fd[i % 2][0], STDIN_FILENO);
		close(pipe_fd[i % 2][0]);
	}
	if (i < cmd->nb_cmd - 1)
	{
		close(pipe_fd[(i + 1) % 2][0]);
		dup2(pipe_fd[(i + 1) % 2][1], STDOUT_FILENO);
		close(pipe_fd[(i + 1) % 2][1]);
	}
	execve(cmd->path[i], &cmd->args[i][1], env);
	return (perror("execve"), -1);
}

int	ft_pipex_parent(int pipe_fd[2][2], t_cmd *cmd, int i, char **argv, pid_t *child)
{
	int		status;

	waitpid(child[i], &status, 0);
	if (i == cmd->nb_cmd)
	{
		close(pipe_fd[i % 2][1]);
		close(pipe_fd[(i + 1) % 2][0]);
		close(pipe_fd[(i + 1) % 2][1]);
		if (ft_write_output(pipe_fd[i % 2][0], argv, cmd) == -1)
			return (close(pipe_fd[i % 2][0]), -1);
		close(pipe_fd[i % 2][0]);
	}
	return (status);
}

int	ft_pipex(char** env, t_cmd *cmd, char **argv)
{
	int		pipe_fd[2][2];
	pid_t	child[cmd->nb_cmd];
	int		i;
	int		infile_fd;

	i = 0;
	if (pipe(pipe_fd[i]) == -1)
		return (perror("pipe error"), -1);
	if (pipe(pipe_fd[i + 1]) == -1)
		return (perror("pipe error"), -1);
	if (cmd->H_D == true)
		ft_get_input(pipe_fd[0][1]);
	else
	{
		infile_fd = open(argv[1], O_RDONLY);
		if (infile_fd == -1)
			return (perror("open"), -1);
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd);
	}
	while (i < cmd->nb_cmd)
	{
		child[i] = fork();
		if (child[i] == 0)
			ft_pipex_childs(pipe_fd, env, cmd, i);
		if (child[i] > 0)
			ft_pipex_parent(pipe_fd, cmd, i, argv, child);
		i++;
	}
	return (0);
}

int	ft_get_input(int pipe_fd)
{
	int		b_read;
	char	buff[BUFF_SIZE];
	
	while ((b_read = read(STDIN_FILENO, buff, BUFF_SIZE)) > 0)
		if (write(pipe_fd, buff, b_read) != b_read)
			return (perror("write"), -1);
	return (0);
}

int	ft_write_output(int pipe_fd, char **argv, t_cmd *cmd)
{
	int		b_read;
	char	buff[BUFF_SIZE];
	int		outfile_fd;
	int		i;
	
	if (cmd->H_D == true)
		i = cmd->nb_cmd + 3; 
	else
		i = cmd->nb_cmd + 2; 
	outfile_fd = open(argv[i], O_WRONLY);
	while ((b_read = read(pipe_fd, buff, BUFF_SIZE)) > 0)
    	if (write(outfile_fd, buff, b_read) != b_read)
			return (perror("write"), -1);
	return (0);
}

int	ft_check(char **argv, t_cmd *cmd)
{
	int		i;
	t_bool	error;

	i = 0;
	error = false;
	if (ft_strcmp(argv[1], "here_doc") == 0)
		cmd->H_D = true;
	else if (access(argv[1], R_OK) == -1)
	{
		perror(argv[1]);
		error = true;
	}
	while(argv[i + 1])
		i++;
	if (access(argv[i], W_OK) == -1)
	{
		perror(argv[i]);
		error = true;
	}
	if (error == true)
		return (-1);
	if (cmd->H_D == true)
		i -= 1;
	return (i);
}

char 	*ft_find_in_env(char *env, char *cmd)
{
	char	**temp;
	char	*path;
	int		i;

	i = 0;
	temp = ft_split(env, ':');
	while (temp[i])
	{
		path = ft_strjoin(temp[i], cmd);
		if (access(path, X_OK) == 0)
			return (ft_free_split(temp), path);
		i++;
	}
	ft_free_split(temp);
	return (NULL);
}

char	*ft_check_env(char **env, char *cmd)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
	cmd = ft_strjoin("/", cmd);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH", 4) == 0)
		{
			path = ft_find_in_env(env[i], cmd);
			if (path)
				return (free(cmd), path);
		}
		i++;
	}
	return (ft_error("command not found : ", cmd, "\n"), NULL);	
}

int	ft_build_args(char **argv, t_cmd *cmd, char **env)
{
	int	i;
	int	j;

	i = 2;
	j = 0;
	cmd->path = malloc(sizeof(char*) * cmd->nb_cmd);
	if (!cmd->path)
		return (-1);
	cmd->args = malloc(sizeof(char**) * cmd->nb_cmd);
	if (!cmd->args)
		return (ft_unlink(cmd->fd),ft_free_path(cmd->path), -1);
	if (cmd->fd > 0)
		i = 3;
	while (argv[i + 1] != NULL)
	{
		cmd->args[j] = ft_split(argv[i], ' ');
		if (!cmd->args)
			return (ft_unlink(cmd->fd), ft_free_args(cmd->args), ft_free_path(cmd->path), -1);
		cmd->path[j] = ft_check_env(env, cmd->args[j][0]);
		if (!cmd->path[j])
			return (ft_unlink(cmd->fd), ft_free_args(cmd->args), ft_free_path(cmd->path), -1);
		i++;
		j++;
	}
	return (0);
}

int main(int argc, char **argv, char **env)
{
	t_cmd	*cmd;
	
	if (argc)
	// 	return (perror("Arguments missing"), -1);
	// if (env)
	cmd = NULL;
	cmd = ft_newcmd();
	if (!cmd || !env)
		return (free(cmd), -1);
	cmd->nb_cmd = ft_check(argv, cmd) - 2;
	if (cmd->nb_cmd <= 0)
		return (free(cmd),-1);
	if (ft_build_args(argv, cmd, env) < 0)
		return (free(cmd), -1);
	ft_pipex(env, cmd, argv);
	return (0);
}
/*
steps : 

check / build = ok 
for in/outfile (permissions, creation) --> file_check
and here_doc

build = ok
build the args array char *args[] --> build_args
look for the cmds path in the enpv --> get_path

build multiprocess =
create pipes (nb_cmd-1)
create 1 child for each cmd
execute the cmds 
1st child read from infile or infile.txt (here_doc) need to try to read from stdin instead of storing the stdin in a file.
last child write to outfile or if no outfile stdout
be carefull baout open and clos in each child but also to close all in the parent (need to see for infil and outfile if they need a double close)

wait and write = 
wait for all before writting to the outfile */
