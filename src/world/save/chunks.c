/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:14:45 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/10 00:14:45 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_mlem_value	save_chunk_data(t_tsr *tsr, t_tsr_chunk_id id)
{
	t_mlem_value	array;
	t_tsr_tile_id	tile;
	size_t			i;

	array = mlem_array_empty(64);
	if (!array.type)
		return ((t_mlem_value){0});
	i = 0;
	while (i < 64)
	{
		tile = tsr->world.chunks[id].tiles[i++].type;
		mlem_array_append(&array, mlem_reference(
				tsr->world_data.mlem.objectv.value[tile].value));
	}
	return (array);
}

static t_mlem_value	save_chunk(t_tsr *tsr, t_tsr_chunk_id id)
{
	const t_mlem_string	keys[6] = {
		"west", "east", "down", "up", "north", "south"};
	t_mlem_value		object;
	t_mlem_value		data;
	size_t				i;

	object = mlem_object_empty(7);
	if (!object.type)
		return ((t_mlem_value){0});
	data = save_chunk_data(tsr, id);
	if (!data.type)
	{
		mlem_destroy(object);
		return ((t_mlem_value){0});
	}
	mlem_object_append(&object, "tiles", data);
	i = 0;
	while (i < 6)
	{
		if (tsr->world.chunk_refs[id].neighbors[i])
			mlem_object_append(&object, keys[i],
				mlem_int(tsr->world.chunk_refs[id].neighbors[i] - 1));
		i++;
	}
	return (object);
}

t_mlem_value	save_chunks(t_tsr *tsr)
{
	t_mlem_value	array;
	t_mlem_value	chunk;
	size_t			i;

	array = mlem_array_empty(tsr->world.chunk_count - 1);
	if (!array.type)
		return ((t_mlem_value){0});
	i = 1;
	while (i < tsr->world.chunk_count)
	{
		chunk = save_chunk(tsr, i);
		if (!chunk.type)
		{
			mlem_destroy(array);
			return ((t_mlem_value){0});
		}
		mlem_array_append(&array, chunk);
		i++;
	}
	return (array);
}
