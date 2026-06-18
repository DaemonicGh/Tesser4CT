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

#include "mlem.h"
#include "tsr.h"

static void	load_chunk_tiles(t_tsr_chunk *chunk, t_mlem_value array)
{
	int			tile;
	t_mlem_int	count;
	uint8_t		rotation;
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = 0;
	tile = 0;
	count = 0;
	rotation = 12;
	while (i < array.arrayv.len)
	{
		if (array.arrayv.value[i].type == MLEM_TYPE_INT)
			count = array.arrayv.value[i].intv.value - 1;
		else if (array.arrayv.value[i].type == MLEM_TYPE_ARRAY)
			rotation
				= mlem_as_int(mlem_array_get(array.arrayv.value[i], 0), 2) << 2
				| mlem_as_int(mlem_array_get(array.arrayv.value[i], 1), 0);
		else
		{
			while (count-- > 0 && j < 64)
				chunk->tiles[j++] = tsr_tile_r(tile, rotation);
			tile = array.arrayv.value[i].refv.value->value.intv.value;
			rotation = 12;
			count = 1;
		}
		i++;
	}
}

static void	load_chunk_neighbors(t_mlem_value object, t_tsr_chunk *chunk)
{
	const t_mlem_string	keys[6] = {
		"west", "east", "down", "up", "north", "south"};
	t_mlem_value		*value;
	uint32_t			i;

	i = 0;
	while (i < 6)
	{
		value = mlem_object_get(object, keys[i]);
		if (value)
			chunk->neighbors[i] = value->intv.value + 1;
		i++;
	}
}

static t_tsr_chunk	load_chunk(t_tsr *tsr, t_mlem_value object)
{
	const t_mlem_string	keys[6] = {
		"skybox_west", "skybox_east", "skybox_down",
		"skybox_up", "skybox_north", "skybox_south"};
	t_tsr_chunk			chunk;
	t_mlem_value		*value;
	int					i;

	(void)tsr;
	chunk = (t_tsr_chunk){0};
	i = 0;
	while (i < 6)
	{
		value = mlem_object_get(object, keys[i]);
		if (value)
			chunk.limits[i].type = value->refv.value->value.intv.value;
		else
			chunk.limits[i] = tsr->world_data.skybox;
		i++;
	}
	value = mlem_object_get(object, "tiles");
	if (value)
		load_chunk_tiles(&chunk, *value);
	load_chunk_neighbors(object, &chunk);
	return (chunk);
}

void	load_chunk_data(t_tsr *tsr, t_mlem_value array)
{
	t_mlem_value	chunk;
	uint32_t		i;

	tsr->world.chunk_count = array.arrayv.len + 1;
	tsr->world.chunk_capacity = tsr->world.chunk_count;
	tsr->world.chunks = malloc(
			sizeof(t_tsr_chunk) * tsr->world.chunk_capacity);
	if (!tsr->world.chunks)
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
	tsr->world.chunks[0] = (t_tsr_chunk){0};
	i = 0;
	while (i < 6)
		tsr->world.chunks[0].limits[i++] = tsr->world_data.skybox;
	i = 0;
	while (i < array.arrayv.len)
	{
		chunk = array.arrayv.value[i++];
		tsr->world.chunks[i] = load_chunk(tsr, chunk);
	}
}
