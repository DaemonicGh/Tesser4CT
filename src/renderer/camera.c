/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:26:36 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/21 20:49:38 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

// TODO: roll implementation
void	tsr_update_camera(t_tsr *tsr)
{
	tsr->camera.chunk_position = tsr->player.chunk_position;
	tsr->camera.chunk = tsr->player.chunk;
	if (!tsr->mbx->settings.lock_cursor)
		return ;
	tsr->camera.rotation.x = fwrap(
			tsr->camera.rotation.x - (tsr->mbx->cursor_delta.x
				/ tsr->mbx->vp->size.x * CAMERA_SENSITIVITY), -M_PI, M_PI);
	tsr->camera.rotation.y = fclamp(
			tsr->camera.rotation.y - (tsr->mbx->cursor_delta.y
				/ tsr->mbx->vp->size.y * CAMERA_SENSITIVITY), -M_PI_2, M_PI_2);
	tsr->camera.forward = vec3(
			-sin(tsr->camera.rotation.x) * cos(tsr->camera.rotation.y),
			sin(tsr->camera.rotation.y),
			-cos(tsr->camera.rotation.x) * cos(tsr->camera.rotation.y));
	tsr->camera.right = vec3_normalize(vec3_cross(
				tsr->camera.forward, vec3(0, 1, 0)));
	tsr->camera.up = vec3_cross(tsr->camera.right, tsr->camera.forward);
}
