/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiles.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:03:41 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/21 16:03:41 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "mlem.h"

static bool	get_bool(t_mlem_value object, t_mlem_string key, bool def)
{
	t_mlem_value	*value;

	value = mlem_object_get(object, key);
	if (value)
		return (mlem_dereference(*value).bool_v);
	return (def);
}

static void set_textures(t_tsr_tile *tile, t_mlem_value object)
{
	const t_mlem_string	keys[6] = {
		"west", "east", "bottom", "top", "north", "south"};
	t_mlem_value		*textures;
	t_mlem_value		*value;
	int					i;

	value = mlem_object_get(object, "texture");
	if (value)
	{
		value = mlem_dereference_ptr(value);
		i = 0;
		while (i < 6)
			tile->keys[i++] = value->string_v;
	}
	textures = mlem_object_get(object, "textures");
	if (!textures)
		return ;
	textures = mlem_dereference_ptr(textures);
	i = 0;
	while (i < 6)
	{
		value = mlem_object_get(*textures, keys[i]);
		if (value)
			tile->keys[i] = value->string_v;
		i++;
	}
}

static void	load_tile(t_tsr_tile *tile, t_mlem_value object)
{
	const t_tsr_tile	def = {0};

	if (object.type != MLEM_TYPE_OBJECT)
	{
		*tile = def;
		return ;
	}
	set_textures(tile, object);
	tile->skip = get_bool(object, "skip", def.skip);
	tile->backface = get_bool(object, "backface", def.backface);
	tile->inner_backface = get_bool(object, "inner_backface", def.inner_backface);
	tile->skybox = get_bool(object, "skybox", def.skybox);
	tile->specular = get_bool(object, "specular", def.specular);
}

void	load_tile_data(t_tsr *tsr)
{
	t_mlem_value	tiles;
	t_mlem_value	tile;
	uint32_t		i;

	tiles = mlem_parse("data/tiles.mlem", NULL, (t_mlem_value){0});
	if (!tiles.type)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	i = 0;
	while (i < tiles.array_len && i < TILE_BUFFER_COUNT)
	{
		tile = mlem_dereference(tiles.array_v[i]);
		load_tile(&tsr->world.tiles[i], tile);
		i++;
	}
}
