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

void	get_normal(t_tsr_ray *ray, t_tsr_tile *tile)
{
	t_mbx_region	*nrm;
	t_vec2i			uv;
	t_mbx_color		col;
	t_vec3			normal;

	ray->tile_normal = get_tile_normal(ray->dir, ray->axis);
	return ;
	nrm = tile->texture[ray->axis * 2
		+ (ray->dir_sign.v[ray->axis] > 0)]->texture;
	if (nrm)
	{
		uv = vec2i_mult_vd(nrm->size, ray->uv);
		col = mbx_get_pixel_unsafe(nrm, uv);
		normal.x = 1.0 - col.r * 2.0 / 255;
		normal.y = col.g * 2.0 / 255 - 1.0;
		normal.z = sqrt(1.0 - normal.x * normal.x - normal.y * normal.y);
		ray->tile_normal = normal;
		normal_map_transform(ray);
	}
	else
		ray->tile_normal = get_tile_normal(ray->dir, ray->axis);
}
