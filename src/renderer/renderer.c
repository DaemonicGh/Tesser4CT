/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:43:42 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/23 18:07:57 by emarrot          ###   ########.fr       */
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
		t_dist.x = tsr->pos.x - clamp(
				floor(ray_org.x), 0, tsr->world.width - 1);
	if (sgn.y == -1)
		t_dist.y = ray_org.y - clamp(
				floor(ray_org.y), 0, tsr->world.height - 1);
	if (sgn.z == -1)
		t_dist.z = ray_org.z - clamp(
				floor(ray_org.z), 0, tsr->world.depth - 1);
	t_delta = vec3_abs(t_delta);
	t_dist = vec3_mult(t_dist, t_delta);
	block = vec3i(
			clamp(floor(ray_org.x), 0, tsr->world.width - 1),
			clamp(floor(ray_org.y), 0, tsr->world.height - 1),
			clamp(floor(ray_org.z), 0, tsr->world.depth - 1));
	while (inbound(&tsr->world, block) && !block_get(&tsr->world, block))
		axis = step_tdist(&t_dist, &t_delta, &block, &sgn);
	t_dist = vec3_sub(t_dist, t_delta);
	return ((t_traversal){axis, (double [3]){
		t_dist.x, t_dist.y, t_dist.z}[axis], block});
}

void	setup_frag(t_mbx_region *buffer, t_tsr *tsr)
{
	const double	ratio = (double)buffer->size.y / buffer->size.x;
	t_vec3			ray_dir;
	t_vec3			ray_dir_tmp;
	t_vec3			dxy[2];
	t_vec2i			frag;
	t_traversal		traversal;
	t_mbx_color		icolor;

	ray_dir = vec3(-1.0, ratio, 1.0);
	vec3_pitch_rot(&ray_dir, -tsr->ang.y);
	vec3_yaw_rot(&ray_dir, -tsr->ang.x);
	dxy[0] = vec3(2.0 / buffer->size.x, 0.0, 0.0);
	dxy[1] = vec3(0.0, -2.0 * ratio / buffer->size.y, 0.0);
	vec3_pitch_rot(dxy + 0, -tsr->ang.y);
	vec3_yaw_rot(dxy + 0, -tsr->ang.x);
	vec3_pitch_rot(dxy + 1, -tsr->ang.y);
	vec3_yaw_rot(dxy + 1, -tsr->ang.x);
	frag.y = 0;
	while (frag.y < buffer->size.y)
	{
		ray_dir_tmp = ray_dir;
		frag.x = 0;
		while (frag.x < buffer->size.x)
		{
			traversal = ray_traversal(ray_dir, tsr->pos, tsr);
			if (traversal.axis == 0)
				icolor = color(0xffff20);
			else if (traversal.axis == 1)
				icolor = color(0x20ffff); 
			else if (traversal.axis == 2)
				icolor = color(0xff20ff);
			if (!inbound(&tsr->world, traversal.block))
				icolor = color(0);
			mbx_set_pixel_raw(buffer, frag, icolor);
			ray_dir = vec3_add(ray_dir, dxy[0]);
			frag.x++;
		}
		ray_dir = vec3_add(ray_dir_tmp, dxy[1]);
		frag.y++;
	}
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

#include <stdlib.h>

static t_mbx_color	combiner(t_vec3i icolor)
{
	return (color(icolor.x << 16 | icolor.y << 8 | icolor.z));
}

static t_vec3i	unmixer(t_mbx_color icolor)
{
	return (vec3i(icolor.r, icolor.g, icolor.b));
}

t_mbx_color	rt_shader(t_vec2 uv, void *data)
{
	t_uniform	*uniform;
	t_vec3		ray_dir;
	t_tsr		*tsr;
	t_traversal	traversal;
	t_vec3i		frag_color;
	t_vec3		point;
	t_vec2		tuv;

	uniform = data;
	tsr = uniform->tsr;
	ray_dir = vec3_add(vec3_add(uniform->forward_dir,
		vec3_mult_d(uniform->right_dir, uv.x * 2.0 - 1.0)),
		vec3_mult_d(uniform->up_dir,
			(1.0 - uv.y * 2.0) * uniform->aspect_ratio));
	traversal = ray_traversal(ray_dir, tsr->pos, tsr);
	if (!inbound(&tsr->world, traversal.block))
		return (color(0));	
	point = vec3_add(tsr->pos, vec3_mult_d(ray_dir, traversal.dist));
	point = vec3_add(point, vec3_mult_d(tsr->light, 1e-12));
	if (traversal.axis == 0)
		tuv = vec2(fmod(point.z, 1.0), fmod(point.y, 1.0));
	else if (traversal.axis == 1)
		tuv = vec2(fmod(point.x, 1.0), fmod(point.z, 1.0));
	else
		tuv = vec2(fmod(point.x, 1.0), fmod(point.y, 1.0));
	traversal = ray_traversal(tsr->light, point, tsr);
	frag_color = unmixer(mbx_get_pixel_unsafe(tsr->tex,
		vec2i_mult_vd(tsr->tex->size, tuv)));
	if (block_get(&tsr->world, traversal.block))
		return (combiner(vec3i_div_d(frag_color, 2)));
	return (combiner(frag_color));
}
