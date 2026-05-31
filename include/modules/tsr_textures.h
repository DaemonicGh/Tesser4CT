/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_textures.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:02:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/28 21:49:22 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tsr_core.h"

typedef struct s_tsr_pbr_texture_atlas
{
	enum e_tsr_texture_flags: uint8_t
	{
		TX_NONE			= 0x00,
		TX_NORMAL		= 0x01,
		TX_EMISSIVE		= 0x02,
		TX_SPECULAR		= 0x04,
		TX_ANIMATED		= 0x10
	}				flags;
	uint8_t			emissive_channel;
	uint8_t			specular_channel;
	int				normal_v;
	int				emissive_v;
	int				specular_v;
	t_mbx_atlas		*texture;
}	t_tsr_texture;
