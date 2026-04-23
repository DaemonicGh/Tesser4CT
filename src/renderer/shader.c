/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:10:48 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/23 17:50:06 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/tsr_renderer.h"
#include <pthread.h>
#define BLOCK_LENGTH 16
#define THREADS 14

void	block_shader(t_job *job, const t_shader *shader)
{
	t_vec2i				frag_coord;
	t_vec2i				end_coord;
	t_vec2				uv;
	const t_vec2		duv = vec2_div_rd(1.0, vec2_vi(shader->buffer->size));
	double				tmp_u;

	frag_coord = job->start_coord;
	end_coord = vec2i_add_d(job->start_coord, BLOCK_LENGTH);
	end_coord = vec2i_exec2(end_coord, shader->buffer->size, min);
	uv = vec2_div(vec2_vi(frag_coord), vec2_vi(shader->buffer->size));
	tmp_u = uv.x;
	while (frag_coord.y < end_coord.y)
	{
		frag_coord.x = job->start_coord.x;
		uv.x = tmp_u;
		while (frag_coord.x < end_coord.x)
		{
			mbx_set_pixel_raw(shader->buffer,
				frag_coord, shader->f(uv, shader->data));
			frag_coord.x++;
			uv.x += duv.x;
		}
		frag_coord.y++;
		uv.y += duv.y;
	}
}

void	*thread_logic(void	*thread_pkg)
{
	const t_thread_pkg	*pkg = thread_pkg;
	t_job				*job;

	pthread_mutex_lock(pkg->mut);
	while (*pkg->job_len)
	{
		(*pkg->job_len)--;
		job = pkg->jobs + *pkg->job_len;
		pthread_mutex_unlock(pkg->mut);
		block_shader(job, pkg->shader);
		pthread_mutex_lock(pkg->mut);
	}
	pthread_mutex_unlock(pkg->mut);
	return (NULL);
}

void	threads_manager(t_job *jobs, size_t job_len, const t_shader *shader)
{
	pthread_mutex_t	mut;
	t_thread_pkg	threads[THREADS];
	void			*ret;
	size_t			i;

	pthread_mutex_init(&mut, NULL);
	i = 0;
	while (i < THREADS)
	{
		threads[i] = (t_thread_pkg){0, &mut, &job_len, jobs, shader};
		pthread_create(&threads[i].thread, NULL, thread_logic, threads + i);
		i++;
	}
	(void)ret;
	i = 0;
	while (i < THREADS)
		pthread_join(threads[i++].thread, &ret);
	pthread_mutex_destroy(&mut);
}

void	fragment_shader(t_mbx_region *buffer,
	t_mbx_color (*f)(t_vec2, void *), void *data)
{
	const t_shader	shader = {buffer, f, data};
	t_vec2i			coord;
	size_t			job_len;
	t_job			*jobs;
	size_t			i;

	job_len = ceil((double)buffer->size.x / BLOCK_LENGTH)
		* ceil((double)buffer->size.y / BLOCK_LENGTH);
	jobs = malloc(job_len * sizeof(t_job));
	if (!jobs)
		return ;
	i = 0;
	coord.y = 0;
	while (coord.y < buffer->size.y)
	{
		coord.x = 0;
		while (coord.x < buffer->size.x)
		{
			jobs[i++] = (t_job){coord};
			coord.x += BLOCK_LENGTH;
		}
		coord.y += BLOCK_LENGTH;
	}
	threads_manager(jobs, job_len, &shader);
	free(jobs);
}
