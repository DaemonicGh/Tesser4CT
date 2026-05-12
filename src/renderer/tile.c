/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tile.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 23:38:59 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/09 15:11:01 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

t_vec3	get_normal(t_vec3 ray_dir, int axis)
{
	t_vec3	normal;

	normal = vec3_zero();
	normal.comp[axis] = fsign(ray_dir.comp[axis]);
	return (normal);
}

t_vec3	reflect(t_vec3 ray_dir, t_vec3 normal)
{
	return (vec3_sub(ray_dir, vec3_mult_d(
				normal, 2.0 * vec3_dot(normal, ray_dir))));
}
