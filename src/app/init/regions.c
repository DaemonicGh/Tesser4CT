/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:57:44 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/29 00:52:24 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_mbx_region	*load_texture(t_tsr *tsr, char *path)
{
	t_mbx_region	*region;

	if (!path)
		return (NULL);
	region = mbx_create_region_from_file(tsr->mbx, path);
	if (!region)
	{
		tsr_report_m(STATUS_WARNING, REPORT_NULLIMGF, path);
		return (tsr->textures.default_region);
	}
	return (region);
}

void	load_regions(t_tsr *tsr)
{
	tsr->textures.default_region = load_texture(tsr,
			"assets/textures/default.png");
	if (!tsr->textures.default_region)
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLDEFIMGF);
	tsr->textures.font_small = load_texture(tsr,
			"assets/textures/fonts/small.png");
	tsr->textures.font_small->subregion_size = vec2i(5, 7);
	tsr->textures.font_title = load_texture(tsr,
			"assets/textures/fonts/title.png");
	tsr->textures.font_title->subregion_size = vec2i(10, 12);
	tsr->textures.tile_highlight = load_texture(tsr,
			"assets/textures/tile_highlight.png");
	tsr->textures.tile_face_highlight = load_texture(tsr,
			"assets/textures/tile_face_highlight.png");
	tsr->textures.hotbar_selection = load_texture(tsr,
			"assets/textures/hotbar_selection.png");
}
