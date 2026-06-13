/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_core.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 22:12:13 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/28 21:52:53 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdatomic.h>

#include "mlem.h"
#include "mbx.h"
#include "tsr_constants.h"

typedef uint8_t					t_tsr_tile_id;
typedef uint16_t				t_tsr_chunk_id;
typedef struct s_tsr_context	t_tsr;

typedef struct s_tsr_pbr_texture_atlas
{
	enum e_tsr_texture_flags: uint8_t
	{
		TX_NONE			= 0x00,
		TX_NORMAL		= 0x01,
		TX_PROPERTY		= 0x02,
		TX_ANIMATED		= 0x04
	}				flags;
	t_mbx_atlas		*texture;
}	t_tsr_texture;

typedef struct s_tsr_tile_data
{
	t_tsr_texture	*texture[6];
	char			*name;
	bool			skip;
	bool			backface;
	bool			inner_backface;
	bool			skybox;
	bool			specular;
}	t_tsr_tile_data;

typedef struct s_tsr_tile
{
	t_tsr_tile_id	type;
	uint8_t			orientation;
	uint8_t			light;
	uint8_t			render_light;
}	t_tsr_tile;

typedef struct s_tsr_chunk_ref
{
	uint64_t		process;
	t_tsr_chunk_id	neighbors[6];
}	t_tsr_chunk_ref;

typedef struct s_tsr_chunk
{
	t_tsr_tile		tiles[64];
	t_tsr_tile		limits[6];
	t_tsr_chunk_id	id;
}	t_tsr_chunk;

typedef struct s_tsr_render_ray
{
	t_vec3			origin;
	t_vec3			dir;
	t_vec3			delta;
	t_vec3			abs_delta;
	t_vec3i			dir_sign;
	t_vec3i			iter;
	t_vec3			dist;
	t_vec3			position;
	t_tsr_chunk_id	chunk;
	t_vec3i			tile_position;
	int				tile_index;
	int				lifetime;
	int8_t			axis;
	int8_t			tile_axis;
	int8_t			face;
	t_tsr_tile		*tile;
	t_tsr_tile_data	*tile_data;
	t_tsr_tile		*front_tile;
	t_vec2			uv;
	t_tsr_texture	*texture;
	t_vec3			tile_normal;
	t_vec4			color;
	bool			is_shadow;
}	t_tsr_ray;

typedef struct s_tsr_player
{
	t_vec3			rotation;
	t_vec3			hitbox;
	t_vec3			forward;
	t_vec3			right;
	t_vec3			up;
	t_vec3			velocity;
	t_tsr_chunk_id	chunk;
	t_vec3			position;
	t_tsr_tile_id	hotbar_tile;
	t_vec3i			face;
	t_tsr_ray		ray;
	t_tsr_chunk_id	tile_highlight_chunk;
	t_vec3i			tile_highlight_pos;
	int				tile_highlight_axis;
	enum	e_tsr_game_prompt_state: int
	{
		PROMPT_STATE_NONE,
		PROMPT_STATE_CHUNK,
		PROMPT_STATE_TP,
		PROMPT_STATE_SAVE,
	}				prompt_state;
}	t_tsr_player;

typedef struct s_tsr_camera
{
	t_vec3			rotation;
	t_vec3			right;
	t_vec3			up;
	t_vec3			forward;
	t_vec3			velocity;
	t_tsr_chunk_id	chunk;
	t_vec3			position;
}	t_tsr_camera;

typedef struct s_tsr_world_data
{
	t_tsr_tile_data		tiles[TILE_BUFFER_SIZE];
	size_t				tile_count;
	t_mlem_value		mlem;
	t_mlem_string		name;
	t_vec3				skylight;
	t_vec4				skylight_color;
	t_tsr_chunk_id		origin;
	t_tsr_tile			skybox;
}	t_tsr_world_data;

typedef struct s_tsr_world_content
{
	t_tsr_chunk_ref		*chunk_refs;
	t_tsr_chunk			*chunks;
	size_t				chunk_count;
	size_t				chunk_capacity;
	t_tsr_world_data	*data;
}	t_tsr_world;

typedef struct s_tsr_prompt
{
	t_vec2i				position;
	const char			*message;
	char				buffer[PROMPT_SIZE];
	uint32_t			cursor;
	bool				visible;
	bool				is_error;
	double				shake;
}	t_tsr_prompt;

typedef struct s_tsr_context
{
	t_mbx				*mbx;
	struct	s_tsr_texture_manager
	{
		t_tsr_texture		textures[TEXTURE_BUFFER_SIZE];
		t_mlem_value		mlem;
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
		struct s_tsr_rendering_data
		{
			t_tsr_camera		camera;
			t_tsr_world			world;
			t_mbx_color			(*frag_shader)(t_tsr * tsr, t_vec2 uv);
		}	data;
	}	rendering;
	struct	s_tsr_ui_manager
	{
		t_mbx_region		*target;
		t_tsr_prompt		prompt;
		enum	e_tsr_ui_state: int
		{
			UI_STATE_GAME,
			UI_STATE_MAIN,
			UI_STATE_PAUSE,
			UI_STATE_SETTING
		}					state;
		struct	s_tsr_hotbar
		{
			double				offset;
			double				delta;
		}					hotbar;
	}	ui;
	t_tsr_world_data	world_data;
	t_tsr_world			world;
	t_tsr_player		player;
	t_tsr_camera		camera;
	struct s_tsr_extras
	{
		double				aspect_ratio;
		double				fov;
		double				focal_length;
		bool				show_chunks;
	}	extras;
}	t_tsr;
