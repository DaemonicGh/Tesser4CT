/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:27:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/29 01:52:40 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "tsr.h"

static void	init_state(t_tsr *tsr)
{
	static int	prev_state = -1;

	if (prev_state == tsr->ui.state)
		return ;
	prev_state = tsr->ui.state;
	if (tsr->ui.state == UI_STATE_GAME)
		tsr_init_game(tsr);
	else if (tsr->ui.state == UI_STATE_MAIN)
		tsr_init_main_menu(tsr);
	else if (tsr->ui.state == UI_STATE_PAUSE)
		tsr_init_pause_menu(tsr);
	else if (tsr->ui.state == UI_STATE_SETTING)
		tsr_init_setting_menu(tsr);
	mbx_refresh_settings(tsr->mbx);
}

static void	update_state(t_tsr *tsr)
{
	if (tsr->ui.state == UI_STATE_GAME)
		tsr_update_game(tsr);
	else if (tsr->ui.state == UI_STATE_MAIN)
		tsr_update_main_menu(tsr);
	else if (tsr->ui.state == UI_STATE_PAUSE)
		tsr_update_pause_menu(tsr);
	else if (tsr->ui.state == UI_STATE_SETTING)
		tsr_update_setting_menu(tsr);
}

void	main_update(t_mbx *mbx, void *data)
{
	t_tsr			*tsr;

	tsr = data;
	tsr->ui.cursor = vec2_round(vec2_div(tsr->mbx->cursor,
				vec2i_truediv(tsr->mbx->vp->size, tsr->ui.target->size)));
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	prepare_next_render(tsr);
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	update_state(tsr);
	init_state(tsr);
	mbx_render_region_as_viewport(mbx, tsr->mbx->viewport,
		MBX_VIEWPORT_RENDER_KEEP);
	mbx_render_region_as_viewport(mbx, tsr->ui.target,
		MBX_VIEWPORT_RENDER_KEEP);
}

int	main(void)
{
	t_tsr	*tsr;

	tsr_report(STATUS_DEBUG, REPORT_DEBUG_ON);
	tsr = tsr_init();
	init_state(tsr);
	mbx_run(tsr->mbx, main_update, tsr);
	tsr_exit(tsr, STATUS_INFO, REPORT_SUCCESS);
}
