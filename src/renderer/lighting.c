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

bool
	cast_shadows(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec3	dir = tsr->world_data.skylight;
	t_vec3			position;
	t_tsr_ray		shadow_ray;
	int				i;

	position = vec3_add(ray->position, vec3_mult_d(dir, -1e-6));
	position = vec3_mod_d(position, 4);
	i = 0;
	while (i < 3)
	{
		if (position.v[i] < 0)
			position.v[i] += 4;
		i++;
	}
	shadow_ray = setup_ray(tsr, position, ray->chunk, dir);
	shadow_ray.lifetime = tsr->extras.shadow_distance;
	while (shadow_ray.lifetime)
	{
		trace_ray(tsr, &shadow_ray, true);
		if (!shadow_ray.tile_data->transparent)
			break ;
	}
	return (!shadow_ray.tile_data->skybox);
}

static t_tsr_pbr	get_pbr_properties(t_tsr_ray *ray)
{
	t_vec2i			uv;
	t_mbx_color		col;

	if (~ray->texture->flags & TX_PBR)
		return ((t_tsr_pbr){0});
	uv = vec2i_mult_vd(ray->texture->texture
		[ray->mipmap]->subregion_size, vec2_add(ray->uv, vec2(0, 2)));
	col = mbx_get_pixel_unsafe(ray->texture->texture[ray->mipmap], uv);
	return ((t_tsr_pbr){
		.specular = col.r / 255.,
		.f0 = col.g / 255.,
		.emissive = col.b / 255.
	});
}

static t_vec3
	get_shadow_modifiers(t_tsr *tsr, t_tsr_ray *ray, bool shadow)
{
	t_vec3			light;
	t_tsr_pbr		pbr;
	double			diffuse;

	light = get_global_illumination(tsr, ray);
	pbr = get_pbr_properties(ray);
	if (!shadow)
	{
		get_normal(tsr, ray);
		diffuse = fmax(-vec3_dot(ray->normal, tsr->world_data.skylight), 0);
		if (pbr.specular)
			pbr.specular *= pow(fmax(0, vec3_dot(vec3_normalize(ray->dir),
							reflect(tsr->world_data.skylight, ray->normal))),
					1 + 32 * pbr.f0);
		light = vec3_add(light, vec3_mult_d(
					tsr->world_data.tiles[tsr->world_data.skybox.type].light,
					diffuse + pbr.specular));
	}
	else if (pbr.emissive)
		light = vec3_add(light,
				vec3_mult_d(vec3_sub_rd(1, light), pbr.emissive));
	return (light);
}

static double	get_dither_ratio(t_vec2i pos)
{
	static const int	matrix[8][8] = {
	{0, 32, 8, 40, 2, 34, 10, 42},
	{48, 16, 56, 24, 50, 18, 58, 26},
	{12, 44, 4, 36, 14, 46, 6, 38},
	{60, 28, 52, 20, 62, 30, 54, 22},
	{3, 35, 11, 43, 1, 33, 9, 41},
	{51, 19, 59, 27, 49, 17, 57, 25},
	{15, 47, 7, 39, 13, 45, 5, 37},
	{63, 31, 55, 23, 61, 29, 53, 21}};

	return (matrix[pos.y % 8][pos.x % 8] * 0.00015);
}

void	apply_lighting_effects(
	t_tsr *restrict tsr, t_tsr_ray *restrict ray,
	t_vec2i frag_pos, t_mbx_color *restrict col)
{
	const double	dither = get_dither_ratio(frag_pos);
	t_vec3			light;
	bool			shadow;

	shadow = cast_shadows(tsr, ray);
	light = get_shadow_modifiers(tsr, ray, shadow);
	col->r *= fclamp(sqrt(light.r) + dither, 0, 1);
	col->g *= fclamp(sqrt(light.g) + dither, 0, 1);
	col->b *= fclamp(sqrt(light.b) + dither, 0, 1);
}
