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
	cast_shadows(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec2i	axis = vec2i((ray->axis == 0) * 2, (ray->axis == 1) + 1);
	const t_vec3	dir = vec3(0.98, 0.22, 0.05);
	t_vec3			position;
	t_tsr_ray		shadow_ray;

	position = vec3_add(ray->position, vec3_mult_d(dir, -1e-8));
	position.comp[axis.x] = (floor(position.comp[axis.x]
				* ray->region->size.x) + 0.5) / ray->region->size.x;
	position.comp[axis.y] = (floor(position.comp[axis.y]
				* ray->region->size.y) + 0.5) / ray->region->size.y;
	shadow_ray = setup_ray(tsr, position, dir);
	shadow_ray.is_shadow = true;
	shadow_ray.tile = &tsr->world.tiles[0];
	shadow_ray.prev_tile = shadow_ray.tile;
	while (true)
	{
		trace_ray(tsr, &shadow_ray);
		if (shadow_ray.render_tile)
		{
			if (shadow_ray.tile->skybox)
				break ;
			shadow_ray.color = color_blend(
					get_hit_color(tsr, &shadow_ray, shadow_ray.tile),
					shadow_ray.color);
			if (shadow_ray.color.a == 0xFF)
				break ;
		}
		if (shadow_ray.render_prev_tile)
		{
			if (shadow_ray.prev_tile->skybox)
				break ;
			shadow_ray.color = color_blend(
					get_hit_color(tsr, &shadow_ray, shadow_ray.prev_tile),
					shadow_ray.color);
			if (shadow_ray.color.a == 0xFF)
				break ;
		}
	}
	return (color_r_g_b_a(
			shadow_ray.color.r * (255 - shadow_ray.color.a) >> 8,
			shadow_ray.color.g * (255 - shadow_ray.color.a) >> 8,
			shadow_ray.color.b * (255 - shadow_ray.color.a) >> 8,
			shadow_ray.color.a / 2));
}

static t_mbx_color
	perform_shadow_modifiers(t_tsr_ray *ray, t_mbx_color color)
{
	const t_vec3	light = vec3(1.8, 1.2, 0.8);
	const t_vec3	dark = vec3(0.4, 0.5, 0.6);
	const t_vec3	dir = vec3(0.98, 0.22, 0.05);
	double			inc;

	inc = vec3_dot(get_normal(ray->forward, ray->axis), dir) * 0.5 + 0.5;
	color.r = min(color.r * flerp(light.x, dark.x, inc), 255);
	color.g = min(color.g * flerp(light.y, dark.y, inc), 255);
	color.b = min(color.b * flerp(light.z, dark.z, inc), 255);
	return (color);
}

t_mbx_color	get_hit_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile)
{
	t_mbx_color		color;

	if (tile->skybox)
		get_skybox_uv(ray);
	else
		get_tile_uv(ray);
	ray->region = tile->region[ray->axis * 2
		+ (ray->delta_sign.comp[ray->axis] > 0)];
	ray->texture_uv = vec2i_mult_vd(ray->region->size, ray->uv);
	color = mbx_get_pixel_unsafe(ray->region, ray->texture_uv);
	if (tile->skybox || ray->is_shadow)
		return (color);
	color = color_blend(color, cast_shadows(tsr, ray));
	color = perform_shadow_modifiers(ray, color);
	return (color);
}
