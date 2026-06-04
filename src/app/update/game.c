/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 02:42:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/20 16:28:23 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	tsr_init_game(t_tsr *tsr)
{
	tsr->mbx->settings.hide_cursor = true;
	tsr->mbx->settings.lock_cursor = true;
	tsr->rendering.frag_shader = draw_ray;
}

void	tsr_update_game(t_tsr *tsr)
{
	tsr_update_player(tsr);
	if (mbx_btnp(tsr->mbx, MBX_KEY_F1))
	{
		tsr->mbx->settings.lock_cursor = !tsr->mbx->settings.lock_cursor;
		tsr->mbx->settings.hide_cursor = !tsr->mbx->settings.hide_cursor;
		mbx_refresh_settings(tsr->mbx);
	}
	if (mbx_btnp(tsr->mbx, MBX_KEY_ESCAPE))
		tsr->ui.state = UI_STATE_PAUSE;
	mbx_clear(tsr->ui.target, color_rgba(0x0));
	draw_ui(tsr);
}
