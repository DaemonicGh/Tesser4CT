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

#include "tsr.h"

static t_vec3	get_as_vec3(
	t_mlem_value object, t_mlem_string key, t_vec3 fallback)
{
	t_mlem_value	*value;
	t_vec3			vec;
	size_t			i;

	value = mlem_object_get(object, key);
	if (!value || value->type != MLEM_TYPE_ARRAY || value->arrayv.len > 3)
		return (fallback);
	i = 0;
	vec = fallback;
	while (i < value->arrayv.len)
	{
		vec.v[i] = mlem_as_float(&value->arrayv.value[i], fallback.v[i]);
		i++;
	}
	return (vec);
}

static void	load_map_attributes(t_tsr *tsr, t_mlem_value object)
{
	t_mlem_value	*value;

	tsr->world.data = &tsr->world_data;
	tsr->world_data.origin = mlem_as_int(
			mlem_object_get(object, "origin"), 0) + 1;
	value = mlem_object_get(object, "skybox");
	if (value && value->type == MLEM_TYPE_REFERENCE)
		tsr->world_data.skybox = tsr_tile(mlem_as_int(
					&value->refv.value->value, 1), 3, 0);
	else
		tsr->world_data.skybox = tsr_tile(1, 3, 0);
	tsr->player.chunk = mlem_as_int(mlem_object_get(object, "player_chunk"),
			tsr->world_data.origin - 1) + 1;
	tsr->player.position = get_as_vec3(
			object, "player_position", vec3_d(1.01));
	tsr->camera.rotation = get_as_vec3(
			object, "player_rotation", vec3_d(0));
	tsr->player.rotation.x = tsr->camera.rotation.x;
	tsr->world_data.skylight_color = get_as_vec3(
			object, "skylight_color", vec3_d(1));
	tsr->world_data.skylight = vec3_normalize(vec3_neg(get_as_vec3(
					object, "skylight_direction", vec3(0.01, -0.99, 0.01))));
	tsr->world_data.shadow_color = get_as_vec3(
			object, "shadow_color", vec3_d(0.1));
}

static void	setup_name(t_tsr *tsr, char *name)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (j < MAP_SELECT_PREFIX_LEN)
		name[i++] = MAP_SELECT_PREFIX[j++];
	j = 0;
	while (tsr->ui.prompt.buffer[j])
		name[i++] = tsr->ui.prompt.buffer[j++];
	j = 0;
	while (j < MAP_SELECT_SUFFIX_LEN)
		name[i++] = MAP_SELECT_SUFFIX[j++];
	name[i] = 0;
}

static bool	load_map_data(t_tsr *tsr, t_mlem_value map)
{
	t_mlem_value		*value;

	value = mlem_object_get(map, "map_id");
	if (!value || value->type != MLEM_TYPE_STRING)
		return (false);
	free(tsr->world_data.name);
	tsr->world_data.name = value->strv.value;
	value->strv.value = NULL;
	value = mlem_object_get(map, "attributes");
	if (!value || value->type != MLEM_TYPE_OBJECT)
		return (false);
	load_map_attributes(tsr, *value);
	value = mlem_object_get(map, "chunks");
	if (!value || value->type != MLEM_TYPE_ARRAY)
		return (false);
	load_chunk_data(tsr, *value);
	return (true);
}

bool	load_map(t_tsr *tsr)
{
	char				name[PROMPT_SIZE
		+ MAP_SELECT_PREFIX_LEN + MAP_SELECT_SUFFIX_LEN];
	t_mlem_value		map;

	setup_name(tsr, name);
	map = mlem_parse(name, NULL, tsr->world_data.mlem);
	if (!map.type)
		return (false);
	load_map_data(tsr, map);
	mlem_destroy(map);
	return (true);
}
