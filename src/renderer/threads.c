/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 18:21:56 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/26 18:21:56 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	render_job(t_tsr *tsr, size_t i)
{
	const t_vec2i	start = vec2i(
			i % tsr->rendering.job_region_count.x * RENDER_JOB_REGION_SIZE,
			i / tsr->rendering.job_region_count.x * RENDER_JOB_REGION_SIZE);
	const t_vec2i	end = vec2i_add_d(start, RENDER_JOB_REGION_SIZE);
	t_vec2i			pos;

	pos.y = start.y;
	while (pos.y < end.y)
	{
		pos.x = start.x;
		while (pos.x < end.x)
		{
			mbx_set_pixel_raw(tsr->mbx->vp, pos,
				tsr->rendering.frag_shader(tsr, pos));
			pos.x++;
		}
		pos.y++;
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
	t_tsr	*tsr;

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
