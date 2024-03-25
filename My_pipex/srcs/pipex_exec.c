/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anferre <anferre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:49:19 by anferre           #+#    #+#             */
/*   Updated: 2024/03/25 18:54:07 by anferre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <../include/pipex.h>

static int	ft_pipex_childs(int p_fd[2][2], char **env, t_cmd *cmd, int i)
{
	close(p_fd[i % 2][1]);
	if (i > 0)
	{
		if (dup2(p_fd[i % 2][0], STDIN_FILENO) == -1)
		{
			ft_c_fd(p_fd[(i + 1) % 2], NULL, cmd->std_fd);
			return (close(p_fd[i % 2][0]), -1);
		}
	}
	close(p_fd[i % 2][0]);
	if (dup2(p_fd[(i + 1) % 2][1], STDOUT_FILENO) == -1)
	{
		ft_c_fd(p_fd[0], p_fd[1], cmd->std_fd);
		return (-1);
	}
	close(p_fd[(i + 1) % 2][0]);
	close(p_fd[(i + 1) % 2][1]);
	close(cmd->std_fd[0]);
	close(cmd->std_fd[1]);
	execve(cmd->path[i], cmd->args[i], env);
	if (i < cmd->nb_cmd - 1)
		perror("execve");
	exit(errno);
}

static int	ft_pipex_parent(int p_fd[2][2], t_cmd *cmd, int i)
{
	if (dup2(cmd->std_fd[0], STDIN_FILENO) == -1)
		return (ft_c_fd(p_fd[0], p_fd[1], cmd->std_fd), -1);
	if (dup2(cmd->std_fd[1], STDOUT_FILENO) == -1)
		return (ft_c_fd(p_fd[0], p_fd[1], cmd->std_fd), -1);
	close(p_fd[i % 2][1]);
	close(p_fd[i % 2][0]);
	if (i < cmd->nb_cmd - 1)
	{
		if (pipe(p_fd[i % 2]) == -1)
		{
			ft_c_fd(p_fd[(i + 1) % 2], NULL, cmd->std_fd);
			return (perror("pipe"), -1);
		}
	}
	return (0);
}

static int	ft_write_output(int pipe_fd, char **argv, t_cmd *cmd)
{
	char	buff[BUFF_SIZE];
	int		b_read;
	int		outfile_fd;
	int		i;

	if (cmd->h_d == true)
		i = cmd->nb_cmd + 3;
	else
		i = cmd->nb_cmd + 2;
	if (access(argv[i], F_OK) != -1)
		outfile_fd = open(argv[i], O_WRONLY | O_CREAT | O_EXCL, 0600);
	else if (cmd->h_d == false)
		outfile_fd = open(argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	else
		outfile_fd = open(argv[i], O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (outfile_fd < 0)
		return (perror("Open_Outfile"), -1);
	b_read = read(pipe_fd, buff, BUFF_SIZE);
	while (b_read > 0)
	{
		if (write(outfile_fd, buff, b_read) != b_read)
			return (close(outfile_fd), perror("write"), -1);
		b_read = read(pipe_fd, buff, BUFF_SIZE);
	}
	return (close(outfile_fd), 0);
}

static int	ft_wait_write(t_cmd *cmd, int p_fd[2][2], char **argv, pid_t *child)
{
	int		status;
	int		i;

	i = 0;
	status = 0;
	while (i < cmd->nb_cmd)
	{
		if ((waitpid(child[i], &status, 0) < 0))
			return (perror("waitpid"), -1);
		i++;
	}
	i--;
	ft_c_fd(p_fd[i % 2], NULL, cmd->std_fd);
	close(p_fd[(i + 1) % 2][1]);
	if (ft_write_output(p_fd[(i + 1) % 2][0], argv, cmd) == -1)
		return (close(p_fd[(i + 1) % 2][0]), -1);
	close(p_fd[(i + 1) % 2][0]);
	if (cmd->path[i] == NULL)
	{
		ft_free_all(cmd, cmd->nb_cmd);
		exit(127);
	}
	return (0);
}

int	ft_pipex(char **env, t_cmd *cmd, char **argv)
{
	int		p_fd[2][2];
	pid_t	*child;
	int		i;

	i = -1;
	child = malloc(sizeof(pid_t) * cmd->nb_cmd);
	if (!child)
		return (-1);
	if (ft_create_pipes(cmd, argv, p_fd) < 0)
		return (free(child), -1);
	while (++i < cmd->nb_cmd)
	{
		child[i] = fork();
		if (child[i] < 0)
			return (perror("fork"), free(child), ft_c_fd(p_fd[0], p_fd[1], cmd->std_fd), -1);
		if (child[i] == 0)
			if (ft_pipex_childs(p_fd, env, cmd, i) == -1)
				return (free(child), close(cmd->std_fd[0]), close(cmd->std_fd[1]), -1);
		if (child[i] > 0)
			if (ft_pipex_parent(p_fd, cmd, i) != 0)
				return (free(child), -1);
	}
	if ((ft_wait_write(cmd, p_fd, argv, child)) == -1)
		return (free(child), -1);
	return (free(child), 0);
}
