/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:43:42 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/24 17:51:40 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/tsr_renderer.h"

static int	step_tdist(
	t_vec3 *t_dist, t_vec3 *t_delta, t_vec3i *block, t_vec3i *sgn)
{
	if (t_dist->x < t_dist->y)
	{
		if (t_dist->x < t_dist->z)
		{
			t_dist->x += t_delta->x;
			block->x += sgn->x;
			return (0);
		}
	}
	else if (t_dist->y < t_dist->z)
	{
		t_dist->y += t_delta->y;
		block->y += sgn->y;
		return (1);
	}
	t_dist->z += t_delta->z;
	block->z += sgn->z;
	return (2);
}

t_traversal	ray_traversal(t_vec3 ray_dir, t_vec3 ray_org, t_tsr *tsr)
{
	t_vec3	t_delta;
	t_vec3	t_dist;
	t_vec3i	block;
	t_vec3i	sgn;
	int		axis;

	t_delta = vec3_div_rd(1.0, ray_dir);
	sgn = vec3i(1, 1, 1);
	if (t_delta.x < 0)
		sgn.x = -1;
	if (t_delta.y < 0)
		sgn.y = -1;
	if (t_delta.z < 0)
		sgn.z = -1;
	t_dist = vec3(
			clamp(floor(ray_org.x), 0, tsr->world.width - 1) + 1 - ray_org.x,
			clamp(floor(ray_org.y), 0, tsr->world.height - 1) + 1 - ray_org.y,
			clamp(floor(ray_org.z), 0, tsr->world.depth - 1) + 1 - ray_org.z
			);
	if (sgn.x == -1)
		t_dist.x = ray_org.x - clamp(
				floor(ray_org.x), 0, tsr->world.width - 1);
	if (sgn.y == -1)
		t_dist.y = ray_org.y - clamp(
				floor(ray_org.y), 0, tsr->world.height - 1);
	if (sgn.z == -1)
		t_dist.z = ray_org.z - clamp(
				floor(ray_org.z), 0, tsr->world.depth - 1);
	t_delta = vec3_abs(t_delta);
	t_dist = vec3_mult(t_dist, t_delta);
	block = vec3i_vd(vec3_exec(ray_org, floor));
	block.x = clamp(block.x, 0, tsr->world.width - 1);
	block.y = clamp(block.y, 0, tsr->world.height - 1);
	block.z = clamp(block.z, 0, tsr->world.depth - 1);
	axis = step_tdist(&t_dist, &t_delta, &block, &sgn);
	while (inbound(&tsr->world, block) && !block_get(&tsr->world, block))
		axis = step_tdist(&t_dist, &t_delta, &block, &sgn);
	t_dist = vec3_sub(t_dist, t_delta);
	return ((t_traversal){axis, (double [3]){
		t_dist.x, t_dist.y, t_dist.z}[axis], block});
}

t_vec3	get_normal(t_vec3 ray_dir, int axis)
{
	t_vec3	normal;

	normal = vec3_zero();
	if (axis == 0)
		normal.x = 1.0;
	else if (axis == 1)
		normal.y = 1.0;
	else if (axis == 2)
		normal.z = 1.0;
	if (ray_dir.x < 0)
		normal.x = -normal.x;
	if (ray_dir.y < 0)
		normal.y = -normal.y;
	if (ray_dir.z < 0)
		normal.z = -normal.z;
	return (normal);
}

/*static t_vec3	reflection(t_vec3 incident, t_vec3 normal)
{
	return (vec3_sub(incident, vec3_mult_d(normal,
		2.0 * vec3_dot(incident, normal))));
}*/

static t_mbx_color	combiner(t_vec3i icolor)
{
	return (color(icolor.x << 16 | icolor.y << 8 | icolor.z));
}

static t_vec3i	unmixer(t_mbx_color icolor)
{
	return (vec3i(icolor.r, icolor.g, icolor.b));
}

