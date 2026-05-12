/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:59:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/29 23:28:57 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	get_ray_position(t_tsr_ray *ray)
{
	ray->position = vec3_add(ray->origin, vec3_mult_d(ray->forward,
				ray->dist.comp[ray->axis] - ray->abs_delta.comp[ray->axis]));
}

static bool	step_ray(t_tsr *tsr, t_tsr_ray *ray)
{
	if (ray->dist.z < ray->dist.x && ray->dist.z < ray->dist.y)
		ray->axis = 2;
	else
		ray->axis = (ray->dist.y < ray->dist.x);
	ray->dist.comp[ray->axis] += ray->abs_delta.comp[ray->axis];
	ray->tile_position.comp[ray->axis] += ray->delta_sign.comp[ray->axis];
	ray->tile_index += ray->iter.comp[ray->axis];
	return (ray->tile_position.comp[ray->axis] >= 0
		&& ray->tile_position.comp[ray->axis]
		< tsr->wworld.size.comp[ray->axis]);
}

bool	resolve_region_collision(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec3ix2	box = vec3ix2(vec3i_zero(), tsr->wworld.size);
	t_vec3x2		dist;
	t_vec2			min_max;
	int				ray_dist;

	get_ray_position(ray);
	dist = vec3x2(
			vec3_mult(vec3_sub(vec3_vi(box.p1), ray->position), ray->delta),
			vec3_mult(vec3_sub(vec3_vi(box.p2), ray->position), ray->delta));
	min_max.x = vec3_exec_xy_yz(fmax, vec3_exec2(fmin, dist.p1, dist.p2));
	min_max.y = vec3_exec_xy_yz(fmin, vec3_exec2(fmax, dist.p1, dist.p2));
	if (min_max.y < 1e-3 || min_max.x > min_max.y)
		return (false);
	ray_dist = min_max.x;
	while (ray_dist--)
		step_ray(tsr, ray);
	return (true);
}

void	trace_ray(t_tsr *tsr, t_tsr_ray *ray)
{
	ray->render_tile = false;
	ray->render_prev_tile = false;
	while (!ray->render_tile && !ray->render_prev_tile)
	{
		ray->prev_tile = ray->tile;
		if (step_ray(tsr, ray))
			ray->tile = &tsr->world.tiles[
				tsr->wworld.blocks[ray->tile_index]];
		else
			ray->tile = &tsr->world.tiles[6];
		if (ray->tile != ray->prev_tile)
		{
			if (!ray->tile->skip_process)
				ray->render_tile = true;
			if (ray->prev_tile->backface)
				ray->render_prev_tile = true;
		}
		else if (ray->prev_tile->inner_backface)
			ray->render_prev_tile = true;
	}
}
