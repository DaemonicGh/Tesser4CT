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

t_tsr_tile	tsr_tile(t_tsr_tile_id id, uint8_t orientation)
{
	return ((t_tsr_tile){.type = id, .orientation = orientation});
}

int	tsr_get_tile_index(t_vec3i pos)
{
	return (pos.x + pos.y * 4 + pos.z * 16);
}

bool	tsr_set_tile(const t_tsr_world *world,
	t_tsr_chunk_id chunk_id, t_vec3i pos, t_tsr_tile tile)
{
	const bool	skip = world->data->tiles[tile.type].skip;
	int			index;

	chunk_id = tsr_relocate_chunk(world, chunk_id, &pos);
	if (!chunk_id)
		return (false);
	index = tsr_get_tile_index(pos);
	world->chunks[chunk_id].tiles[index] = tile;
	if (skip)
		world->chunk_refs[chunk_id].process &= ~(1ul << index);
	else
		world->chunk_refs[chunk_id].process |= 1ul << index;
	return (true);
}

t_tsr_tile	*tsr_get_tile(
	const t_tsr_world *world, t_tsr_chunk_id chunk_id, t_vec3i pos)
{
	chunk_id = tsr_relocate_chunk(world, chunk_id, &pos);
	if (!chunk_id)
		return (&world->data->skybox);
	return (&world->chunks[chunk_id].tiles[tsr_get_tile_index(pos)]);
}
