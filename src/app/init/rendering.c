/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 19:30:01 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/31 19:30:01 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	init_threads(t_tsr *tsr)
{
	size_t				i;

	tsr->rendering.running = true;
	tsr->rendering.thread_count = max(sysconf(_SC_NPROCESSORS_ONLN) - 1, 1);
	tsr->rendering.threads = malloc(
			tsr->rendering.thread_count * sizeof(pthread_t));
	if (!tsr->rendering.threads)
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
	if (pthread_barrier_init(&tsr->rendering.wait_barrier, NULL,
			tsr->rendering.thread_count + 1))
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLTHREAD);
	i = 0;
	while (i < tsr->rendering.thread_count)
	{
		if (pthread_create(&tsr->rendering.threads[i],
				NULL, tsr_render_thread_init, tsr))
			tsr_exit(tsr, STATUS_ERROR, REPORT_NULLTHREAD);
		i++;
	}
}

void	init_rendering(t_tsr *tsr)
{
	if (!mbx_create_main_window(tsr->mbx,
			vec2i(DEFAULT_VIEWPORT_W, DEFAULT_VIEWPORT_H),
			DEFAULT_WINDOW_TITLE, DEFAULT_WINDOW_FLAGS))
		tsr_exit(tsr, STATUS_ERROR, REPORT_NULLMBXWIN);
	tsr->rendering.swap_target = tsr->mbx->viewport;
	tsr->rendering.target = mbx_create_region_with_image(
			tsr->mbx, tsr->mbx->vp->size);
	tsr->ui.target = mbx_create_region_with_image(
			tsr->mbx, tsr->mbx->vp->size);
	if (!tsr->rendering.target || !tsr->ui.target)
		tsr_exit(tsr, STATUS_ERROR, REPORT_MEMORY);
	mbx_change_alloc_flags(tsr->mbx, tsr->rendering.target,
		MBX_ALLOC_FREE_ON_RUN_END, MBX_ALLOC_EMPTY);
	mbx_change_alloc_flags(tsr->mbx, tsr->ui.target,
		MBX_ALLOC_FREE_ON_RUN_END, MBX_ALLOC_EMPTY);
	init_threads(tsr);
}
