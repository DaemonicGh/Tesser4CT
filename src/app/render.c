/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 02:45:33 by rprieur           #+#    #+#             */
/*   Updated: 2026/07/01 02:45:55 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	update_buffers(t_tsr *tsr)
{
	t_mbx_region	*tmp;

	if (tsr->rendering.target->size.y != tsr->extras.render_scale)
	{
		mbx_resize_region_with_content(tsr->mbx, &tsr->rendering.target,
			vec2i(tsr->extras.aspect_ratio * tsr->extras.render_scale,
				tsr->extras.render_scale));
		mbx_resize_region_with_content(tsr->mbx, &tsr->rendering.swap_target,
			vec2i(tsr->extras.aspect_ratio * tsr->extras.render_scale,
				tsr->extras.render_scale));
	}
	tmp = tsr->rendering.target;
	tsr->rendering.target = tsr->rendering.swap_target;
	tsr->rendering.swap_target = tmp;
}

void	prepare_next_render(t_tsr *tsr)
{
	update_buffers(tsr);
	tsr->rendering.current_job = 0;
	tsr->rendering.cross_cycle = tsr->mbx->frames_elapsed % 2;
	tsr->rendering.job_region_count = vec2i(
			tsr->rendering.target->size.x / RENDER_JOB_REGION_W,
			tsr->rendering.target->size.y / RENDER_JOB_REGION_H);
	tsr->rendering.job_count = (tsr->rendering.job_region_count.x
			* tsr->rendering.job_region_count.y);
	tsr->mbx->viewport = tsr->rendering.swap_target;
	tsr->rendering.data.camera = tsr->camera;
	if (tsr->rendering.data.world.chunks != tsr->world.chunks)
		free(tsr->rendering.data.world.chunks);
	tsr->rendering.data.world = tsr->world;
	tsr_world_update(tsr);
	tsr->rendering.data.frag_shader = tsr->rendering.frag_shader;
}
