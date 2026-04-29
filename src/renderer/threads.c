/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 18:21:56 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/27 16:52:40 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_utils.h"
#include "tsr.h"
#include <stdatomic.h>

static void	render_job(t_tsr *tsr, size_t job_i)
{
	const t_vec2i	start = vec2i_mult_d(vec2i(
				job_i % tsr->rendering.job_region_count.x,
				job_i / tsr->rendering.job_region_count.x),
			RENDER_JOB_REGION_SIZE);
	const t_vec2	uv_delta = vec2_div_rd(1, vec2_vi(tsr->mbx->vp->size));
	t_vec2i			pos;
	size_t			i;
	t_vec2			uv;

	pos.y = 0;
	i = start.y * tsr->mbx->vp->size.x + start.x;
	uv.y = (double)start.y / tsr->mbx->vp->size.y - 0.5;
	while (pos.y++ < RENDER_JOB_REGION_SIZE)
	{
		pos.x = 0;
		uv.x = (double)start.x / tsr->mbx->vp->size.x - 0.5;
		while (pos.x++ < RENDER_JOB_REGION_SIZE)
		{
			mbx_set_pixel_raw_i(tsr->mbx->vp, i++,
				tsr->rendering.frag_shader(tsr, uv));
			uv.x += uv_delta.x;
		}
		uv.y += uv_delta.y;
		i += tsr->mbx->vp->size.x - RENDER_JOB_REGION_SIZE;
	}
}

static void	render_thread_loop(t_tsr *tsr)
{
	size_t	i;

	while (tsr->rendering.running)
	{
		i = atomic_fetch_add(&tsr->rendering.current_job, 1);
		if (i >= tsr->rendering.job_count)
			return ;
		render_job(tsr, i);
	}
}

void	*tsr_render_thread_init(void *data)
{
	t_tsr		*tsr;

	tsr = data;
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	while (tsr->rendering.running)
	{
		pthread_barrier_wait(&tsr->rendering.wait_barrier);
		render_thread_loop(tsr);
		pthread_barrier_wait(&tsr->rendering.wait_barrier);
	}
	return (NULL);
}
