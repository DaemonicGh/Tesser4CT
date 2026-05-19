/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:55:17 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/18 15:55:17 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_mbx_color
	cast_shadows(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec3	dir = tsr->world.global_light;
	t_vec3			position;
	t_tsr_ray		shadow_ray;

	position = vec3_add(ray->position, vec3_mult_d(dir, -1e-8));
	shadow_ray = setup_ray(tsr, position, dir);
	shadow_ray.is_shadow = true;
	shadow_ray.color = color_rgba(0xFFFFFF00);
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

static void
	perform_shadow_modifiers(t_vec3 forward, t_vec3 normal, t_mbx_color shadow,
	t_tsr *tsr, t_vec4 *color, t_tsr_tile *tile)
{
	const t_vec4x2	light = vec4x2(
			vec4(1.8, 1.2, 0.8, 1.0), vec4(0.4, 0.6, 1.0, 1.0));
	const t_vec4	full_shadow = vec4_from_color(shadow);
	const t_vec4	lighten_shadow = vec4_add_d(
			vec4_mult_d(full_shadow, 0.8), 0.2);
	double			diffuse;
	double			specular;

	diffuse = fclamp(-vec3_dot(normal, tsr->world.global_light),
			0.0, 1.0) * 0.8;
	if (tile->specular)
		specular = pow(fclamp(vec3_dot(vec3_normalize(forward),
						reflect(tsr->world.global_light, normal)),
					0.0, 1.0), 32) * 1.1;
	else
		specular = 0.0;
	*color = vec4_mult(*color, vec4_add(light.p2,
				vec4_mult(light.p1, vec4_add(
						vec4_mult_d(lighten_shadow, diffuse),
						vec4_mult_d(full_shadow, specular)))));
}

void	do_lighting(
	t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile, t_mbx_color *color)
{
	t_vec4	ncolor;

	ncolor = vec4_square(vec4_from_color(*color));
	perform_shadow_modifiers(
		ray->forward, ray->normal, cast_shadows(tsr, ray), tsr, &ncolor, tile);
	color->r = min((int)(sqrt(ncolor.x) * 255), 255);
	color->g = min((int)(sqrt(ncolor.y) * 255), 255);
	color->b = min((int)(sqrt(ncolor.z) * 255), 255);
}
