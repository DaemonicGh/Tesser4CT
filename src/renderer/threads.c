/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 18:21:56 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/20 18:00:14 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdatomic.h>
#include "tsr.h"

static void	render_job(t_tsr *tsr, size_t job_i)
{
	const t_vec2i	start = vec2i(
			job_i % tsr->rendering.job_region_count.x * RENDER_JOB_REGION_W,
			job_i / tsr->rendering.job_region_count.x * RENDER_JOB_REGION_H);
	t_vec2i			pos;
	t_vec2i			frag_pos;
	t_mbx_color		col;

	pos.y = 0;
	frag_pos.y = start.y;
	while (pos.y++ < RENDER_JOB_REGION_H)
	{
		pos.x = 0;
		frag_pos.x = start.x;
		while (pos.x++ < RENDER_JOB_REGION_W)
		{
			if (!tsr->extras.cross_fill || frag_pos.x % 2 == frag_pos.y % 2
				== tsr->rendering.cross_cycle)
				col = tsr->rendering.data.frag_shader(tsr, frag_pos);
			else
				col = mbx_get_pixel_raw(tsr->rendering.swap_target, frag_pos);
			mbx_set_pixel_raw(tsr->rendering.target, frag_pos, col);
			frag_pos.x++;
		}
		frag_pos.y++;
	}
}

static void	render_thread_loop(t_tsr *tsr)
{
	size_t	i;

	while (tsr->rendering.running)
	{
		i = atomic_fetch_add(&tsr->rendering.current_job, 1);
		if (i >= tsr->rendering.job_count)
			break ;
		render_job(tsr, i);
	}
}

void	*tsr_render_thread_init(void *data)
{
	t_tsr		*tsr;

	tsr = data;
	tsr->rendering.threads_waiting++;
	pthread_barrier_wait(&tsr->rendering.wait_barrier);
	tsr->rendering.threads_waiting--;
	while (tsr->rendering.running)
	{
		tsr->rendering.threads_waiting++;
		pthread_barrier_wait(&tsr->rendering.wait_barrier);
		tsr->rendering.threads_waiting--;
		render_thread_loop(tsr);
		tsr->rendering.threads_waiting++;
		pthread_barrier_wait(&tsr->rendering.wait_barrier);
		tsr->rendering.threads_waiting--;
	}
	return (NULL);
}
