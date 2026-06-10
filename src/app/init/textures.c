/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:44:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/29 02:48:37 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_tsr_texture	load_texture(t_tsr *tsr, t_mlem_value object)
{
	const t_tsr_texture	def = {.texture = tsr->textures._default};
	t_tsr_texture		texture;

	texture = def;
	if (object.type != MLEM_TYPE_OBJECT)
		return (texture);
	texture.flags = TX_NONE;
	texture.texture = mbx_create_region_from_file(tsr->mbx,
			mlem_object_get(object, "file")->strv.value);
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
