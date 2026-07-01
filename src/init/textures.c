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

#include ".mlem_values.h"
#include "modules/mbx_drawing.h"
#include "modules/mbx_structs.h"
#include "tsr.h"
#include "tsr_core.h"
#include "tsr_utils.h"

static t_mbx_color	color_average_4(
	t_mbx_color col1, t_mbx_color col2, t_mbx_color col3, t_mbx_color col4)
{
	uint8_t	a;

	a = (col1.a + col2.a + col3.a + col4.a) / 4;
	if (a < 128)
		a = 0;
	else
		a = 255;
	return ((t_mbx_color){
		.r = (col1.r + col2.r + col3.r + col4.r) / 4,
		.g = (col1.g + col2.g + col3.g + col4.g) / 4,
		.b = (col1.b + col2.b + col3.b + col4.b) / 4,
		.a = a,
	});
}

static void	fill_mipmap(t_mbx_atlas *src, t_mbx_atlas *mipmap)
{
	t_vec2i		pos;
	t_vec2i		mipmap_pos;
	t_mbx_color	col;

	mipmap_pos.x = 0;
	while (mipmap_pos.x < mipmap->size.x)
	{
		pos.x = mipmap_pos.x * 2;
		mipmap_pos.y = 0;
		while (mipmap_pos.y < mipmap->size.y)
		{
			pos.y = mipmap_pos.y * 2;
			col = color_average_4(
					mbx_get_pixel_raw(src, pos),
					mbx_get_pixel_raw_xy(src, pos.x + 1, pos.y),
					mbx_get_pixel_raw_xy(src, pos.x, pos.y + 1),
					mbx_get_pixel_raw_xy(src, pos.x + 1, pos.y + 1));
			mbx_set_pixel_raw(mipmap, mipmap_pos, col);
			mipmap_pos.y++;
		}
		mipmap_pos.x++;
	}
}

static void	generate_mipmaps(t_tsr *tsr, t_tsr_texture *texture)
{
	size_t	i;

	i = 1;
	while (i < MIPMAP_LEVELS)
	{
		if (texture->texture[i - 1]->size.x < 2
			|| texture->texture[i - 1]->size.y < 2)
		{
			texture->texture[i] = texture->texture[i - 1];
			i++;
			continue ;
		}
		texture->texture[i] = mbx_create_region(tsr->mbx,
				vec2i_div_d(texture->texture[i - 1]->size, 2));
		if (!texture->texture[i])
			tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
		texture->texture[i]->subregion_size = vec2i_div_d(
				texture->texture[i - 1]->subregion_size, 2);
		fill_mipmap(texture->texture[i - 1], texture->texture[i]);
		i++;
	}
}

static t_tsr_texture	load_texture(
	t_tsr *tsr, t_mlem_string name, t_mlem_value object)
{
	t_tsr_texture		texture;
	t_mlem_value		*value;

	if (object.type != MLEM_TYPE_OBJECT)
		return (tsr->textures.textures[0]);
	value = mlem_object_get(object, "file");
	if (!value || value->type != MLEM_TYPE_STRING)
	{
		tsr_report_m(STATUS_WARNING, "Invalid path for", name);
		return (tsr->textures.textures[0]);
	}
	texture.texture[0] = load_region(tsr, value->strv.value);
	if (texture.texture[0] == tsr->textures.default_region)
		return (tsr->textures.textures[0]);
	texture.flags = TX_NONE;
	if (mlem_as_bool(mlem_object_get(object, "normal"), false))
		texture.flags |= TX_NORMAL;
	if (mlem_as_bool(mlem_object_get(object, "pbr"), false)
		&& (texture.flags & TX_NORMAL))
		texture.flags |= TX_PBR;
	texture.texture[0]->subregion_size = vec2i_i(texture.texture[0]->size.y
			/ (1 + (bool)(texture.flags & (TX_NORMAL | TX_BUMP))
				+ (bool)(texture.flags & TX_PBR)));
	generate_mipmaps(tsr, &texture);
	texture.anim_speed = mlem_as_float(
			mlem_object_get(object, "animation_speed"), 0);
	return (texture);
}

void	load_texture_data(t_tsr *tsr)
{
	t_mlem_value		textures;
	t_mlem_reference	texture;
	uint32_t			i;

	textures = mlem_parse(TSR_ROOT "data/textures.mlem",
			NULL, (t_mlem_value){0});
	if (!textures.type)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	i = 0;
	tsr->textures.textures[0] = (t_tsr_texture){
		.texture[0] = tsr->textures.default_region};
	generate_mipmaps(tsr, &tsr->textures.textures[0]);
	while (i < textures.objectv.len && i < TEXTURE_BUFFER_SIZE)
	{
		texture = textures.objectv.value[i].value.refv.value;
		tsr->textures.textures[i] = load_texture(
				tsr, texture->name, texture->value);
		tsr->textures.count++;
		mlem_destroy(texture->value);
		texture->value = mlem_raw_value(MLEM_TYPE_USER_POINTER,
				(uint64_t) & tsr->textures.textures[i]);
		i++;
	}
	tsr->textures.mlem = textures;
}
