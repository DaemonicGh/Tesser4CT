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
#include "tsr_core.h"
#include "tsr_renderer.h"
#include "tsr_world.h"

static t_vec4
	cast_shadows(t_tsr *tsr, t_tsr_ray *ray)
{
	const t_vec3	dir = tsr->world_data.skylight;
	t_vec3			position;
	t_tsr_ray		shadow_ray;

	position = vec3_exec3(fwrap, ray->position, vec3_d(0), vec3_d(4));
	position = vec3_add(position, vec3_mult_d(ray->dir, -1e-6));
	shadow_ray = setup_ray(tsr, position, ray->chunk, dir);
	shadow_ray.lifetime = 50;
	shadow_ray.is_shadow = true;
	while (true)
	{
		trace_ray(tsr, &shadow_ray);
		if (shadow_ray.tile_data->skybox)
			return (vec4(1, 1, 1, 0));
		if (set_ray_tile_color(tsr, &shadow_ray, shadow_ray.tile))
			return (vec4(0, 0, 0, 0.5));
	}
}

static t_tsr_pbr	get_pbr_properties(t_tsr_ray *ray)
{
	t_vec2i			uv;
	t_mbx_color		col;

	if (~ray->texture->flags & TX_PROPERTY)
		return ((t_tsr_pbr){0});
	uv = vec2i_mult_vd(ray->texture->texture->subregion_size,
			vec2_add(ray->uv, vec2(0, 2)));
	col = mbx_get_pixel_unsafe(ray->texture->texture, uv);
	return ((t_tsr_pbr){
		.specular = col.r / 255.,
		.f0 = col.g / 255.,
		.emissive = col.b / 255.
	});
}

static float	get_global_light(t_tsr *tsr, t_tsr_ray *ray)
{
	t_vec2i			axis;
	t_vec2			uv;
	t_vec2i			dir;
	float			light[6];
	t_vec3i			origin;
	t_tsr_chunk_id	chunk_origin;
	t_vec3i			pos;
	t_tsr_chunk_id	chunk;

	axis = vec2i((ray->tile_axis == 0) * 2, (ray->tile_axis == 1) + 1);
	uv = vec2(ray->position.v[axis.x], ray->position.v[axis.y]);
	uv = vec2(uv.x - floor(uv.x), uv.y - floor(uv.y));
	origin = vec3i_sub(ray->tile_position,
			vec3i_vd(get_tile_normal(ray->dir, ray->tile_axis)));
	chunk_origin = tsr_relocate_chunk(&tsr->rendering.data.world,
			ray->chunk, &origin);
	dir = vec2i(1, 1);
	if (uv.x < 0.5)
	{
		uv.x = 0.5 - uv.x;
		dir.x = -1;
	}
	else
		uv.x -= 0.5;
	if (uv.y < 0.5)
	{
		uv.y = 0.5 - uv.y;
		dir.y = -1;
	}
	else
		uv.y -= 0.5;
	chunk_origin = tsr_relocate_chunk(&tsr->rendering.data.world,
			chunk_origin, &origin);
	pos = origin;
	chunk = chunk_origin;
	light[0] = tsr->rendering.data.world.chunks[chunk_origin]
		.tiles[tsr_get_tile_index(origin)].light;
	pos = origin;
	chunk = chunk_origin;
	pos.v[axis.x] += dir.x;
	chunk = tsr_relocate_chunk(&tsr->rendering.data.world, chunk, &pos);
	if (chunk)
	{
		light[1] = tsr->rendering.data.world.chunks[chunk]
			.tiles[tsr_get_tile_index(pos)].light;
		light[0] = flerp(light[0], light[1], uv.x);
	}
	origin.v[axis.y] += dir.y;
	chunk = chunk_origin;
	pos = origin;
	chunk = tsr_relocate_chunk(&tsr->rendering.data.world, chunk, &pos);
	if (chunk)
		light[2] = tsr->rendering.data.world.chunks[chunk]
			.tiles[tsr_get_tile_index(pos)].light;
	else
		light[2] = light[0];
	if (light[1] > light[0] || light[2] > light[0])
	{
		chunk = chunk_origin;
		pos = origin;
		pos.v[axis.x] += dir.x;
		chunk = tsr_relocate_chunk(&tsr->rendering.data.world, chunk, &pos);
		if (chunk)
		{
			light[3] = tsr->rendering.data.world.chunks[chunk]
				.tiles[tsr_get_tile_index(pos)].light;
			light[2] = flerp(light[2], light[3], uv.x);
		}
	}
	return (flerp(light[0], light[2], uv.y) / 128);
}

static t_vec4
	get_shadow_modifiers(t_tsr *tsr, t_tsr_ray *ray, t_vec4 shadow)
{
	const t_vec4	dark = vec4_mult_d(
			vec4(0.4, 0.6, 1.0, 1.0), get_global_light(tsr, ray));
	t_vec4			light;
	t_tsr_pbr		pbr;
	double			diffuse;

	pbr = get_pbr_properties(ray);
	diffuse = fmax(-vec3_dot(ray->tile_normal, tsr->world_data.skylight), 0);
	if (pbr.specular)
		pbr.specular *= pow(fmax(0, vec3_dot(vec3_normalize(ray->dir),
						reflect(tsr->world_data.skylight, ray->tile_normal))),
				1 + 32 * pbr.f0);
	light = vec4_add(dark, vec4_mult_d(
				vec4_mult(tsr->world.data->skylight_color, shadow),
				diffuse + pbr.specular));
	if (pbr.emissive)
		light = vec4_add(light,
				vec4_mult_d(vec4_sub_rd(1, light), pbr.emissive));
	return (light);
}

void	apply_lighting_effects(
	t_tsr *tsr, t_tsr_ray *ray, t_vec4 *col)
{
	t_vec4		ncol;
	t_vec4		shadow;
	t_vec4		light;

	get_normal(tsr, ray);
	ncol = vec4(col->r * col->r, col->g * col->g, col->b * col->b, col->a);
	shadow = cast_shadows(tsr, ray);
	light = get_shadow_modifiers(tsr, ray, shadow);
	ncol = vec4_mult(ncol, light);
	col->r = fmin(sqrt(ncol.x), 1);
	col->g = fmin(sqrt(ncol.y), 1);
	col->b = fmin(sqrt(ncol.z), 1);
}
