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

t_tsr_chunk_id	tsr_relocate_chunk(
	const t_tsr_world *world, t_tsr_chunk_id chunk, t_vec3 *pos)
{
	int	axis;
	int	side;

	axis = 0;
	side = 0;
	while (axis <= 2)
	{
		while (pos->v[axis] < 0)
		{
			chunk = world->chunks[chunk].neighbors[side];
			if (!chunk)
				return (0);
			pos->v[axis] += 4;
		}
		while (pos->v[axis] >= 4)
		{
			chunk = world->chunks[chunk].neighbors[side + 1];
			if (!chunk)
				return (0);
			pos->v[axis] -= 4;
		}
		axis++;
		side += 2;
	}
	return (chunk);
}

t_tsr_chunk_id	tsr_new_chunk(t_tsr_world *world)
{
	t_tsr_world		new;
	size_t			i;

	if (world->chunk_count != world->chunk_capacity)
		return (world->chunk_count++);
	new = *world;
	new.chunk_capacity = world->chunk_capacity * 1.5 + 1;
	new.chunks = malloc(new.chunk_capacity * sizeof(t_tsr_chunk));
	if (!new.chunks)
	{
		free(new.chunks);
		return (0);
	}
	i = 0;
	while (i < world->chunk_count)
	{
		new.chunks[i] = world->chunks[i];
		i++;
	}
	while (i < new.chunk_capacity)
		new.chunks[i++] = world->chunks[0];
	*world = new;
	return (world->chunk_count++);
}
