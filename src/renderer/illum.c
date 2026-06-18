/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   illum.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 02:41:22 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/16 03:15:55 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"
#include "tsr_renderer.h"
#include <math.h>

static t_tsr_illum	init_illum(t_tsr *restrict tsr, t_tsr_ray *restrict ray)
{
	t_tsr_illum	illum;

	illum.axis = vec2i((ray->tile_axis == 0) * 2, (ray->tile_axis == 1) + 1);
	illum.uv = vec2(
			ray->position.v[illum.axis.x],
			ray->position.v[illum.axis.y]);
	illum.uv = vec2(
			illum.uv.x - floor(illum.uv.x) - 0.5,
			illum.uv.y - floor(illum.uv.y) - 0.5);
	illum.dir = vec2i_i(1);
	if (illum.uv.x < 0)
	{
		illum.uv.x = -illum.uv.x;
		illum.dir.x = -1;
	}
	if (illum.uv.y < 0)
	{
		illum.uv.y = -illum.uv.y;
		illum.dir.y = -1;
	}
	illum.pos = vec3i_sub(ray->tile_position, ray->tile_normal);
	illum.chunk = tsr_relocate_chunk(&tsr->rendering.data.world,
			ray->chunk, &illum.pos);
	return (illum);
}

static bool	horizontal_light_lerp(
	t_tsr *tsr, t_tsr_illum *illum, uint8_t light)
{
	t_vec3i			pos;
	t_tsr_chunk_id	chunk;

	pos = illum->pos;
	pos.v[illum->axis.x] += illum->dir.x;
	chunk = tsr_relocate_chunk(&tsr->rendering.data.world, illum->chunk, &pos);
	if (chunk)
	{
		illum->light[light + 1] = tsr->rendering.data.world.chunks[chunk]
			.tiles[tsr_get_tile_index(pos)].render_light;
		illum->light[light] = flerp(
				illum->light[light], illum->light[light + 1], illum->uv.x);
		return (true);
	}
	illum->light[light + 1] = illum->light[light];
	return (false);
}

double	get_global_illumination(t_tsr *tsr, t_tsr_ray *ray)
{
	t_tsr_illum		illum;

	illum = init_illum(tsr, ray);
	illum.light[0] = tsr->rendering.data.world.chunks[illum.chunk]
		.tiles[tsr_get_tile_index(illum.pos)].render_light;
	horizontal_light_lerp(tsr, &illum, 0);
	illum.pos.v[illum.axis.y] += illum.dir.y;
	illum.chunk = tsr_relocate_chunk(
			&tsr->rendering.data.world, illum.chunk, &illum.pos);
	if (!illum.chunk)
		return (illum.light[0] / 255);
	illum.light[2] = tsr->rendering.data.world.chunks[illum.chunk]
		.tiles[tsr_get_tile_index(illum.pos)].render_light;
	if (illum.light[1] > illum.light[0]
		|| illum.light[2] > illum.light[0])
		horizontal_light_lerp(tsr, &illum, 2);
	return (flerp(illum.light[0], illum.light[2], illum.uv.y) / 255);
}
