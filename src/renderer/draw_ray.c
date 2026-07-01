/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:43:42 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/28 22:08:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_drawing.h"
#include "modules/mbx_structs.h"
#include "tsr.h"
#include "tsr_constants.h"

static void	fix_zeros(t_vec3 *restrict origin, t_vec3 *restrict forward)
{
	size_t	i;

	i = 0;
	while (i < 3)
	{
		if (origin->v[i] == 0)
			origin->v[i] = 1e-6;
		if (forward->v[i] == 0)
			forward->v[i] = 1e-6;
		i++;
	}
}

t_tsr_ray	setup_ray_unsafe(
	t_tsr *tsr, t_vec3 origin, t_tsr_chunk_id chunk, t_vec3 forward)
{
	const t_vec3i		tile_pos = vec3i_vd(vec3_exec(floor, origin));
	t_tsr_ray			ray;

	ray = (t_tsr_ray){.origin = origin,
		.dir = forward, .dir_sign = vec3i_vd(vec3_sign(forward)),
		.delta = vec3_div_rd(1, forward),
		.chunk = chunk, .tile_position = tile_pos,
		.lifetime = tsr->extras.render_distance};
	ray.abs_delta = vec3_abs(ray.delta);
	ray.iter = vec3i_mult(vec3i(1, 4, 16), ray.dir_sign);
	ray.tile_index = tile_pos.x + tile_pos.y * 4 + tile_pos.z * 16;
	ray.tile = tsr_get_tile(&tsr->rendering.data.world, chunk, tile_pos);
	ray.tile_data = &tsr->rendering.data.world.data->tiles[ray.tile->type];
	ray.dist = vec3_mult(vec3_sub(vec3_vi(vec3i_add(tile_pos, vec3i(
							ray.dir_sign.x > 0,
							ray.dir_sign.y > 0,
							ray.dir_sign.z > 0))),
				ray.origin), ray.delta);
	return (ray);
}

t_tsr_ray	setup_ray(
	t_tsr *tsr, t_vec3 origin, t_tsr_chunk_id chunk, t_vec3 forward)
{
	t_vec3i				tile_pos;

	fix_zeros(&origin, &forward);
	chunk = tsr_relocate_chunk(&tsr->rendering.data.world, chunk, &origin);
	tile_pos = vec3i_vd(vec3_exec(floor, origin));
	origin = vec3_add(vec3_vi(tile_pos), vec3_exec(fract, origin));
	return (setup_ray_unsafe(tsr, origin, chunk, forward));
}

t_mbx_color	draw_ray(t_tsr *tsr, t_vec2i frag_pos)
{
	const t_tsr_camera	camera = tsr->rendering.data.camera;
	const t_vec2		uvc = vec2(16. / 9
			* ((double)frag_pos.x / tsr->rendering.target->size.x - 0.5),
			(double)frag_pos.y / tsr->rendering.target->size.y - 0.5);
	t_tsr_ray			ray;

	ray = setup_ray(tsr, camera.position, camera.chunk,
			vec3_add(vec3_mult_d(camera.forward, tsr->extras.focal_length),
				vec3_add(vec3_mult_d(camera.right, uvc.x),
					vec3_mult_d(camera.up, -uvc.y))));
	while (ray.lifetime)
	{
		trace_ray(tsr, &ray, false);
		if (set_ray_tile_color(tsr, &ray, frag_pos))
			break ;
	}
	return (ray.color);
}
