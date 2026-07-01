/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_player.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 02:45:35 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 02:47:33 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tsr_core.h"

enum	e_tsr_player_chunk_mode
{
	CHUNK_DEFAULT				= 0x2,
	CHUNK_CONNECT_MASK			= 0x3,
	CHUNK_CONNECT_FRONT_ONEWAY	= 0x0,
	CHUNK_CONNECT_FRONT			= 0x1,
	CHUNK_CONNECT_ALL			= 0x2,
	CHUNK_TARGET_MASK			= 0x4,
	CHUNK_TARGET_NEIGHBOR		= 0x0,
};

void
player_rotation(t_tsr *tsr);

void
player_collision(t_tsr *tsr);

void
player_movement(t_tsr *tsr);

void
item_select(t_tsr *tsr);

void
place_and_destroy(t_tsr *tsr);

void
create_chunk(t_tsr *tsr, t_tsr_chunk_id chunk, int flags);

void
player_update_prompt(t_tsr *tsr);

void
player_tools(t_tsr *tsr);

void
tsr_update_player(t_tsr *tsr);
