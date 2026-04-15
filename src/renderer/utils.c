/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:32:34 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/15 16:41:18 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/tsr_renderer.h"

void	vec3_yaw_rot(t_vec3 *v, double ang)
{
	const double	x = v->x;
	const double	cosa = cos(ang);
	const double	sina = sin(ang);

	v->x = x * cosa - v->z * sina;
	v->z = x * sina + v->z * cosa;
}

void	vec3_pitch_rot(t_vec3 *v, double ang)
{
	const double	z = v->z;
	const double	cosa = cos(ang);
	const double	sina = sin(ang);

	v->z = z * cosa - v->y * sina;
	v->y = z * sina + v->y * cosa;
}
