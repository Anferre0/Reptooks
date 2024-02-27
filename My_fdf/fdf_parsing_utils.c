/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anferre <anferre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:14:20 by anferre           #+#    #+#             */
/*   Updated: 2024/02/27 14:07:42 by anferre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/fdf.h"

void	*ft_new_array(t_size *size)
{
	int	**array;
	int	i;

	i = 0;
	array = malloc(size->rows * sizeof(int *));
	if (!array)
		return (NULL);
	while (i < size->rows)
	{
		array[i] = malloc(size->cols * sizeof(int));
		if (!array[i])
			return(free(array), NULL);
		i++;
	}
	return (array);
}

void	ft_free_array(int **array, int rows)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (i < rows)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_free_split(char **str)
{
	int	j;

	j = 0;
	while (str[j])
	{
		free(str[j]);
		j++;
	}
	free (str);
}

void	*ft_init_size()
{
	t_size	*size;

	size = malloc(sizeof(t_size));
	if (!size)
		return (NULL);
	size->cols = 0;
	size->rows = 0;
	size->a = 0;
	size->b = 0;
	size->middle_x = 0;
	size->middle_y = 0;
	size->scale_x = 0;
	size->scale_y = 0;
	size->scale_z = 1;
	return (size);
}

int	ft_countcols(char *str)
{
	int	i;
	int	check;
	int	sum;

	i = 0;
	check = 1;
	sum = 0;
	while (str[i])
	{
		if ((str[i] == ' ') | (str[i] == '\t')| (str[i] == '\n'))
			check = 1;
		else if (check == 1)
		{
			sum++;
			check = 0;
		}
		i++;
	}
	return (sum);
}

void	ft_free_struct(t_data *data)
{
	if (data)
	{
		if (data->array3d && data->size->rows)
			ft_free_array(data->array3d, data->size->rows);
		data->array3d = NULL;
		if (data->mlx->mlx && data->data_img->img)
			mlx_destroy_image(data->mlx->mlx, data->data_img->img);
		data->mlx->mlx_win = NULL;
		if (data->coor)
			ft_free_coor(data->coor);
		data->coor = NULL;
		if (data->data_img)
			free(data->data_img);
		data->data_img = NULL;
		if (data->size)
			free(data->size);
		if (data->mlx->mlx)
			mlx_destroy_display(data->mlx->mlx);
		if (data->mlx->mlx)
			free(data->mlx->mlx);
		if (data->mlx)
			free(data->mlx);
		data->mlx = NULL;
		free(data);
	}
}
