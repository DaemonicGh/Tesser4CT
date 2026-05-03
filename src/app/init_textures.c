/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:57:44 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/30 17:40:23 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_mbx_region	*fallback(t_tsr *tsr)
{
	if (!tsr->extras.default_region)
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLDEFIMGF);
	return (tsr->extras.default_region);
}

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
	const char	*key;
	size_t		i;
	size_t		j;

	tsr->extras.default_region = load_texture(tsr, "assets/default.png");
	tsr->ui.fonts.small = load_texture(tsr, "assets/fonts/small.png");
	tsr->ui.fonts.small->subregion_size = vec2i(5, 7);
	i = 0;
	while (i < TILE_COUNT)
	{
		j = 0;
		while (j < 6)
		{
			if (tsr->world.tiles[i].keys[j])
				key = tsr->world.tiles[i].keys[j];
			tsr->world.tiles[i].region[j] = atlas_get(&tsr->atlas, key);
			if (!tsr->world.tiles[i].region[j])
			{
				if (tsr->world.tiles[i].keys[j])
					tsr_report_m(STATUS_WARNING, REPORT_NULLIMGF,
						tsr->world.tiles[i].keys[j]);
				tsr->world.tiles[i].region[j] = fallback(tsr);
			}
			j++;
		}
		i++;
	}
}

void	init_textures(t_tsr *tsr)
{
	static const char	*path[] = {"assets/tiles/stone.png",
		"assets/tiles/dirt.png", "assets/tiles/oak_planks.png",
		"assets/tiles/iron_block.png", "assets/tiles/moss_block.png",
		"assets/tiles/blue_coral_block.png", "assets/tiles/glass.png",
		"assets/tiles/white_glass.png", "assets/tiles/grass_block_top.png",
		"assets/tiles/grass_block_side.png", "assets/tiles/oak_leaves.png",
		"assets/tiles/oak_log_side.png", "assets/tiles/oak_log_top.png",
		"assets/tiles/skybox_up.png", "assets/tiles/skybox_down.png",
		"assets/tiles/skybox_east.png", "assets/tiles/skybox_west.png",
		"assets/tiles/skybox_north.png", "assets/tiles/skybox_south.png", 0};
	static const char	*key[] = {"stone", "dirt", "oak_planks", "iron_block",
		"moss_block", "blue_coral_block", "glass", "white_glass",
		"grass_block_top", "grass_block_side", "oak_leaves", "oak_log_side",
		"oak_log_top", "skybox_up", "skybox_down", "skybox_east", "skybox_west",
		"skybox_north", "skybox_south", 0};
	size_t				i;

	i = 0;
	while (key[i] && path[i])
	{
		atlas_add(&tsr->atlas, key[i], path[i]);
		i++;
	}
	load_textures(tsr);
}
