/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 23:15:39 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/01 23:15:39 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	load_chunk_tiles(t_tsr_chunk *chunk, t_mlem_value array)
{
	int			tile;
	t_mlem_int	count;
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = 0;
	tile = 0;
	count = 0;
	while (i < array.array_len)
	{
		if (array.array_v[i].type == MLEM_TYPE_INT)
			count = array.array_v[i].int_v - 1;
		else
		{
			tile = array.array_v[i].reference_v->value.int_v;
			count = 1;
		}
		while (count-- > 0 && j < (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE))
			chunk->tiles[j++] = tile;
		i++;
	}
}

static t_tsr_chunk	load_chunk(t_tsr *tsr, t_mlem_value object)
{
	const t_mlem_string	keys[6] = {
		"west", "east", "down", "up", "north", "south"};
	t_tsr_chunk			chunk;
	t_mlem_value		*value;
	uint32_t			i;

	chunk = (t_tsr_chunk){0};
	i = 0;
	while (i < 6)
	{
		value = mlem_object_get(object, keys[i]);
		if (value)
			chunk.neighbors[i] = &tsr->world.chunks[value->int_v];
		i++;
	}
	value = mlem_object_get(object, "tiles");
	if (value)
		load_chunk_tiles(&chunk, *value);
	return (chunk);
}

void	load_chunk_data(t_tsr *tsr, t_mlem_value array)
{
	t_mlem_value	chunk;
	uint32_t		i;

	tsr->world.chunks = malloc(sizeof(t_tsr_chunk) * array.array_len);
	if (!tsr->world.chunks)
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
	i = 0;
	while (i < array.array_len)
	{
		chunk = array.array_v[i];
		tsr->world.chunks[i] = load_chunk(tsr, chunk);
		i++;
	}
}
