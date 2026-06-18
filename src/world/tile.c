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

int	tsr_get_tile_index(t_vec3i pos)
{
	return (pos.x + pos.y * 4 + pos.z * 16);
}

bool	tsr_set_tile(const t_tsr_world *world,
	t_tsr_chunk_id chunk, t_vec3i pos, t_tsr_tile tile)
{
	int			index;

	chunk = tsr_relocate_chunk(world, chunk, &pos);
	if (!chunk)
		return (false);
	index = tsr_get_tile_index(pos);
	world->chunks[chunk].tiles[index] = tile;
	return (true);
}

t_tsr_tile	*tsr_get_tile(
	const t_tsr_world *world, t_tsr_chunk_id chunk, t_vec3i pos)
{
	chunk = tsr_relocate_chunk(world, chunk, &pos);
	if (!chunk)
		return (&world->data->skybox);
	return (&world->chunks[chunk].tiles[tsr_get_tile_index(pos)]);
}