#include <stdio.h>

t_mbx_color	rt_shader(t_vec2 uv, void *data)
{
	t_uniform	*uniform;
	t_vec3		ray_dir;
	t_tsr		*tsr;
	t_traversal	traversal;
	t_vec3i		frag_color;
	t_vec3		point;
	//t_vec3	reflect;
	t_vec2		tuv;
	t_type		type;

	uniform = data;
	tsr = uniform->tsr;
	ray_dir = vec3_add(vec3_add(uniform->forward_dir,
		vec3_mult_d(uniform->right_dir, uv.x * 2.0 - 1.0)),
		vec3_mult_d(uniform->up_dir,
			(1.0 - uv.y * 2.0) * uniform->aspect_ratio));
	traversal = ray_traversal(ray_dir, tsr->pos, tsr);
	if (!inbound(&tsr->world, traversal.block))
		return (color(0x90e0fc));
	point = vec3_add(tsr->pos, vec3_mult_d(ray_dir, traversal.dist));
	if (traversal.axis == 0)
		tuv = vec2(point.z, point.y);
	else if (traversal.axis == 1)
		tuv = vec2(point.x, point.z);
	else
		tuv = vec2(point.x, point.y);
	tuv = vec2(tuv.x - floor(tuv.x), tuv.y - floor(tuv.y));
	type = block_get(&tsr->world, traversal.block);
	frag_color = unmixer(mbx_get_pixel_unsafe(tsr->tex[type],
		vec2i_mult_vd(tsr->tex[type]->size, tuv)));
	// Code for double reflexion
	/*frag_color = vec3i_mult_d(frag_color, 3);
	reflect = reflection(ray_dir, get_normal(ray_dir, traversal.axis));
	point = vec3_add(point, vec3_mult_d(reflect, 1e-12));
	traversal = ray_traversal(reflect, point, tsr);
	if (!inbound(&tsr->world, traversal.block))
	{
		frag_color = vec3i_add(frag_color, vec3i_mult_d(
			vec3i(144, 224, 252), 3));
		return (combiner(vec3i_div_d(frag_color, 6)));
	}
	point = vec3_add(point, vec3_mult_d(reflect, traversal.dist));
	if (traversal.axis == 0)
		tuv = vec2(fmod(point.z, 1.0), fmod(point.y, 1.0));
	else if (traversal.axis == 1)
		tuv = vec2(fmod(point.x, 1.0), fmod(point.z, 1.0));
	else
		tuv = vec2(fmod(point.x, 1.0), fmod(point.y, 1.0));
	frag_color = vec3i_add(frag_color, vec3i_mult_d(unmixer(
		mbx_get_pixel_unsafe(tsr->tex,
			vec2i_mult_vd(tsr->tex->size, tuv))), 2));
	reflect = reflection(reflect, get_normal(reflect, traversal.axis));
	point = vec3_add(point, vec3_mult_d(reflect, 1e-12));
	traversal = ray_traversal(reflect, point, tsr);
	if (!inbound(&tsr->world, traversal.block))
	{
		frag_color = vec3i_add(frag_color, vec3i(144, 224, 252));
		return (combiner(vec3i_div_d(frag_color, 6)));
	}
	point = vec3_add(point, vec3_mult_d(reflect, traversal.dist));
	if (traversal.axis == 0)
		tuv = vec2(fmod(point.z, 1.0), fmod(point.y, 1.0));
	else if (traversal.axis == 1)
		tuv = vec2(fmod(point.x, 1.0), fmod(point.z, 1.0));
	else
		tuv = vec2(fmod(point.x, 1.0), fmod(point.y, 1.0));
	frag_color = vec3i_add(frag_color, unmixer(
		mbx_get_pixel_unsafe(tsr->tex, vec2i_mult_vd(tsr->tex->size, tuv))));*/
	return (combiner(frag_color));
}
