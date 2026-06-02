/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:55:17 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/27 14:38:18 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_vec4
	cast_shadows(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec3	dir = tsr->world.global_light;
	t_tsr_ray		shadow_ray;

	return (vec4(1, 1, 1, 1));
	shadow_ray = setup_ray(tsr, vec3_add(ray->position,
				vec3_mult_d(dir, -1e-4)), ray->chunk, dir);
	shadow_ray.color = vec4(1, 1, 1, 0);
	shadow_ray.lifetime = 20;
	shadow_ray.is_shadow = true;
	while (true)
	{
		trace_ray(tsr, &shadow_ray);
		if (shadow_ray.draw_tile)
		{
			if (shadow_ray.tile->skybox)
				break ;
			if (set_ray_tile_color(tsr, &shadow_ray, shadow_ray.tile))
				return (vec4(1, 1, 1, 0.5));
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
		t_vec4 shadow, t_tsr_tile *tile)
{
	const t_vec4	light = tsr->world.global_light_col;
	const t_vec4	dark = vec4(0.4, 0.6, 1.0, 1.0);
	double			diffuse;
	double			specular;

	diffuse = fmax(-vec3_dot(ray->tile_normal, tsr->world.global_light), 0);
	specular = 0.0;
	if (tile->specular)
		specular = pow(fmax(vec3_dot(vec3_normalize(ray->dir),
						reflect(tsr->world.global_light, ray->tile_normal)),
					0), 32);
	return (vec4_add(dark,
			vec4_mult(light,
				vec4_add(
					vec4_mult_d(vec4_mult(shadow, light), diffuse),
					vec4_mult_d(shadow, specular)))));
}

void	apply_lighting_effects(
	t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile, t_vec4 *col)
{
	t_vec4	ncol;
	t_vec4	shadow;

	get_normal(ray, tile);
	ncol = vec4(col->r * col->r, col->g * col->g, col->b * col->b, col->a);
	shadow = cast_shadows(tsr, ray);
	ncol = vec4_mult(ncol, get_shadow_modifiers(tsr, ray, shadow, tile));
	col->r = fmin(sqrt(ncol.x), 1);
	col->g = fmin(sqrt(ncol.y), 1);
	col->b = fmin(sqrt(ncol.z), 1);
}
