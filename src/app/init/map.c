/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:05:49 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/01 18:05:49 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlem.h"
#include "tsr.h"

static t_vec3	get_as_vec3(
	t_mlem_value object, t_mlem_string key, t_vec3 fallback)
{
	t_mlem_value	*value;
	t_vec3			vec;

	value = mlem_object_get(object, key);
	if (!value || value->type != MLEM_TYPE_ARRAY || value->array_len != 3)
		return (fallback);
	vec.x = mlem_as_float(&value->array_v[0], fallback.x);
	vec.y = mlem_as_float(&value->array_v[1], fallback.y);
	vec.z = mlem_as_float(&value->array_v[2], fallback.z);
	return (vec);
}

static void	load_map_attributes(t_tsr *tsr, t_mlem_value object)
{
	tsr->world.origin = &tsr->world.chunks[1];
	tsr->world.skybox = (t_tsr_tile){.type = 1};
	tsr->world.spawn_pos = get_as_vec3(object, "spawn", vec3_zero());
	tsr->world.global_light_col = vec3_w(
			get_as_vec3(object, "light_col", vec3_zero()), 1);
	tsr->world.global_light = get_as_vec3(object, "light_dir", vec3(0, -1, 0));
}

void	load_map_data(t_tsr *tsr)
{
	t_mlem_value		map;
	t_mlem_value		*value;

	map = mlem_parse("maps/test.mlem", NULL, tsr->world.tiles_mlem);
	if (!map.type)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	value = mlem_object_get(map, "chunks");
	if (!value || value->type != MLEM_TYPE_ARRAY)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	load_chunk_data(tsr, *value);
	value = mlem_object_get(map, "attributes");
	if (!value || value->type != MLEM_TYPE_OBJECT)
		tsr_exit(tsr, STATUS_ERROR, NULL);
	load_map_attributes(tsr, *value);
	tsr->world.map_mlem = map;
}
