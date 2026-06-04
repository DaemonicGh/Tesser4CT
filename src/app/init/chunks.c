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
#include "tsr_world.h"

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
		while (count-- > 0 && j < 64)
			chunk->tiles[j++].type = tile;
		i++;
	}
}

static t_tsr_chunk	load_chunk(t_tsr *tsr, t_mlem_value object)
{
	t_tsr_chunk			chunk;
	t_mlem_value		*value;

	(void)tsr;
	chunk = (t_tsr_chunk){0};
	value = mlem_object_get(object, "tiles");
	if (value)
		load_chunk_tiles(&chunk, *value);
	return (chunk);
}

static t_tsr_chunk_ref	load_chunk_ref(
	t_tsr *tsr, t_mlem_value object, t_tsr_chunk *chunk)
{
	const t_mlem_string	keys[6] = {
		"west", "east", "down", "up", "north", "south"};
	t_tsr_chunk_ref		chunk_ref;
	t_mlem_value		*value;
	uint32_t			i;

	i = 0;
	while (i < 6)
	{
		value = mlem_object_get(object, keys[i]);
		if (value)
			chunk_ref.neighbors[i] = value->int_v + 1;
		i++;
	}
	i = 64;
	chunk_ref.process = 0;
	while (i--)
	{
		chunk_ref.process <<= 1;
		chunk_ref.process |= !tsr->world.tiles[chunk->tiles[i].type].skip;
	}
	return (chunk_ref);
}

void	load_chunk_data(t_tsr *tsr, t_mlem_value array)
{
	t_mlem_value	chunk;
	uint32_t		i;

	tsr->world.chunks = malloc(
			sizeof(t_tsr_chunk) * (array.array_len + 1));
	tsr->world.chunk_refs = malloc(
			sizeof(t_tsr_chunk_ref) * (array.array_len + 1));
	if (!tsr->world.chunks || !tsr->world.chunk_refs)
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
	tsr->world.chunks[0] = (t_tsr_chunk){0};
	tsr->world.chunk_refs[0] = (t_tsr_chunk_ref){0};
	i = 0;
	while (i < array.array_len)
	{
		chunk = array.array_v[i++];
		tsr->world.chunks[i] = load_chunk(tsr, chunk);
		tsr->world.chunks[i].id = i;
		tsr->world.chunk_refs[i] = load_chunk_ref(
				tsr, chunk, &tsr->world.chunks[i]);
	}
}
