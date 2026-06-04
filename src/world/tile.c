/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tile.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 01:40:51 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/04 01:40:51 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"

t_tsr_tile	tsr_tile(t_tsr_tile_id id, uint8_t orientation)
{
	return ((t_tsr_tile){.type = id, .orientation = orientation});
}

int	tsr_get_tile_index(t_vec3i pos)
{
	return (pos.x + pos.y * 4 + pos.z * 16);
}

bool	tsr_set_tile(
	t_tsr *tsr, t_tsr_chunk *chunk, t_vec3i pos, t_tsr_tile tile)
{
	const bool	skip = tsr->world.tiles[tile.type].skip;
	int			index;

	chunk = tsr_relocate_chunk(tsr, chunk, &pos);
	if (!chunk)
		return (false);
	index = tsr_get_tile_index(pos);
	chunk->tiles[index] = tile;
	if (skip)
		tsr->world.chunk_refs[chunk->id].process &= ~(1ul << index);
	else
		tsr->world.chunk_refs[chunk->id].process |= 1ul << index;
	return (true);
}

t_tsr_tile	*tsr_get_tile(
	t_tsr *tsr, t_tsr_chunk *chunk, t_vec3i pos)
{
	chunk = tsr_relocate_chunk(tsr, chunk, &pos);
	if (!chunk)
		return (&tsr->world.skybox);
	return (&chunk->tiles[tsr_get_tile_index(pos)]);
}
