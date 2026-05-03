/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 23:38:55 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/30 15:17:25 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/types/mbx_s_color.h"
#include "tsr.h"

static void	get_skybox_uv(t_tsr_ray *ray)
{
	const t_vec3	abs_fw = vec3_abs(ray->forward);
	t_vec2i			axis;

	if (abs_fw.z > abs_fw.x && abs_fw.z > abs_fw.y)
		ray->axis = 2;
	else
		ray->axis = (abs_fw.y > abs_fw.x);
	axis = vec2i((ray->axis == 0) * 2, (ray->axis == 1) + 1);
	ray->uv = vec2(ray->forward.comp[axis.x], ray->forward.comp[axis.y]);
	ray->uv = vec2_div_d(ray->uv, abs_fw.comp[ray->axis]);
	if ((ray->forward.comp[ray->axis] > 0) == !(ray->axis & 2))
		ray->uv.x = ray->uv.x * 0.5 + 0.5;
	else
		ray->uv.x = -ray->uv.x * 0.5 + 0.5;
	ray->uv.y = -ray->uv.y * 0.5 + 0.5;
}

static void	get_tile_uv(t_tsr_ray *ray)
{
	const t_vec2i	axis = vec2i((ray->axis == 0) * 2, (ray->axis == 1) + 1);

	get_ray_position(ray);
	ray->uv = vec2(ray->position.comp[axis.x], ray->position.comp[axis.y]);
	if ((ray->forward.comp[ray->axis] < 0) == !(ray->axis & 2))
		ray->uv.x = ray->uv.x - floor(ray->uv.x);
	else
		ray->uv.x = ceil(ray->uv.x) - ray->uv.x;
	ray->uv.y = ceil(ray->uv.y) - ray->uv.y;
}

static t_mbx_color
	perform_shadow_modifiers(t_tsr_ray *ray, t_mbx_color color)
{
	const t_vec3	light = vec3(1.8, 1.2, 0.8);
	const t_vec3	dark = vec3(0.4, 0.5, 0.6);
	const t_vec3	dir = vec3(0.98, 0.22, 0);
	double			inc;

	if (ray->render_tile->skybox)
		return (color);
	inc = vec3_dot(get_normal(ray->forward, ray->axis), dir) * 0.5 + 0.5;
	color.r = min(color.r * flerp(light.x, dark.x, inc), 255);
	color.g = min(color.g * flerp(light.y, dark.y, inc), 255);
	color.b = min(color.b * flerp(light.z, dark.z, inc), 255);
	return (color);
}

t_mbx_color	get_hit_color(t_tsr_ray *ray)
{
	t_mbx_region	*region;

	if (ray->render_tile->skybox)
		get_skybox_uv(ray);
	else
		get_tile_uv(ray);
	region = ray->render_tile->region[ray->axis * 2
		+ (ray->delta_sign.comp[ray->axis] > 0)];
	ray->texture_uv = vec2i_mult_vd(region->size, ray->uv);
	return (perform_shadow_modifiers(ray,
			mbx_get_pixel_unsafe(region, ray->texture_uv)));
}
