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
#include "tsr_world.h"

static t_tsr_texture	*get_texture(t_mlem_value *reference)
{
	t_mlem_value		value;

	if (!reference || reference->type != MLEM_TYPE_REFERENCE)
		return (NULL);
	value = reference->reference_v->value;
	if (!value.pointer_v)
		return (NULL);
	return (value.pointer_v);
}

static void	set_textures(t_tsr_tile_data *tile, t_mlem_value object)
{
	const t_mlem_string	keys[6] = {
		"west", "east", "bottom", "top", "north", "south"};
	t_mlem_value		*textures;
	t_tsr_texture		*texture;
	int					i;

	texture = get_texture(mlem_object_get(object, "texture"));
	if (texture)
	{
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
		texture = get_texture(mlem_object_get(*textures, keys[i]));
		if (texture)
			tile->texture[i] = texture;
		i++;
	}
}

static void	set_values(t_tsr_tile_data *tile, t_mlem_value object)
{
	t_mlem_value		*value;

	value = mlem_object_get(object, "skip");
	if (value)
		tile->skip = mlem_dereference(*value).bool_v;
	value = mlem_object_get(object, "backface");
	if (value)
		tile->backface = mlem_dereference(*value).bool_v;
	value = mlem_object_get(object, "inner_backface");
	if (value)
		tile->inner_backface = mlem_dereference(*value).bool_v;
	value = mlem_object_get(object, "skybox");
	if (value)
		tile->skybox = mlem_dereference(*value).bool_v;
	value = mlem_object_get(object, "specular");
	if (value)
		tile->specular = mlem_dereference(*value).bool_v;
}

static t_tsr_tile_data	load_tile(t_tsr *tsr, t_mlem_value object)
{
	const t_tsr_tile_data	def = {.texture = {
		&tsr->textures.textures[0], &tsr->textures.textures[0],
		&tsr->textures.textures[0], &tsr->textures.textures[0],
		&tsr->textures.textures[0], &tsr->textures.textures[0]}};
	t_tsr_tile_data			tile;

	tile = def;
	if (object.type != MLEM_TYPE_OBJECT)
		return (tile);
	set_textures(&tile, object);
	set_values(&tile, object);
	tsr->world.tile_count++;
	return (tile);
}

void	load_tile_data(t_tsr *tsr)
{
	t_mlem_value		tiles;
	t_mlem_reference	tile;
	uint32_t			i;

	tiles = mlem_parse("data/tiles.mlem", NULL, tsr->textures.textures_mlem);
	if (!tiles.type)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	i = 0;
	while (i < tiles.object_len && i < TILE_BUFFER_SIZE)
	{
		tile = tiles.object_v[i].value.reference_v;
		tsr->world.tiles[i] = load_tile(tsr, tile->value);
		mlem_destroy(tile->value);
		tile->value = mlem_int(i);
		i++;
	}
	tsr->world.tiles_mlem = tiles;
}
