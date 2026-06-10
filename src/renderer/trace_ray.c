/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:59:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/21 16:41:16 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	get_ray_position(t_tsr_ray *ray)
{
	ray->position = vec3_add(ray->origin, vec3_mult_d(ray->dir,
				ray->distance - ray->abs_delta.v[ray->axis]));
}

static void	check_chunk_bounds(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec3i	c_iter = vec3i(3, 12, 48);

	if (ray->tile_position.v[ray->axis] < 0)
	{
		ray->chunk = tsr->rendering.data.world.chunk_refs[ray->chunk]
			.neighbors[ray->axis * 2];
		ray->tile_position.v[ray->axis] += 4;
		ray->tile_index += c_iter.v[ray->axis];
	}
	else if (ray->tile_position.v[ray->axis] >= 4)
	{
		ray->chunk = tsr->rendering.data.world.chunk_refs[ray->chunk]
			.neighbors[ray->axis * 2 + 1];
		ray->tile_position.v[ray->axis] -= 4;
		ray->tile_index -= c_iter.v[ray->axis];
	}
	else
		ray->tile_index += ray->iter.v[ray->axis];
}

static void	step_ray(t_tsr *tsr, t_tsr_ray *ray)
{
	if (ray->dist.z < ray->dist.x && ray->dist.z < ray->dist.y)
		ray->axis = 2;
	else
		ray->axis = (ray->dist.y < ray->dist.x);
	ray->dist.v[ray->axis] += ray->abs_delta.v[ray->axis];
	ray->tile_position.v[ray->axis] += ray->dir_sign.v[ray->axis];
	if (ray->lifetime-- < 0)
	{
		ray->chunk = 0;
		return ;
	}
	check_chunk_bounds(tsr, ray);
}

void	trace_ray(t_tsr *tsr, t_tsr_ray *ray)
{
	while (true)
	{
		step_ray(tsr, ray);
		if (!ray->chunk)
		{
			ray->tile = &tsr->world.data->skybox;
			ray->tiles = &tsr->rendering.data.world.chunks[0];
			break ;
		}
		if (tsr->rendering.data.world.chunk_refs[ray->chunk].process
			& (1ul << ray->tile_index))
		{
			ray->tiles = &tsr->rendering.data.world.chunks[ray->chunk];
			ray->tile = &ray->tiles->tiles[ray->tile_index];
			if (ray->tile->type <= tsr->world.data->tile_count)
				break ;
		}
	}
	ray->tile_data = &tsr->world.data->tiles[ray->tile->type];
	ray->distance = ray->dist.v[ray->axis];
}
