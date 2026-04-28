/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:43:42 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/26 16:55:39 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

t_tsr_ray	setup_ray(t_tsr *tsr, t_vec3 origin, t_vec3 forward)
{
	t_tsr_ray	ray;

	ray.origin = origin;
	ray.forward = forward;
	ray.delta = vec3_div_rd(1, forward);
	ray.abs_delta = vec3_abs(ray.delta);
	ray.delta_sign = vec3i_vd(vec3_sign(ray.delta));
	ray.tile_position = vec3i_vd(vec3_exec(floor, ray.origin));
	ray.tile = &tsr->world.tiles[0];
	ray.dist = vec3_mult(vec3_sub(
				vec3_vi(vec3i_add(ray.tile_position, vec3i(
							ray.delta_sign.x > 0,
							ray.delta_sign.y > 0,
							ray.delta_sign.z > 0))),
				ray.origin), ray.delta);
	return (ray);
}

void	get_skybox_uv(t_tsr_ray *ray)
{
	const t_vec3	abs_fw = vec3_abs(ray->forward);

	if (abs_fw.z > abs_fw.x && abs_fw.z > abs_fw.y)
		ray->axis = 2;
	else
		ray->axis = (abs_fw.y > abs_fw.x);
	if (ray->axis == 0)
		ray->uv = vec2(ray->forward.z, ray->forward.y);
	else if (ray->axis == 1)
		ray->uv = vec2(ray->forward.x, ray->forward.z);
	else
		ray->uv = vec2(ray->forward.x, ray->forward.y);
	ray->uv = vec2_div_d(ray->uv, abs_fw.comp[ray->axis]);
	ray->uv = vec2_add_d(vec2_mult_d(ray->uv, -0.5), 0.5);
	ray->texture_uv = vec2i_mult_vd(ray->tile->region->size, ray->uv);
}

void	get_uv(t_tsr_ray *ray)
{
	get_ray_position(ray);
	if (ray->axis == 0)
		ray->uv = vec2(ray->position.z, ray->position.y);
	else if (ray->axis == 1)
		ray->uv = vec2(ray->position.x, ray->position.z);
	else
		ray->uv = vec2(ray->position.x, ray->position.y);
	ray->uv = vec2(
			ray->uv.x - floor(ray->uv.x),
			ray->uv.y - floor(ray->uv.y));
	ray->texture_uv = vec2i_mult_vd(ray->tile->region->size, ray->uv);
}

t_mbx_color	draw_ray(t_tsr *tsr, t_vec2 uvc)
{
	t_tsr_ray	ray;

	ray = setup_ray(tsr, tsr->camera.position,
			vec3_add(tsr->camera.forward, vec3_add(
					vec3_mult_d(tsr->camera.right, uvc.x * (16.0 / 9)),
					vec3_mult_d(tsr->camera.up, -uvc.y))));
	trace_ray(tsr, &ray);
	if (ray.tile->skybox)
		get_skybox_uv(&ray);
	else
		get_uv(&ray);
	return (mbx_get_pixel_unsafe(ray.tile->region, ray.texture_uv));
}
