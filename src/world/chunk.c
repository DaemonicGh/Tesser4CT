/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 01:40:56 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/04 01:40:56 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"

t_tsr_chunk	*tsr_get_chunk_neighbor(
	t_tsr *tsr, t_tsr_chunk *chunk, int side)
{
	size_t	id;

	id = tsr->world.chunk_refs[chunk->id].neighbors[side];
	if (!id)
		return (NULL);
	return (&tsr->world.chunks[id]);
}

t_tsr_chunk_id	tsr_relocate_chunk(
	const t_tsr_world *world, t_tsr_chunk_id chunk_id, t_vec3i *pos)
{
	int	axis;
	int	side;

	axis = 0;
	side = 0;
	while (axis <= 2)
	{
		while (pos->v[axis] < 0)
		{
			chunk_id = world->chunk_refs[chunk_id].neighbors[side];
			if (!chunk_id)
				return (0);
			pos->v[axis] += 4;
		}
		while (pos->v[axis] >= 4)
		{
			chunk_id = world->chunk_refs[chunk_id].neighbors[side + 1];
			if (!chunk_id)
				return (0);
			pos->v[axis] -= 4;
		}
		axis++;
		side += 2;
	}
	return (chunk_id);
}

static void	copy_chunks(t_tsr_world *src, t_tsr_world *dest)
{
	size_t	i;

	i = 0;
	while (i < src->chunk_count)
	{
		dest->chunks[i] = src->chunks[i];
		dest->chunk_refs[i] = src->chunk_refs[i];
		i++;
	}
	while (i < dest->chunk_capacity)
	{
		dest->chunks[i] = src->chunks[0];
		dest->chunks[i].id = i;
		dest->chunk_refs[i] = src->chunk_refs[0];
		i++;
	}
}

t_tsr_chunk_id	tsr_new_chunk(t_tsr_world *world)
{
	t_tsr_world		new;

	if (world->chunk_count == world->chunk_capacity)
	{
		new = *world;
		new.chunk_capacity = world->chunk_capacity * 1.5 + 1;
		new.chunks = malloc(new.chunk_capacity * sizeof(t_tsr_chunk));
		new.chunk_refs = malloc(new.chunk_capacity * sizeof(t_tsr_chunk_ref));
		if (!new.chunks || !new.chunk_refs)
		{
			free(new.chunks);
			free(new.chunk_refs);
			return (0);
		}
		copy_chunks(world, &new);
		*world = new;
	}
	return (world->chunk_count++);
}
