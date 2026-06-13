/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 16:51:50 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/28 22:08:59 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	normal_map_transform(t_tsr_ray *ray)
{
	const t_vec3	nrm = ray->tile_normal;

	if (ray->axis == 0)
	{
		if (ray->dir.x < 0)
			ray->tile_normal = vec3(-nrm.z, nrm.y, nrm.x);
		else
			ray->tile_normal = vec3(nrm.z, nrm.y, -nrm.x);
	}
	else if (ray->axis == 1)
	{
		if (ray->dir.y < 0)
			ray->tile_normal = vec3(nrm.x, -nrm.z, nrm.y);
		else
			ray->tile_normal = vec3(nrm.x, nrm.z, -nrm.y);
	}
	else if (ray->dir.z < 0)
		ray->tile_normal = vec3(-nrm.x, nrm.y, -nrm.z);
}

void	get_normal(t_tsr *tsr, t_tsr_ray *ray)
{
	t_vec2i			uv;
	t_mbx_color		col;
	t_vec3			normal;

	(void)tsr;
	if (!(ray->texture->flags & TX_NORMAL))
	{
		ray->tile_normal = get_tile_normal(ray->dir, ray->axis);
		return ;
	}
	uv = vec2i_mult_vd(ray->texture->texture->subregion_size,
			vec2_add(ray->uv, vec2(0, 1)));
	col = mbx_get_pixel_unsafe(ray->texture->texture, uv);
	normal.x = 1.0 - col.r * 2.0 / 255;
	normal.y = col.g * 2.0 / 255 - 1.0;
	normal.z = sqrt(1.0 - normal.x * normal.x - normal.y * normal.y);
	ray->tile_normal = normal;
	normal_map_transform(ray);
}
