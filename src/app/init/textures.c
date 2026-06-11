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
#include "mlem.h"
#include "tsr_textures.h"

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
	if (value && value->type == MLEM_TYPE_BOOL && value->bool_v)
		texture.flags |= TX_NORMAL;
	value = mlem_object_get(object, "property");
	if (value && value->type == MLEM_TYPE_BOOL && value->bool_v
			&& (texture.flags & TX_NORMAL))
		texture.flags |= TX_PROPERTY;
	texture.texture = mbx_create_region_from_file(tsr->mbx,
			mlem_object_get(object, "file")->string_v);
	tsr->textures.count++;
	return (texture);
}

static void	setup_texture(t_tsr *tsr, t_mlem_value textures, uint32_t i)
{
	char	*allocd_id_key;

	allocd_id_key = ft_strdup("id");
	if (!allocd_id_key
		|| !mlem_object_append(&textures.object_v[i].value,
			allocd_id_key, mlem_int(i)))
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
}

void	load_texture_data(t_tsr *tsr, t_mlem_value textures)
{
	t_mlem_value	*texture;
	uint32_t		i;

	if (!textures.type)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	i = 0;
	while (i < textures.object_len && i < TEXTURE_BUFFER_SIZE)
	{
		texture = mlem_dereference_ptr(&textures.object_v[i].value);
		setup_texture(tsr, textures, i);
		tsr->textures.textures[i] = load_texture(tsr, *texture);
		i++;
	}
}
