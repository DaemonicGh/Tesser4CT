/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:59:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/13 18:48:43 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	get_ray_position(t_tsr_ray *ray)
{
	ray->position = vec3_add(ray->origin, vec3_mult_d(ray->dir,
				ray->distance - ray->abs_delta.v[ray->axis]));
}

static bool	is_ray_inbounds(t_tsr *tsr, t_tsr_ray *ray)
{
	return (ray->tile_position.v[ray->axis] >= 0
		&& ray->tile_position.v[ray->axis]
		< tsr->wworld.size.v[ray->axis]);
}

static bool	step_ray(t_tsr *tsr, t_tsr_ray *ray)
{
	if (ray->dist.z < ray->dist.x && ray->dist.z < ray->dist.y)
		ray->axis = 2;
	else
		ray->axis = (ray->dist.y < ray->dist.x);
	ray->dist.v[ray->axis] += ray->abs_delta.v[ray->axis];
	ray->tile_position.v[ray->axis] += ray->dir_sign.v[ray->axis];
	ray->tile_index += ray->iter.v[ray->axis];
	return (is_ray_inbounds(tsr, ray));
}

void	trace_ray(t_tsr *tsr, t_tsr_ray *ray)
{
	ray->draw_tile = false;
	ray->draw_prev_tile = false;
	while (!ray->draw_tile && !ray->draw_prev_tile)
	{
		ray->prev_tile = ray->tile;
		if (step_ray(tsr, ray))
			ray->tile = &tsr->world.tiles[
				tsr->wworld.blocks[ray->tile_index]];
		else
			ray->tile = &tsr->world.tiles[1];
		if (ray->tile != ray->prev_tile)
		{
			if (!ray->tile->skip)
				ray->draw_tile = true;
			if (ray->prev_tile->backface)
				ray->draw_prev_tile = true;
		}
		else if (ray->prev_tile->inner_backface)
			ray->draw_prev_tile = true;
	}
	ray->distance = ray->dist.v[ray->axis];
}
