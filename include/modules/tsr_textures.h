/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_textures.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:02:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/06/10 11:23:42 by emarrot          ###   ########.fr       */
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
		TX_PROPERTY		= 0x02,
		TX_ANIMATED		= 0x04
	}				flags;
	t_mbx_atlas		*texture;
}	t_tsr_texture;
