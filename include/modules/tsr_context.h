/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_context.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 22:12:13 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 02:45:47 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "modules/types/mbx_s_region.h"
#include "tsr_world.h"

typedef struct s_tsr_player
{
	t_vec3			position;
	t_vec3			rotation;
	t_vec3			hitbox;
	t_vec3			right;
	t_vec3			up;
	t_vec3			forward;
	t_vec3			velocity;
	t_tsr_tile_id	tile_id;
}	t_tsr_player;

typedef struct s_tsr_camera
{
	t_vec3		position;
	t_vec3		rotation;
	t_vec3		right;
	t_vec3		up;
	t_vec3		forward;
	t_vec3		velocity;
}	t_tsr_camera;

#define TSR_TEXTURE_COUNT	6

typedef struct s_tsr_context
{
	t_mbx				*mbx;
	union	u_tsr_texture_manager
	{
		struct
		{
			t_mbx_region		*_default;
			t_mbx_region		*tile_highlight;
			t_mbx_region		*tile_face_highlight;
			t_mbx_region		*hotbar_selection;
			t_mbx_atlas			*font_small;
			t_mbx_atlas			*font_title;
		};
		t_mbx_region		*regions[TSR_TEXTURE_COUNT];
	}	textures;
	struct	s_tsr_rendering_manager
	{
		t_mbx_region		*target;
		t_mbx_region		*swap_target;
		size_t				thread_count;
		pthread_t			*threads;
		pthread_barrier_t	wait_barrier;
		atomic_bool			running;
		size_t				job_count;
		atomic_size_t		current_job;
		t_vec2i				job_region_count;
		t_mbx_color			(*frag_shader)(t_tsr * tsr, t_vec2 uv);
		t_mbx_color			(*current_frag_shader)(t_tsr * tsr, t_vec2 uv);
	}	rendering;
	struct	s_tsr_ui_manager
	{
		t_mbx_region		*target;
		enum	e_tsr_ui_state: int
		{
			UI_STATE_GAME = 0,
			UI_STATE_MAIN,
			UI_STATE_PAUSE
		}					state;
		struct	s_tsr_hotbar
		{
			double					offset;
			double					delta;
		}					hotbar;
	}	ui;
	struct s_tsr_world_manager
	{
		t_tsr_tile			tiles[TILE_BUFFER_COUNT];
		t_vec3				global_light;
		t_vec3i				tile_highlight_pos;
		int					tile_highlight_axis;
		pthread_t			loader_thread;
		pthread_barrier_t	loader_wait_barrier;
	}	world;
	t_world				wworld;
	t_atlas				atlas;
	t_mbx_region		*nmap;
	t_tsr_player		player;
	t_tsr_camera		camera;
	struct s_tsr_extras
	{
		double				aspect_ratio;
	}	extras;
}	t_tsr;
