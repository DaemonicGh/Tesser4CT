/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   illumination.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 02:38:29 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/15 02:38:29 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_constants.h"
#include "tsr_core.h"

void	cast_illumination_ray(
	t_tsr *tsr, t_tsr_chunk_id chunk, t_vec3 pos)
{
	t_tsr_ray		ray;
	t_tsr_tile		*prev;
	uint8_t			light;

	ray = setup_ray_unsafe(tsr, pos, chunk,
			vec3_neg(tsr->world_data.skylight));
	if (!(ray.tile_data->skybox || ray.tile_data->transparent))
		return ;
	ray.lifetime = ILLUM_RAY_LIFETIME;
	ray.tile->light = 255;
	light = 255;
	while (true)
	{
		step_ray(tsr, &ray);
		if (!ray.chunk)
			return ;
		prev = ray.tile;
		ray.tile = &tsr->world.chunks[ray.chunk].tiles[ray.tile_index];
		ray.tile_data = &tsr->world_data.tiles[ray.tile->type];
		if (!ray.tile_data->transparent)
		{
			prev->light = 255;
			return ;
		}
		ray.tile->light = max(ray.tile->light, light);
		light *= 0.75;
	}
}

static void	cast_limit_rays(
	t_tsr *tsr, t_tsr_chunk_id chunk, uint8_t face)
{
	const uint8_t	axis = face / 2;
	const t_vec2i	plane = vec2i(axis == 0, (axis != 2) + 1);
	t_vec3			pos;

	pos.v[plane.x] = 0.5;
	while (pos.v[plane.x] < 4)
	{
		pos.v[plane.y] = 0.5;
		while (pos.v[plane.y] < 4)
		{
			if (face & 1)
				pos.v[axis] = 4 - 1e-2;
			else
				pos.v[axis] = 1e-2;
			cast_illumination_ray(tsr, chunk, pos);
			pos.v[plane.y]++;
		}
		pos.v[plane.x]++;
	}
}

static void	cast_chunk_limit_rays(t_tsr *tsr, t_tsr_chunk_id chunk)
{
	const int		sides[3] = {
		(tsr->world_data.skylight.x > 0),
		2 + (tsr->world_data.skylight.y > 0),
		4 + (tsr->world_data.skylight.z > 0)};
	t_tsr_tile		*tile;
	size_t			index;

	index = 0;
	while (index < 3)
	{
		if (!tsr->world.chunks[chunk].neighbors[sides[index]])
		{
			tile = &tsr->world.chunks[chunk].limits[sides[index]];
			if (tsr->world_data.tiles[tile->type].skybox)
				cast_limit_rays(tsr, chunk, sides[index]);
		}
		index++;
	}
}

void	cast_illumination_rays(t_tsr *tsr)
{
	t_tsr_chunk		*chunk;
	size_t			i;
	size_t			j;

	i = 1;
	while (i < tsr->world.chunk_count)
	{
		chunk = &tsr->world.chunks[i];
		j = 0;
		while (j < 64)
		{
			if (tsr->world_data.tiles[chunk->tiles[j].type].skybox)
				cast_illumination_ray(tsr, i, vec3_add(vec3_d(0.5),
						vec3(j & 3, j >> 2 & 3, j >> 4 & 3)));
			j++;
		}
		cast_chunk_limit_rays(tsr, i);
		i++;
	}
}
