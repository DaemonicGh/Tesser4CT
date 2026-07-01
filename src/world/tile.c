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

int	tsr_get_tile_index(t_vec3i pos)
{
	return (pos.x + pos.y * 4 + pos.z * 16);
}

bool	tsr_set_tile(const t_tsr_world *world,
	t_tsr_chunk_id chunk, t_vec3i pos, t_tsr_tile tile)
{
	t_vec3		dpos;
	int			index;

	dpos = vec3_vi(pos);
	chunk = tsr_relocate_chunk(world, chunk, &dpos);
	if (!chunk)
		return (false);
	pos = vec3i_vd(dpos);
	index = tsr_get_tile_index(pos);
	world->chunks[chunk].tiles[index] = tile;
	return (true);
}

t_tsr_tile	*tsr_get_tile(
	const t_tsr_world *world, t_tsr_chunk_id chunk, t_vec3i pos)
{
	t_vec3			dpos;
	t_tsr_chunk_id	reloc;
	t_vec3i			dist;
	uint			axis;

	dpos = vec3_vi(pos);
	reloc = tsr_relocate_chunk(world, chunk, &dpos);
	pos = vec3i_vd(dpos);
	if (reloc)
		return (&world->chunks[reloc].tiles[tsr_get_tile_index(pos)]);
	dist = vec3i_exec3(range_dist, pos, vec3i_i(0), vec3i_i(4));
	if (dist.z > dist.x && dist.z > dist.y)
		axis = 2;
	else
		axis = (dist.y > dist.x);
	return (&world->chunks[chunk].limits[axis * 2 + (pos.v[axis] < 2)]);
}

t_tsr_tile_data	*tsr_get_tile_data(
	const t_tsr_world *world, t_tsr_chunk_id chunk, t_vec3i pos)
{
	return (&world->data->tiles[tsr_get_tile(world, chunk, pos)->type]);
}
