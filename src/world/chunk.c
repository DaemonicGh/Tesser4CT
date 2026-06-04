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

t_tsr_chunk	*tsr_get_chunk_neighbor(
	t_tsr *tsr, t_tsr_chunk *chunk, int side)
{
	return (
		&tsr->world.chunks[
			tsr->world.chunk_refs[chunk->id].neighbors[side]
		]
	);
}

t_tsr_chunk	*tsr_relocate_chunk(
	t_tsr *tsr, t_tsr_chunk *chunk, t_vec3i *pos)
{
	int	axis;
	int	side;

	axis = 0;
	side = 0;
	while (axis <= 2)
	{
		while (pos->v[axis] < 0)
		{
			chunk = tsr_get_chunk_neighbor(tsr, chunk, side);
			if (!chunk)
				return (NULL);
			pos->v[axis] += 4;
		}
		while (pos->v[axis] >= 4)
		{
			chunk = tsr_get_chunk_neighbor(tsr, chunk, side + 1);
			if (!chunk)
				return (NULL);
			pos->v[axis] -= 4;
		}
		axis++;
		side += 2;
	}
	return (chunk);
}
