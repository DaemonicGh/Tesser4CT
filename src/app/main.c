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

static void	prepare_next_render(t_tsr *tsr)
{
	t_mbx_region	*tmp;

	tsr->rendering.current_job = 0;
	tsr->rendering.job_region_count = vec2i(
			tsr->mbx->vp->size.x / RENDER_JOB_REGION_W,
			tsr->mbx->vp->size.y / RENDER_JOB_REGION_H);
	tsr->rendering.job_count = (tsr->rendering.job_region_count.x
			* tsr->rendering.job_region_count.y);
	tmp = tsr->rendering.target;
	tsr->rendering.target = tsr->rendering.swap_target;
	tsr->rendering.swap_target = tmp;
	tsr->mbx->viewport = tsr->rendering.swap_target;
	tsr->rendering.data.camera = tsr->camera;
	if (tsr->rendering.data.world.chunks != tsr->world.chunks)
		free(tsr->rendering.data.world.chunks);
	if (tsr->rendering.data.world.chunk_refs != tsr->world.chunk_refs)
		free(tsr->rendering.data.world.chunk_refs);
	tsr->rendering.data.world = tsr->world;
	tsr->rendering.data.frag_shader = tsr->rendering.frag_shader;
}

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
	t_tsr	*tsr;

	tsr = data;
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
