/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiles.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:03:41 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/28 23:53:35 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "mlem.h"
#include "tsr_textures.h"
#include <stdio.h>

static bool	get_bool(t_mlem_value object, t_mlem_string key, bool def)
{
	t_mlem_value	*value;

	value = mlem_object_get(object, key);
	if (value)
		return (mlem_dereference(*value).bool_v);
	return (def);
}

static t_tsr_texture	*get_texture(t_tsr *tsr, t_mlem_value *reference)
{
	t_mlem_value		*value;

	if (!reference)
		return (&tsr->textures.textures[0]);
	value = mlem_dereference_ptr(reference);
	value = mlem_object_get(*value, "id");
	if (!value)
		return (&tsr->textures.textures[0]);
	return (&tsr->textures.textures[value->int_v]);
}

static void	set_textures(t_tsr *tsr, t_tsr_tile *tile, t_mlem_value object)
{
	const t_mlem_string	keys[6] = {
		"west", "east", "bottom", "top", "north", "south"};
	t_mlem_value		*textures;
	t_mlem_value		*value;
	t_tsr_texture		*texture;
	int					i;

	value = mlem_object_get(object, "texture");
	if (value)
	{
		texture = get_texture(tsr, value);
		i = 0;
		while (i < 6)
			tile->texture[i++] = texture;
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
			tile->texture[i] = get_texture(tsr, value);
		i++;
	}
}

static void	load_tile(t_tsr *tsr, t_tsr_tile *tile, t_mlem_value object)
{
	const t_tsr_tile	def = {.texture = {
		&tsr->textures.textures[0], &tsr->textures.textures[0],
		&tsr->textures.textures[0], &tsr->textures.textures[0],
		&tsr->textures.textures[0], &tsr->textures.textures[0]}};

	*tile = def;
	if (object.type != MLEM_TYPE_OBJECT)
		return ;
	set_textures(tsr, tile, object);
	tile->skip = get_bool(object, "skip", def.skip);
	tile->backface = get_bool(object, "backface", def.backface);
	tile->inner_backface = get_bool(object,
			"inner_backface", def.inner_backface);
	tile->skybox = get_bool(object, "skybox", def.skybox);
	tile->specular = get_bool(object, "specular", def.specular);
	tsr->world.tile_count++;
}

void	load_tile_data(t_tsr *tsr, t_mlem_value tiles)
{
	t_mlem_value	tile;
	uint32_t		i;

	if (!tiles.type)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	i = 0;
	while (i < tiles.array_len && i < TILE_BUFFER_SIZE)
	{
		tile = mlem_dereference(tiles.array_v[i]);
		load_tile(tsr, &tsr->world.tiles[i], tile);
		i++;
	}
}
