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

static void	check_chunk_bounds(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec3i	c_iter = vec3i(3, 12, 48);

	if (ray->tile_position.v[ray->axis] < 0)
	{
		ray->chunk = tsr->rendering.data.world.chunks
		[ray->chunk].neighbors[ray->axis * 2];
		ray->tile_position.v[ray->axis] += 4;
		ray->tile_index += c_iter.v[ray->axis];
	}
	else if (ray->tile_position.v[ray->axis] >= 4)
	{
		ray->chunk = tsr->rendering.data.world.chunks
		[ray->chunk].neighbors[ray->axis * 2 + 1];
		ray->tile_position.v[ray->axis] -= 4;
		ray->tile_index -= c_iter.v[ray->axis];
	}
	else
	{
		ray->tile_index += ray->iter.v[ray->axis];
		return ;
	}
	if (ray->lifetime-- < 0)
		ray->chunk = 0;
}

void	step_ray(t_tsr *restrict tsr, t_tsr_ray *restrict ray)
{
	if (ray->dist.z < ray->dist.x && ray->dist.z < ray->dist.y)
		ray->axis = 2;
	else
		ray->axis = (ray->dist.y < ray->dist.x);
	ray->dist.v[ray->axis] += ray->abs_delta.v[ray->axis];
	ray->tile_position.v[ray->axis] += ray->dir_sign.v[ray->axis];
	check_chunk_bounds(tsr, ray);
}

static void	set_hit_values(t_tsr *tsr, t_tsr_ray *ray)
{
	double	angle;

	ray->tile_axis = ray->axis;
	ray->tile_normal = vec3i_zero();
	ray->tile_normal.v[ray->axis] = fsign(ray->dir.v[ray->axis]);
	ray->position = vec3_add(ray->origin, vec3_mult_d(ray->dir,
				ray->dist.v[ray->axis] - ray->abs_delta.v[ray->axis]));
	ray->distance = vec3_dist(ray->origin, ray->position);
	if (ray->tile_data->skybox)
		ray->mipmap = 0;
	else
	{
		angle = 1 - fabs(vec3_dot(vec3_vi(ray->tile_normal), vec3_div_d(
						ray->dir, (tsr->extras.focal_length - 1) * 0.5 + 1)));
		ray->mipmap = fclamp(fmax(ray->distance, 12)
				/ (tsr->rendering.target->size.y / 90.) * pow(angle, 4), 0, 3);
	}
}

static void	trace_ray_loop(
	t_tsr *tsr, t_tsr_ray *ray, bool backface)
{
	while (true)
	{
		ray->prev_chunk = ray->chunk;
		ray->prev_tile = ray->tile;
		ray->prev_tile_position = ray->tile_position;
		step_ray(tsr, ray);
		if (!ray->chunk)
		{
			ray->tile = &tsr->rendering.data.world.chunks[ray->prev_chunk]
				.limits[ray->axis * 2 + (ray->dir_sign.v[ray->axis] >= 0)];
			return ;
		}
		ray->tile = &tsr->rendering.data.world.chunks[ray->chunk]
			.tiles[ray->tile_index];
		if (!tsr->world_data.tiles[ray->tile->type].skip)
		{
			if (!backface || ray->tile->type != ray->prev_tile->type)
				return ;
		}
		else if (backface)
			break ;
	}
	ray->backface = true;
	ray->tile = ray->prev_tile;
}

void	trace_ray(
	t_tsr *restrict tsr, t_tsr_ray *restrict ray, bool lighting_ray)
{
	const bool	backface = !lighting_ray && ray->tile_data
		&& ray->tile_data->backface && !ray->backface;

	ray->backface = false;
	trace_ray_loop(tsr, ray, backface);
	ray->tile_data = &tsr->world_data.tiles[ray->tile->type];
	if (!lighting_ray)
		set_hit_values(tsr, ray);
}
