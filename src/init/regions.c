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

static void	load_tool_regions(t_tsr *tsr)
{
	tsr->textures.tools[TOOL_CREATE_CHUNK] = load_region(tsr,
			"gui/create_chunk.png");
	tsr->textures.tools[TOOL_HIGHLIGHT_LIMITS] = load_region(tsr,
			"gui/highlight_limits.png");
	tsr->textures.tools[TOOL_SET_LIMIT] = load_region(tsr,
			"gui/set_limit.png");
	tsr->textures.tools[TOOL_TELEPORT] = load_region(tsr,
			"gui/teleport.png");
	tsr->textures.tools[TOOL_SKYLIGHT] = load_region(tsr,
			"gui/set_skylight.png");
	tsr->textures.tools[TOOL_TRASH_CHUNKS] = load_region(tsr,
			"gui/trash_chunks.png");
	tsr->textures.tools[TOOL_SAVE_MAP] = load_region(tsr,
			"gui/save_map.png");
}

static void	load_gui_regions(t_tsr *tsr)
{
	tsr->textures.gui.hotbar_selection = load_region(tsr,
			"gui/hotbar_selection.png");
	tsr->textures.gui.minimap_cursor = load_region(tsr,
			"gui/minimap_cursor.png");
	tsr->textures.gui.mouse_click = load_region(tsr,
			"gui/mouse_click.png");
	tsr->textures.gui.bar = load_atlas(tsr,
			"gui/bar.png", vec2i(0, 13));
	tsr->textures.gui.button = load_atlas(tsr,
			"gui/button.png", vec2i(0, 13));
	tsr->textures.gui.toggle = load_atlas(tsr,
			"gui/toggle.png", vec2i(0, 8));
	tsr->textures.gui.slider = load_atlas(tsr,
			"gui/slider.png", vec2i(5, 0));
}

void	load_regions(t_tsr *tsr)
{
	tsr->textures.default_region = load_region(tsr,
			"default.png");
	if (!tsr->textures.default_region)
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLDEFIMGF);
	tsr->textures.font_small = load_atlas(tsr,
			"fonts/small.png", vec2i(5, 7));
	tsr->textures.font_title = load_atlas(tsr,
			"fonts/title.png", vec2i(10, 12));
	tsr->textures.tile_highlight = load_region(tsr,
			"tiles/highlight.png");
	tsr->textures.tile_face_highlight = load_region(tsr,
			"tiles/face_highlight.png");
	load_gui_regions(tsr);
	load_tool_regions(tsr);
}
