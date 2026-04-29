/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 23:38:55 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/29 23:38:55 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	get_skybox_uv(t_tsr_ray *ray)
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
}

static void	get_tile_uv(t_tsr_ray *ray)
{
	get_ray_position(ray);
	if (ray->axis == 0)
		ray->uv = vec2(ray->position.z, ray->position.y);
	else if (ray->axis == 1)
		ray->uv = vec2(ray->position.x, ray->position.z);
	else
		ray->uv = vec2(ray->position.x, ray->position.y);
	ray->uv.x -= floor(ray->uv.x);
	ray->uv.y -= floor(ray->uv.y);
}

void	get_hit_color(t_tsr_ray *ray)
{
	if (ray->tile->skybox)
		get_skybox_uv(ray);
	else
		get_tile_uv(ray);
	ray->texture_uv = vec2i_mult_vd(ray->tile->region->size, ray->uv);
	ray->color = color_blend(
			mbx_get_pixel_unsafe(ray->tile->region, ray->texture_uv),
			ray->color);
	if (ray->tile->skybox && ray->color.a < 0xFF)
		printf("%i %i\n", ray->color.a,
			mbx_get_pixel_unsafe(ray->tile->region, ray->texture_uv).a);
}
