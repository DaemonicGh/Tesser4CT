/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 16:27:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/09 11:39:51 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	tsr->rendering.target = tsr->mbx->viewport;
	tsr->mbx->viewport = tmp;
	tsr_update_camera(tsr);
}

void	main_update(t_mbx *mbx, void *data)
{
	t_tsr	*tsr;

	(void)mbx;
	tsr = data;
	tsr_update_game(tsr);
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	prepare_next_render(tsr);
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	tsr_draw_game(tsr);
}

int	main(void)
{
	t_tsr	*tsr;

	tsr_report(STATUS_DEBUG, REPORT_DEBUG_ON);
	tsr = tsr_init();
	world_create(&tsr->wworld, vec3i(33, 16, 33));
	tsr->mbx->settings.fps_cap = 120;
	mbx_run(tsr->mbx, main_update, tsr);
	tsr_exit(tsr, STATUS_INFO, REPORT_SUCCESS);
}
