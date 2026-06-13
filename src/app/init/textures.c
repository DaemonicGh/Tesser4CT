/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:44:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/10 11:25:37 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"

static t_tsr_texture	load_texture(t_tsr *tsr, t_mlem_value object)
{
	const t_tsr_texture	def = {.texture = tsr->textures._default};
	t_tsr_texture		texture;
	t_mlem_value		*value;

	texture = def;
	if (object.type != MLEM_TYPE_OBJECT)
		return (texture);
	texture.flags = TX_NONE;
	value = mlem_object_get(object, "normal");
	if (value && value->type == MLEM_TYPE_BOOL && value->boolv.value)
		texture.flags |= TX_NORMAL;
	value = mlem_object_get(object, "property");
	if (value && value->type == MLEM_TYPE_BOOL && value->boolv.value
		&& (texture.flags & TX_NORMAL))
		texture.flags |= TX_PROPERTY;
	texture.texture = mbx_create_region_from_file(tsr->mbx,
			mlem_object_get(object, "file")->strv.value);
	texture.texture->subregion_size = vec2i(texture.texture->size.x,
			texture.texture->size.y / (
				1 + (bool)(texture.flags & TX_NORMAL)
				+ (bool)(texture.flags & TX_PROPERTY)));
	tsr->textures.count++;
	return (texture);
}

void	load_texture_data(t_tsr *tsr)
{
	t_mlem_value		textures;
	t_mlem_reference	texture;
	uint32_t			i;

	textures = mlem_parse("data/textures.mlem", NULL, (t_mlem_value){0});
	if (!textures.type)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	i = 0;
	while (i < textures.objectv.len && i < TEXTURE_BUFFER_SIZE)
	{
		texture = textures.objectv.value[i].value.refv.value;
		tsr->textures.textures[i] = load_texture(tsr, texture->value);
		mlem_destroy(texture->value);
		texture->value = mlem_raw_value(MLEM_TYPE_USER_POINTER,
				(uint64_t) & tsr->textures.textures[i]);
		i++;
	}
	tsr->textures.mlem = textures;
}
