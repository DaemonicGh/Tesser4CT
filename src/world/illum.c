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
#include "tsr_renderer.h"

static void	spread_light(
	t_tsr *tsr, t_tsr_chunk_id chunk, t_tsr_tile_id tile)
{
	const int		values[6][2] = {
	{-1, 3}, {1, -3}, {-4, 12}, {4, -12}, {-16, 48}, {16, -48}};
	t_tsr_chunk_id	ochunk;
	uint8_t			light;
	int				i;

	light = tsr->world.chunks[chunk].tiles[tile].light;
	i = 0;
	while (i < 6)
	{
		if ((tile >> (i & ~1) & 3) != (i % 2 * 3))
			light = max(light, tsr->world.chunks[chunk]
					.tiles[tile + values[i][0]].render_light / 2);
		else
		{
			ochunk = tsr->world.chunks[chunk].neighbors[i];
			if (ochunk)
				light = max(light, tsr->world.chunks[ochunk]
						.tiles[tile + values[i][1]].render_light / 2);
		}
		i++;
	}
	tsr->world.chunks[chunk].tiles[tile].light = light;
}

void	clear_light(t_tsr *tsr)
{
	t_tsr_tile	*tile;
	size_t		chunk;
	size_t		index;

	chunk = 1;
	while (chunk < tsr->world.chunk_count)
	{
		index = 0;
		while (index < 64)
		{
			tile = &tsr->world.chunks[chunk].tiles[index];
			tile->render_light = tile->light;
			tile->light /= 2;
			index++;
		}
		chunk++;
	}
}

void	update_light(t_tsr *tsr)
{
	t_tsr_tile		*tile;
	size_t			chunk;
	size_t			index;

	cast_illumination_rays(tsr);
	chunk = 1;
	while (chunk < tsr->world.chunk_count)
	{
		index = 0;
		while (index < 64)
		{
			tile = &tsr->world.chunks[chunk].tiles[index];
			if (tsr->world_data.tiles[tile->type].transparent)
				spread_light(tsr, chunk, index);
			index++;
		}
		chunk++;
	}
}
