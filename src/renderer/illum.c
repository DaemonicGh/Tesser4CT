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
#include "tsr_renderer.h"

static t_tsr_illum	init_illum(t_tsr_ray *restrict ray)
{
	t_tsr_illum	illum;

	illum = (t_tsr_illum){0};
	illum.axis = vec2i((ray->tile_axis == 0) * 2, (ray->tile_axis == 1) + 1);
	illum.uv.x = ray->position.v[illum.axis.x];
	illum.uv.y = ray->position.v[illum.axis.y];
	illum.uv.x -= floor(illum.uv.x) + 0.5;
	illum.uv.y -= floor(illum.uv.y) + 0.5;
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
	illum.pos = ray->prev_tile_position;
	illum.chunk = ray->prev_chunk;
	return (illum);
}

static bool	horizontal_light_lerp(
	t_tsr *tsr, t_tsr_illum *illum, uint8_t light)
{
	t_vec3			pos;
	t_tsr_chunk_id	chunk;

	pos = vec3_vi(illum->pos);
	pos.v[illum->axis.x] += illum->dir.x;
	chunk = tsr_relocate_chunk(&tsr->rendering.data.world, illum->chunk, &pos);
	if (chunk)
	{
		illum->tiles[light + 1] = &tsr->rendering.data.world.chunks[chunk]
			.tiles[tsr_get_tile_index(vec3i_vd(pos))];
		illum->light[light + 1] = illum->tiles[light + 1]->prev_light;
		illum->light[light] = vec3_exec3(flerp,
				illum->light[light], illum->light[light + 1],
				vec3_d(illum->uv.x));
		return (true);
	}
	illum->light[light + 1] = illum->light[light];
	return (false);
}

t_vec3	get_global_illumination(t_tsr *tsr, t_tsr_ray *ray)
{
	t_tsr_illum		illum;
	t_vec3			dpos;

	illum = init_illum(ray);
	illum.tiles[0] = &tsr->rendering.data.world.chunks[illum.chunk]
		.tiles[tsr_get_tile_index(illum.pos)];
	illum.light[0] = illum.tiles[0]->prev_light;
	horizontal_light_lerp(tsr, &illum, 0);
	illum.pos.v[illum.axis.y] += illum.dir.y;
	dpos = vec3_vi(illum.pos);
	illum.chunk = tsr_relocate_chunk(
			&tsr->rendering.data.world, illum.chunk, &dpos);
	if (!illum.chunk)
		return (illum.light[0]);
	illum.pos = vec3i_vd(dpos);
	illum.tiles[2] = &tsr->rendering.data.world.chunks[illum.chunk]
		.tiles[tsr_get_tile_index(illum.pos)];
	illum.light[2] = illum.tiles[2]->prev_light;
	if ((illum.tiles[1]
			&& tsr->world_data.tiles[illum.tiles[1]->type].transparent)
		|| (illum.tiles[2]
			&& tsr->world_data.tiles[illum.tiles[2]->type].transparent))
		horizontal_light_lerp(tsr, &illum, 2);
	return (vec3_exec3(flerp, illum.light[0], illum.light[2],
			vec3_d(illum.uv.y)));
}
