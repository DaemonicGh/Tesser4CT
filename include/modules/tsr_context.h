/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_context.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 22:12:13 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/28 21:52:53 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "mlem.h"
#include "tsr_constants.h"
#include "tsr_world.h"

typedef struct s_tsr_player
{
	t_vec3			rotation;
	t_vec3			hitbox;
	t_vec3			right;
	t_vec3			up;
	t_vec3			forward;
	t_vec3			velocity;
	t_tsr_chunk		*chunk;
	t_vec3			chunk_position;
	t_tsr_tile_id	tile_id;
}	t_tsr_player;

typedef struct s_tsr_camera
{
	t_vec3			rotation;
	t_vec3			right;
	t_vec3			up;
	t_vec3			forward;
	t_vec3			velocity;
	t_tsr_chunk		*chunk;
	t_vec3			chunk_position;
}	t_tsr_camera;

typedef struct s_tsr_context
{
	t_mbx				*mbx;
	struct	s_tsr_texture_manager
	{
		t_tsr_texture		textures[TEXTURE_BUFFER_SIZE];
		t_mlem_value		textures_mlem;
		size_t				count;
		t_mbx_region		*_default;
		t_mbx_region		*tile_highlight;
		t_mbx_region		*tile_face_highlight;
		t_mbx_region		*hotbar_selection;
		t_mbx_atlas			*font_small;
		t_mbx_atlas			*font_title;
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
			UI_STATE_PAUSE,
			UI_STATE_SETTING
		}					state;
		struct	s_tsr_hotbar
		{
			double					offset;
			double					delta;
		}					hotbar;
	}	ui;
	struct s_tsr_world_manager
	{
		t_tsr_tile_data		tiles[TILE_BUFFER_SIZE];
		t_mlem_value		tiles_mlem;
		size_t				tile_count;
		t_tsr_chunk_ref		*chunk_refs;
		t_tsr_chunk			*chunks;
		t_mlem_value		map_mlem;
		t_tsr_tile			skybox;
		t_vec3				global_light;
		t_vec4				global_light_col;
		t_tsr_chunk			*origin;
		t_vec3				spawn_pos;
		uint8_t				tile_highlight_chunk_id;
		t_vec3i				tile_highlight_pos;
		int					tile_highlight_axis;
	}	world;
	t_tsr_player		player;
	t_tsr_camera		camera;
	struct s_tsr_extras
	{
		double				aspect_ratio;
		double				fov;
		double				focal_length;
	}	extras;
}	t_tsr;
