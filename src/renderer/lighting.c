/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:55:17 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/11 16:15:06 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_vec4
	cast_shadows(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec3	dir = tsr->world.global_light;
	t_vec3			position;
	t_tsr_ray		shadow_ray;

	position = vec3_add(ray->position, vec3_mult_d(dir, -1e-8));
	shadow_ray = setup_ray(tsr, position, dir);
	shadow_ray.color = vec4(1, 1, 1, 0);
	shadow_ray.is_shadow = true;
	while (true)
	{
		trace_ray(tsr, &shadow_ray);
		if (shadow_ray.draw_tile)
		{
			if (shadow_ray.tile->skybox)
				break ;
			shadow_ray.color = vec4_blend(shadow_ray.color,
					ray_tile_color(tsr, &shadow_ray, shadow_ray.tile));
			if (shadow_ray.color.a == 1)
				break ;
		}
		if (shadow_ray.draw_prev_tile)
		{
			if (shadow_ray.prev_tile->skybox)
				break ;
			shadow_ray.color = vec4_blend(shadow_ray.color,
					ray_tile_color(tsr, &shadow_ray, shadow_ray.prev_tile));
			if (shadow_ray.color.a == 1)
				break ;
		}
	}
	return (vec4(
			shadow_ray.color.r * (1 - shadow_ray.color.a),
			shadow_ray.color.g * (1 - shadow_ray.color.a),
			shadow_ray.color.b * (1 - shadow_ray.color.a),
			shadow_ray.color.a * 0.5));
}

static t_vec4
	get_shadow_modifiers(t_tsr *tsr, t_tsr_ray *ray,
		t_vec4 shadow, float *properties)
{
	const t_vec4	light = tsr->world.global_light_col;
	const t_vec4	dark = vec4(0.4, 0.6, 1.0, 1.0);
	double			diffuse;
	double			specular;

	diffuse = fmax(-vec3_dot(ray->tile_normal, tsr->world.global_light), 0);
	specular = properties[1] * pow(fmax(vec3_dot(vec3_normalize(ray->dir),
			reflect(tsr->world.global_light, ray->tile_normal)), 0),
			32 * properties[0] + 1.0);
	return vec4_add(dark, vec4_mult_d(vec4_mult(light, shadow),
		diffuse + specular));
}

static void get_properties(t_tsr_ray *ray, t_tsr_tile *tile, float *properties)
{
	t_tsr_texture	*tex;
	t_vec2i 		uv;
	t_mbx_color		col;

	properties[0] = 0.0;
	properties[1] = 0.0;
	properties[2] = 0.0;
	tex = tile->texture[ray->axis * 2 + (ray->dir_sign.v[ray->axis] > 0)];
	if (~tex->flags & TX_PROPERTY)
		return ;
	uv = vec2i_mult_vd(tex->texture->size, ray->uv);
	uv.y = uv.y / 3 + tex->texture->size.x * 2;
	col = mbx_get_pixel_unsafe(tex->texture, uv);
	properties[0] = (float)col.r / 255;
	properties[1] = (float)col.g / 255;
	properties[2] = (float)col.b / 255;
}

void	apply_lighting_effects(
	t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile, t_vec4 *col)
{
	t_vec4	ncol;
	t_vec4	shadow;
	t_vec4	light;
	float	properties[3];

	get_normal(ray, tile);
	ncol = vec4(col->r * col->r, col->g * col->g, col->b * col->b, col->a);
	shadow = cast_shadows(tsr, ray);
	get_properties(ray, tile, properties);
	light = get_shadow_modifiers(tsr, ray, shadow, properties);
	light = vec4_add(light, vec4_mult_d(vec4_sub(vec4_d(1.0), light),
		properties[2]));
	ncol = vec4_mult(ncol, light);
	col->r = fmin(sqrt(ncol.x), 1);
	col->g = fmin(sqrt(ncol.y), 1);
	col->b = fmin(sqrt(ncol.z), 1);
}
