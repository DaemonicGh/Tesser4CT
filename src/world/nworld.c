/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nworld.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 02:07:10 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/02 02:07:10 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

t_tsr_chunk	*tsr_offset_chunk(t_tsr_chunk *chunk, t_vec3i offset)
{
	int	axis;
	int	side;

	axis = 0;
	side = 0;
	while (axis < 2)
	{
		while (offset.v[axis] < 0)
		{
			chunk = chunk->neighbors[side];
			if (!chunk)
				return (NULL);
			offset.v[axis] += CHUNK_SIZE;
		}
		while (offset.v[axis] >= CHUNK_SIZE)
		{
			chunk = chunk->neighbors[side + 1];
			if (!chunk)
				return (NULL);
			offset.v[axis] -= CHUNK_SIZE;
		}
		axis++;
		side += 2;
	}
	return (chunk);
}

t_tsr_tile_id	*tsr_get_tile_ptr(t_tsr_chunk *chunk, t_vec3i pos)
{
	return (&chunk->tiles[
			pos.x + pos.y * CHUNK_SIZE + pos.z * CHUNK_SIZE * CHUNK_SIZE]);
}

bool	tsr_fix_tile_pos(t_tsr_chunk **chunk, t_vec3i *pos)
{
	*chunk = tsr_offset_chunk(*chunk, *pos);
	if (!(*chunk))
		return (false);
	*pos = vec3i_exec3(wrap, *pos, vec3i_i(0), vec3i_i(CHUNK_SIZE));
	return (true);
}

t_tsr_tile	*tsr_get_tile(t_tsr *tsr, t_tsr_chunk *chunk, t_vec3i pos)
{
	chunk = tsr_offset_chunk(chunk, pos);
	if (!chunk)
		return (tsr->world.skybox);
	return (&tsr->world.tiles[*tsr_get_tile_ptr(chunk,
				vec3i_exec3(wrap, pos, vec3i_i(0), vec3i_i(CHUNK_SIZE)))]);
}
