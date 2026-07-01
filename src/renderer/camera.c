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
#include "tsr_constants.h"

t_vec3x3	get_view_matrix(t_vec3 rotation)
{
	const t_vec2	roll = vec2(cos(rotation.z), sin(rotation.z));
	t_vec3x3		view_up;
	t_vec3x3		view;

	view_up.p3 = vec3(
			-sin(rotation.x) * cos(rotation.y),
			sin(rotation.y),
			-cos(rotation.x) * cos(rotation.y));
	view_up.p1 = vec3_normalize(vec3_cross(view_up.p3, vec3(0, 1, 0)));
	view_up.p2 = vec3_cross(view_up.p1, view_up.p3);
	view.p3 = view_up.p3;
	view.p1 = vec3_add(
			vec3_mult_d(view_up.p1, roll.x),
			vec3_mult_d(view_up.p2, roll.y));
	view.p2 = vec3_sub(
			vec3_mult_d(view_up.p2, roll.x),
			vec3_mult_d(view_up.p1, roll.y));
	return (view);
}

static void	camera_rotation(t_tsr *tsr)
{
	const t_vec2	cursor_off = vec2_mult_d(vec2_div(tsr->mbx->cursor_delta,
				vec2_vi(tsr->mbx->vp->size)), -tsr->extras.camera_sensitivity);
	const t_vec3	vel = vec3_len_clamp(
			vec3_mult_d(tsr->player.velocity, 1), 0, 1);
	const t_vec3	mv_off = vec3(0,
			vec3_dot(tsr->player.forward, vel) * -0.5,
			vec3_dot(tsr->player.right, vel) * -0.5);

	tsr->camera.rotation.x = fwrap(tsr->camera.rotation.x
			+ cursor_off.x, -M_PI, M_PI);
	tsr->camera.rotation.y = fclamp(tsr->camera.rotation.y
			+ cursor_off.y, -M_PI_2, M_PI_2);
	tsr->camera.view_rotation = vec3_add(tsr->camera.rotation, mv_off);
}

void	tsr_update_camera(t_tsr *tsr)
{
	t_vec3x3	view;

	tsr->camera.position = tsr->player.position;
	tsr->camera.chunk = tsr->player.chunk;
	if (!tsr->mbx->settings.lock_cursor)
		return ;
	camera_rotation(tsr);
	view = get_view_matrix(tsr->camera.view_rotation);
	tsr->camera.right = view.p1;
	tsr->camera.up = view.p2;
	tsr->camera.forward = view.p3;
}
