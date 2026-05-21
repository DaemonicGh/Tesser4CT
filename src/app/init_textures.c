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

static t_tsr_texture	fallback(t_tsr *tsr)
{
	if (!tsr->textures._default)
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLDEFIMGF);
	return ((t_tsr_texture){tsr->textures._default, NULL});
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
	size_t			i;
	size_t			j;
	t_tsr_texture	*texture;

	tsr->textures._default = load_texture(tsr,
			"assets/textures/default.png");
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
	i = 0;
	while (i < TILE_BUFFER_COUNT)
	{
		j = 0;
		while (j < 6)
		{
			texture = NULL;
			if (tsr->world.tiles[i].keys[j])
				texture = atlas_get(&tsr->atlas, tsr->world.tiles[i].keys[j]);
			if (!texture)
			{
				if (tsr->world.tiles[i].keys[j])
					tsr_report_m(STATUS_WARNING, REPORT_NULLIMGF,
						tsr->world.tiles[i].keys[j]);
				tsr->world.tiles[i].texture[j] = fallback(tsr);
			}
			else
				tsr->world.tiles[i].texture[j] = *texture;
			j++;
		}
		i++;
	}
}

void	init_textures(t_tsr *tsr)
{
	static const char	*path[] = {
		"assets/textures/tiles/stone.png",
		"assets/textures/tiles/dirt.png",
		"assets/textures/tiles/oak_planks.png",
		"assets/textures/tiles/iron_block.png",
		"assets/textures/tiles/moss.png",
		"assets/textures/tiles/blue_coral_block.png",
		"assets/textures/tiles/glass.png",
		"assets/textures/tiles/white_stained_glass.png",
		"assets/textures/tiles/grass_block_top.png",
		"assets/textures/tiles/grass_block_side.png",
		"assets/textures/tiles/oak_leaves.png",
		"assets/textures/tiles/oak_log.png",
		"assets/textures/tiles/oak_log_top.png",
		"assets/textures/tiles/skybox_up.png",
		"assets/textures/tiles/skybox_down.png",
		"assets/textures/tiles/skybox_east.png",
		"assets/textures/tiles/skybox_west.png",
		"assets/textures/tiles/skybox_north.png",
		"assets/textures/tiles/skybox_south.png",
		"assets/textures/tiles/red_stained_glass.png",
		"assets/textures/tiles/green_stained_glass.png",
		"assets/textures/tiles/blue_stained_glass.png",
		"assets/textures/tiles/stone_bricks.png",
		"assets/textures/tiles/cobblestone.png", 0};
	size_t				i;

	i = 0;
	while (path[i])
		atlas_add(tsr, &tsr->atlas, path[i++]);
	load_textures(tsr);
}
