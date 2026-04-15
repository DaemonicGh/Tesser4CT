/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 16:38:53 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/15 16:58:21 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

typedef unsigned int t_u32;

t_vec3	movement(t_mbx *mbx)
{
	t_vec3			input;
	const double	speed = 10 * mbx->delta_time;

	input = vec3_zero();
	if (mbx_key_held(mbx, MBX_KEY_W))
		input.z += 1.0;
	if (mbx_key_held(mbx, MBX_KEY_A))
		input.x -= 1.0;
	if (mbx_key_held(mbx, MBX_KEY_S))
		input.z -= 1.0;
	if (mbx_key_held(mbx, MBX_KEY_D))
		input.x += 1.0;
	if (mbx_key_held(mbx, MBX_KEY_LSHIFT))
		input.y -= 1.0;
	if (mbx_key_held(mbx, MBX_KEY_SPACE))
		input.y += 1.0;
	input = vec3_mult_d(vec3_normalize(input), speed);
	return (input);
}

void	mouse_cam(t_mbx *mbx, t_vec2 *ang)
{
	const t_vec2i midbuffer = vec2i(
		mbx->viewport.size.x / 2, mbx->viewport.size.y / 2);

	mbx_warp_cursor(mbx, midbuffer);
	ang->x += (double)mbx->cursor_delta.x / 256;
	ang->y += (double)mbx->cursor_delta.y / 256;
}

void	update(t_mbx *mbx, void *args)
{
	const t_mbx_color	background = color(0x101010);
	t_tsr				*tsr;
	t_vec3				mov;
	static char			info[16] = " ";
	static double		mspf = 0.0;
	static size_t		oldframe_elapsed = 0;
	static double		chrono = 0.0;

	tsr = args;
	mov = movement(mbx);
	vec3_yaw_rot(&mov, -tsr->ang.x);
	tsr->pos = vec3_add(tsr->pos, mov);
	mouse_cam(mbx, &tsr->ang);
	mbx_clear(&mbx->viewport, background);
	setup_frag(&mbx->viewport, tsr);
	if (chrono > 1.0)
	{
		snprintf(info, 16, "%.2f ms\n",
			mspf / (mbx->frames_elapsed - oldframe_elapsed));
		mspf = 0.0;
		chrono = 0.0;
		oldframe_elapsed = mbx->frames_elapsed;
	}
	mspf += mbx->seconds_per_frame * 1000;
	chrono += mbx->delta_time;
	mbx_set_text(&mbx->viewport, info, vec2i(0, 0), &tsr->font);
}

void	world_build(t_world *world)
{
	for (size_t x = 0; x < world->width; x++)
		for (size_t y = 0; y < world->height; y++)
			for (size_t z = 0; z < world->depth; z++)
				block_set(world, vec3i(x, y, z),
					(5 <= x && x <= 10 && 5 <= y && y <= 10 && 5 <= z && z <= 10)
					&& !(((6 <= x && x <= 9) && (6 <= y && y <= 9))
					|| ((6 <= y && y <= 9) && (6 <= z && z <= 9))
					|| ((6 <= x && x <= 9) && (6 <= z && z <= 9)))
				);
}

int main()
{
	t_mbx			*mbx;
	t_mbx_region	atlas;
	t_tsr			tsr;

	tsr.size = vec2i(640, 360);
	mbx = mbx_init(tsr.size, "Tesser4ct", 0);
	if (!mbx)
	{
		printf("Can't init MacroBox\n");
		return (1);
	}
	atlas = mbx_make_region_from_file(mbx, "lib/MacroBoX/assets/font_5x7.png"); 
	tsr.font = mbx_make_atlas(atlas, vec2i(5, 7));
	tsr.pos = vec3(8.0, 8.0, 4.0);
	tsr.vel = vec3_zero();
	tsr.ang = vec2_zero();
	world_create(&tsr.world, 16, 16, 16);
	world_build(&tsr.world);
	mbx->settings.show_cursor = false;
	mbx_refresh_settings(mbx);
	mbx_move_cursor(mbx, vec2i(tsr.size.x / 2, tsr.size.y / 2));
	mbx_run(mbx, update, &tsr);
	mbx_destroy_atlas(mbx, &tsr.font);
	mbx_exit(mbx);
	world_destroy(&tsr.world);
}
