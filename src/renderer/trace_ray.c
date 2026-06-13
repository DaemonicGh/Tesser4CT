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
#include "tsr_core.h"
#include "tsr_world.h"
#include <sys/types.h>

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

static void	set_hit_values(t_tsr *tsr, t_tsr_ray *ray)
{
	t_vec3i			position;
	t_tsr_chunk_id	chunk;

	ray->tile_axis = ray->axis;
	ray->tile_data = &tsr->world_data.tiles[ray->tile->type];
	position = vec3i_sub(ray->tile_position,
			vec3i_vd(get_tile_normal(ray->dir, ray->tile_axis)));
	chunk = tsr_relocate_chunk(&tsr->rendering.data.world,
			ray->chunk, &position);
	ray->front_tile = &tsr->rendering.data.world.chunks[chunk]
		.tiles[tsr_get_tile_index(position)];
	ray->position = vec3_add(ray->origin, vec3_mult_d(ray->dir,
				ray->dist.v[ray->axis] - ray->abs_delta.v[ray->axis]));
}

void	trace_ray(t_tsr *tsr, t_tsr_ray *ray)
{
	t_tsr_chunk_id	prev;

	while (true)
	{
		prev = ray->chunk;
		step_ray(tsr, ray);
		if (!ray->chunk)
		{
			ray->tile = &tsr->rendering.data.world.chunks[prev]
				.limits[ray->axis * 2 + (ray->dir_sign.v[ray->axis] >= 0)];
			break ;
		}
		if (tsr->rendering.data.world.chunk_refs[ray->chunk].process
			& (1ul << ray->tile_index))
		{
			ray->tile = &tsr->rendering.data.world.chunks[ray->chunk]
				.tiles[ray->tile_index];
			break ;
		}
	}
	set_hit_values(tsr, ray);
}
