/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 16:59:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/27 16:59:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_world.h"

void	get_ray_position(t_tsr_ray *ray)
{
	ray->position = vec3_add(ray->origin, vec3_mult_d(ray->forward,
				ray->dist.comp[ray->axis] - ray->abs_delta.comp[ray->axis]));
}

static bool	step_ray(t_tsr_ray *ray)
{
	if (ray->dist.z < ray->dist.x && ray->dist.z < ray->dist.y)
		ray->axis = 2;
	else
		ray->axis = (ray->dist.y < ray->dist.x);
	ray->dist.comp[ray->axis] += ray->abs_delta.comp[ray->axis];
	ray->tile_position.comp[ray->axis] += ray->delta_sign.comp[ray->axis];
	return (ray->tile_position.comp[ray->axis] >= 0
		&& ray->tile_position.comp[ray->axis] < 16);
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
		step_ray(ray);
	return (true);
}

void	trace_ray(t_tsr *tsr, t_tsr_ray *ray)
{
	while (ray->tile->skip_process)
	{
		if (step_ray(ray))
			ray->tile = &tsr->world.tiles[
				block_get(&tsr->wworld, ray->tile_position)];
		else
			ray->tile = &tsr->world.tiles[6];
	}
}
