/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 00:14:45 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/12 21:33:44 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlem.h"
#include "tsr.h"

static t_mlem_value	save_chunk_data(t_tsr *tsr, t_tsr_chunk *chunk)
{
	t_mlem_value	array;
	t_mlem_value	value;
	t_tsr_tile_id	tile;
	size_t			i;

	array = mlem_array_empty(64);
	if (!array.type)
		return ((t_mlem_value){0});
	i = 0;
	while (i < 64)
	{
		tile = chunk->tiles[i].type;
		mlem_array_append(&array, mlem_reference(
				tsr->world_data.mlem.objectv.value[tile].value));
		if (chunk->tiles[i].rotation != 24)
		{
			value = mlem_array(2, mlem_int(chunk->tiles[i].rotation >> 2),
					mlem_int(chunk->tiles[i].rotation & 3));
			if (!value.type)
			{
				mlem_destroy(array);
				return ((t_mlem_value){0});
			}
			mlem_array_append(&array, value);
		}
		i++;
	}
	return (array);
}

static void	save_chunk_attributes(
	t_tsr *tsr, t_tsr_chunk *chunk, t_mlem_value *object)
{
	const t_mlem_string	neighbor_keys[6] = {
		"west", "east", "down", "up", "north", "south"};
	const t_mlem_string	skybox_keys[6] = {
		"skybox_west", "skybox_east", "skybox_down",
		"skybox_up", "skybox_north", "skybox_south"};
	t_tsr_tile_id		skybox;
	size_t				i;

	i = 0;
	while (i < 6)
	{
		if (chunk->neighbors[i])
			mlem_object_append(object, neighbor_keys[i],
				mlem_int(chunk->neighbors[i] - 1));
		i++;
	}
	i = 0;
	while (i < 6)
	{
		skybox = chunk->limits[i].type;
		if (skybox != tsr->world_data.skybox.type)
			mlem_object_append(object, skybox_keys[i], mlem_reference(
					tsr->world_data.mlem.objectv.value[skybox].value));
		i++;
	}
}

static t_mlem_value	save_chunk(t_tsr *tsr, t_tsr_chunk *chunk)
{
	t_mlem_value		object;
	t_mlem_value		data;

	object = mlem_object_empty(13);
	if (!object.type)
		return ((t_mlem_value){0});
	data = save_chunk_data(tsr, chunk);
	if (!data.type)
	{
		mlem_destroy(object);
		return ((t_mlem_value){0});
	}
	mlem_object_append(&object, "tiles", data);
	save_chunk_attributes(tsr, chunk, &object);
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
		chunk = save_chunk(tsr, &tsr->world.chunks[i]);
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
