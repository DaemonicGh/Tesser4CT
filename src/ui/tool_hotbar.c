/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_hotbar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 02:19:33 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/26 02:19:33 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_ui.h"

static void	draw_focused(t_tsr *tsr)
{
	const char	*names[][2] = {
	{"Create Chunk", "Link Chunk"}, {"Highlight World Boundaries"},
	{"Set Chunk Boundary Tile", "Set Default Chunk Boundary Tile"},
	{"Teleport to Chunk"}, {"Set Skylight Direction"},
	{NULL, "Destroy Unused Chunks"}, {"Save Map", "Save Map as"}};
	char		name[64];

	mbx_set_region(tsr->ui.target, tsr->textures.gui.hotbar_selection,
		vec2i(32, 319));
	if (names[tsr->player.hotbar_tool][0])
	{
		mbx_set_region(tsr->ui.target, tsr->textures.gui.mouse_click,
			vec2i(4, 300));
		snprintf(name, 64, "%.64s", names[tsr->player.hotbar_tool][0]);
		mbx_set_text(tsr->ui.target, name,
			vec2i(16, 300), tsr->textures.font_small);
	}
	if (names[tsr->player.hotbar_tool][1])
	{
		mbx_set_subregion(tsr->ui.target, tsr->textures.gui.mouse_click,
			vec2i(4, 312), vec2ix2_xy(0, 0, -8, 8));
		snprintf(name, 64, "%.64s", names[tsr->player.hotbar_tool][1]);
		mbx_set_text(tsr->ui.target, name,
			vec2i(16, 312), tsr->textures.font_small);
	}
}

void	draw_tool_hotbar(t_tsr *tsr)
{
	double		off;
	int			i;

	set_hotbar_offset(&tsr->ui.tool_hotbar, tsr->player.hotbar_tool,
		vec2(0, 7));
	i = -1;
	while (i <= 2)
	{
		off = i - tsr->ui.tool_hotbar.offset
			+ (int)(tsr->ui.tool_hotbar.offset) + 1.5;
		mbx_set_region(tsr->ui.target, tsr->textures.tools
		[wrap((int)(tsr->ui.tool_hotbar.offset) + i, 0, 7)],
			vec2i(170 * sin(off / 5) - 12, 348 - 22 * cos(-off * off / 5.7)));
		i++;
	}
	if (tsr->player.on_toolbar)
		draw_focused(tsr);
}
