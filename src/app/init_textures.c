/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 16:57:44 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/13 18:55:03 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_pbr	fallback(t_tsr *tsr)
{
	if (!tsr->textures._default)
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLDEFIMGF);
	return ((t_pbr){tsr->textures._default, NULL});
}

static t_mbx_region	*load_texture(t_tsr *tsr, char *path)
{
	t_mbx_region	*region;

	if (!path)
		return (NULL);
	region = mbx_make_region_from_file(tsr->mbx, path);
	if (region)
		return (region);
	if (!tsr->textures._default)
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLDEFIMGF);
	tsr_report_m(STATUS_WARNING, REPORT_NULLIMGF, path);
	return (tsr->textures._default);
}

static void	load_textures(t_tsr *tsr)
{
	const char	*key;
	size_t		i;
	size_t		j;
	t_pbr		*pbr;

	tsr->textures._default = load_texture(tsr, "assets/default.png");
	tsr->textures.font_small = load_texture(tsr, "assets/fonts/small.png");
	tsr->textures.font_small->subregion_size = vec2i(5, 7);
	tsr->textures.font_title = load_texture(tsr, "assets/fonts/title.png");
	tsr->textures.font_title->subregion_size = vec2i(10, 12);
	tsr->textures.tile_highlight = load_texture(
			tsr, "assets/tile_highlight.png");
	tsr->textures.tile_face_highlight = load_texture(
			tsr, "assets/tile_face_highlight.png");
	tsr->textures.hotbar_selection = load_texture(
			tsr, "assets/hotbar_selection.png");
	i = 0;
	key = NULL;
	while (i < TILE_BUFFER_COUNT)
	{
		j = 0;
		while (j < 6)
		{
			if (tsr->world.tiles[i].keys[j])
				key = tsr->world.tiles[i].keys[j];
			pbr = atlas_get(&tsr->atlas, key);
			if (!pbr)
			{
				if (tsr->world.tiles[i].keys[j])
					tsr_report_m(STATUS_WARNING, REPORT_NULLIMGF,
						tsr->world.tiles[i].keys[j]);
				tsr->world.tiles[i].pbr[j] = fallback(tsr);
			}
			else
				tsr->world.tiles[i].pbr[j] = *pbr;
			j++;
		}
		i++;
	}
}

void	init_textures(t_tsr *tsr)
{
	static const char	*path[] = {"assets/tiles/stone.png",
		"assets/tiles/dirt.png", "assets/tiles/oak_planks.png",
		"assets/tiles/iron_block.png", "assets/tiles/moss.png",
		"assets/tiles/blue_coral_block.png", "assets/tiles/glass.png",
		"assets/tiles/white_stained_glass.png",
		"assets/tiles/grass_block_top.png",
		"assets/tiles/grass_block_side.png", "assets/tiles/oak_leaves.png",
		"assets/tiles/oak_log.png", "assets/tiles/oak_log_top.png",
		"assets/tiles/skybox_up.png", "assets/tiles/skybox_down.png",
		"assets/tiles/skybox_east.png", "assets/tiles/skybox_west.png",
		"assets/tiles/skybox_north.png", "assets/tiles/skybox_south.png",
		"assets/tiles/red_stained_glass.png",
		"assets/tiles/green_stained_glass.png",
		"assets/tiles/blue_stained_glass.png", 
		"assets/tiles/stone_bricks.png", "assets/tiles/cobblestone.png", 0};
	size_t				i;

	i = 0;
	while (path[i])
		atlas_add(&tsr->atlas, path[i++]);
	load_textures(tsr);
}
