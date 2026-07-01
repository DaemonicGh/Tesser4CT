/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 00:47:21 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/20 00:47:21 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_handlers.h"
#include "modules/mbx_structs.h"
#include "tsr.h"
#include "tsr_utils.h"

t_vec3	get_as_vec3(
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

t_mbx_region	*load_region(t_tsr *tsr, const char *path)
{
	const char			fpath[256] = TSR_ROOT "data/textures/";
	const size_t		fpath_len = tsr_strlen(fpath);
	t_mbx_region		*region;

	if (tsr_strlen(path) >= 256 - fpath_len)
	{
		tsr_report_m(STATUS_WARNING, "Region path too long : ", path);
		return (tsr->textures.default_region);
	}
	tsr_strcpy((char *)fpath + fpath_len, path);
	region = mbx_create_region_from_file(tsr->mbx, (char *)fpath);
	if (!region)
	{
		tsr_report_m(STATUS_WARNING, REPORT_NULLIMGF, path);
		return (tsr->textures.default_region);
	}
	return (region);
}

t_mbx_atlas	*load_atlas(t_tsr *tsr, const char *path, t_vec2i subsize)
{
	t_mbx_region	*region;

	region = load_region(tsr, path);
	if (region != tsr->textures.default_region)
		region = mbx_atlas(region, subsize);
	return (region);
}
