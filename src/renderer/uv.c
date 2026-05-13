/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 23:38:55 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/13 18:03:30 by emarrot          ###   ########.fr       */
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
	//const t_vec2i	axis = vec2i((ray->axis == 0) * 2, (ray->axis == 1) + 1);
	const t_vec3	dir = tsr->world.global_light;
	t_vec3			position;
	t_tsr_ray		shadow_ray;

	position = vec3_add(ray->position, vec3_mult_d(dir, -1e-8));
	//position.comp[axis.x] = (floor(position.comp[axis.x]
	//			* ray->region->size.x) + 0.5) / ray->region->size.x;
	//position.comp[axis.y] = (floor(position.comp[axis.y]
	//			* ray->region->size.y) + 0.5) / ray->region->size.y;
	shadow_ray = setup_ray(tsr, position, dir);
	shadow_ray.is_shadow = true;
	shadow_ray.tile = &tsr->world.tiles[0];
	shadow_ray.prev_tile = shadow_ray.tile;
	shadow_ray.color = color_rgba(0xffffff00);
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
			shadow_ray.color.a * 0.5));
}

static t_vec3	normal_map_transform(t_vec3 normal, t_tsr_ray *ray)
{
	if (ray->axis == 0 && ray->forward.x < 0)
		normal = vec3(-normal.z, normal.y, normal.x);
	if (ray->axis == 0 && ray->forward.x >= 0)
		normal = vec3(normal.z, normal.y, -normal.x);
	if (ray->axis == 1 && ray->forward.y < 0)
		normal = vec3(normal.x, -normal.z, normal.y);
	if (ray->axis == 1 && ray->forward.y >= 0)
		normal = vec3(normal.x, normal.z, -normal.y);
	if (ray->axis == 2 && ray->forward.z < 0)
		normal = vec3(-normal.x, normal.y, -normal.z);
	return (normal);
}

static t_mbx_color
	perform_shadow_modifiers(t_vec3 forward, t_vec3 normal, t_mbx_color shadow,
	t_tsr *tsr, t_mbx_color color)
{
	const t_vec3	light = vec3(1.8, 1.2, 0.8);
	const t_vec3	dark = vec3(0.8, 1.0, 1.2);
	const t_vec3	dir = tsr->world.global_light;
	double			diffuse;
	double			specular;
	t_vec3			fcolor;
	t_vec3 full_shadow = vec3_mult_d(
		vec3(shadow.r, shadow.g, shadow.b), 3.921569e-3);
	t_vec3 lighten_shadow = vec3_add_d(vec3_mult_d(full_shadow, 0.8), 0.2);
	
	fcolor = vec3_mult_d(vec3(color.r, color.g, color.b), 3.921569e-3);
	fcolor = vec3_exec2(pow, fcolor, vec3_d(2.2));
	diffuse = fclamp(vec3_dot(normal, dir) * -1, 0.0, 1.0) * 0.8;
	specular = pow(fclamp(vec3_dot(vec3_normalize(forward),
		reflect(dir, normal)), 0.0, 1.0), 32) * 1.1;
	fcolor = vec3_mult(fcolor, vec3_add(
				vec3_mult_d(dark, 0.18),
				vec3_mult(light, vec3_add(
				vec3_mult_d(lighten_shadow, diffuse),
				vec3_mult_d(full_shadow, specular)))));
	fcolor = vec3_exec2(pow, fcolor, vec3_d(0.45));
	color.r = min((int)(fcolor.x * 255), 255);
	color.g = min((int)(fcolor.y * 255), 255);
	color.b = min((int)(fcolor.z * 255), 255);
	return (color);
}

t_mbx_color	get_hit_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile)
{
	t_mbx_color		color;
	t_mbx_color 	color_n;
	t_vec3			normal;
	t_mbx_region	*nrm;
	t_vec2i			uv;

	if (tile->skybox)
		get_skybox_uv(ray);
	else
		get_tile_uv(ray);
	ray->region = tile->pbr[ray->axis * 2
		+ (ray->delta_sign.comp[ray->axis] > 0)].col_tex;
	ray->texture_uv = vec2i_mult_vd(ray->region->size, ray->uv);
	color = mbx_get_pixel_unsafe(ray->region, ray->texture_uv);
	if (tile->skybox)
		return (color);
	if (vec3i_eq(ray->tile_position, tsr->world.tile_highlight_pos))
	{
		if (ray->axis == tsr->world.tile_highlight_axis)
			color = color_blend(color, mbx_get_pixel_unsafe(
						tsr->textures.tile_face_highlight, ray->texture_uv));
		else
			color = color_blend(color, mbx_get_pixel_unsafe(
						tsr->textures.tile_highlight, ray->texture_uv));
	}
	if (ray->is_shadow)
		return (color);
	nrm = tile->pbr[ray->axis * 2
		+ (ray->delta_sign.comp[ray->axis] > 0)].nrm_tex;
	if (nrm)
	{
		uv = vec2i_mult_vd(nrm->size, ray->uv);
		color_n = mbx_get_pixel_unsafe(nrm, uv);
		normal = vec3_sub_d(vec3_mult_d(vec3_mult_d(
			vec3(255 - color_n.r, color_n.g, color_n.b),
			3.921569e-3), 2.0), 1.0);
		normal = normal_map_transform(normal, ray);
	}
	else
		normal = get_normal(ray->forward, ray->axis);
	color = perform_shadow_modifiers(
		ray->forward, normal, cast_shadows(tsr, ray), tsr, color);
	return (color);
}
