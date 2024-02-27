/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_mlx.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anferre <anferre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 11:21:53 by anferre           #+#    #+#             */
/*   Updated: 2024/02/27 14:47:15 by anferre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/fdf.h"

int	ft_render(t_data *data)
{
	if (data->mlx->mlx_win)
	{
		ft_render_background(data->data_img, BLACK_COLOR);
		ft_print_map(data);
	}
	return (0);
}

int	ft_handle_notify(t_data	*data)
{
	mlx_destroy_window(data->mlx->mlx, data->mlx->mlx_win);
	data->mlx->mlx_win = NULL;
	ft_free_struct(data);
	exit(0);
}

int	ft_handle_input(int keysym, t_data *data)
{
	double rx;
	int i;

	rx = 0.1;
	i = 0;
	if (keysym == XK_Escape)
	{
		mlx_destroy_window(data->mlx->mlx, data->mlx->mlx_win);
		data->mlx->mlx_win = NULL;
		ft_free_struct(data);
		exit(0);
	}
	if (keysym == XK_Right)
	{
		data->size->a += rx;
		ft_get_middle_coor(data->coor, data->size);
		ft_convert_to_isometric(data->array3d, data->size, data->coor);
		ft_replace(data->coor, data->size, data->size->middle_x, data->size->middle_y);
		return (1);
	}
	if (keysym == XK_Left)
	{
		data->size->a -= rx;
		ft_get_middle_coor(data->coor, data->size);
		ft_convert_to_isometric(data->array3d, data->size, data->coor);
		ft_replace(data->coor, data->size, data->size->middle_x, data->size->middle_y);
		return (1);
	}
	if (keysym == XK_Up)
	{
		data->size->b += rx;
		ft_get_middle_coor(data->coor, data->size);
		ft_convert_to_isometric(data->array3d, data->size, data->coor);
		ft_replace(data->coor, data->size, data->size->middle_x, data->size->middle_y);
		return (1);
	}
	if (keysym == XK_Down)
	{
		data->size->b -= rx;
		ft_get_middle_coor(data->coor, data->size);
		ft_convert_to_isometric(data->array3d, data->size, data->coor);
		ft_replace(data->coor, data->size, data->size->middle_x, data->size->middle_y);
		return (1);
	}
	if (keysym == XK_KP_Down)
	{
		while (i < (data->size->rows * data->size->cols))
		{
			data->coor[i].y += 5;
			i++;
		}
		return (1);
	}
	if (keysym == XK_KP_Up)
	{
		while (i < (data->size->rows * data->size->cols))
		{
			data->coor[i].y -= 5;
			i++;
		}
		return (1);
	}
	if (keysym == XK_KP_Right)
	{
		while (i < (data->size->rows * data->size->cols))
		{
			data->coor[i].x += 5;
			i++;
		}
		return (1);
	}
	if (keysym == XK_KP_Left)
	{
		while (i < (data->size->rows * data->size->cols))
		{
			data->coor[i].x -= 5;
			i++;
		}
		return (1);
	}
	if (keysym == XK_KP_Add)
	{
		data->size->scale_x += 1;
		data->size->scale_y += 1;
		ft_get_middle_coor(data->coor, data->size);
		ft_convert_to_isometric(data->array3d, data->size, data->coor);
		ft_replace(data->coor, data->size, data->size->middle_x, data->size->middle_y);
		return (1);
	}
	if (keysym == XK_KP_Subtract)
	{
		data->size->scale_x -= 1;
		data->size->scale_y -= 1;
		ft_get_middle_coor(data->coor, data->size);
		ft_convert_to_isometric(data->array3d, data->size, data->coor);
		ft_replace(data->coor, data->size, data->size->middle_x, data->size->middle_y);
		return (1);
	}
	if (keysym == XK_Page_Up)
	{
		data->size->scale_z += 0.1;
		ft_get_middle_coor(data->coor, data->size);
		ft_convert_to_isometric(data->array3d, data->size, data->coor);
		ft_replace(data->coor, data->size, data->size->middle_x, data->size->middle_y);
		return (1);
	}
	if (keysym == XK_Page_Down)
	{
		data->size->scale_z -= 0.1;
		ft_get_middle_coor(data->coor, data->size);
		ft_convert_to_isometric(data->array3d, data->size, data->coor);
		ft_replace(data->coor, data->size, data->size->middle_x, data->size->middle_y);
		return (1);
	}
	return (0);
}