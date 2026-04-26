/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:57:44 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/26 16:57:44 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"

static t_mbx_region	*load_texture(t_tsr *tsr, char *path)
{
	t_mbx_region	*region;

	if (!path)
		return (NULL);
	region = mbx_make_region_from_file(tsr->mbx, path);
	if (region)
		return (region);
	if (!tsr->extras.default_region)
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLDEFIMGF);
	tsr_report_m(STATUS_WARNING, REPORT_NULLIMGF, path);
	return (tsr->extras.default_region);
}

static void	load_textures(t_tsr *tsr)
{
	static char	*paths[TILE_COUNT] = {NULL,
		"assets/tiles/stone.png", "assets/tiles/dirt.png",
		"assets/tiles/oak_plank.png", "assets/tiles/iron_block.png",
		"assets/tiles/moss_block.png", "assets/tiles/blue_coral_block.png"};
	size_t		i;

	tsr->extras.default_region = load_texture(tsr, "assets/default.png");
	tsr->ui.fonts.small = load_texture(tsr, "assets/fonts/small.png");
	tsr->ui.fonts.small->subregion_size = vec2i(5, 7);
	i = 0;
	while (i < TILE_COUNT)
	{
		tsr->world.tiles[i].region = load_texture(tsr, paths[i]);
		i++;
	}
}

void	init_textures(t_tsr *tsr)
{
	load_textures(tsr);
}
