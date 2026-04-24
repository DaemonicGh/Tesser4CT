/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_s_tsr.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:46:46 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/24 17:26:59 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_S_TSR_H
# define TSR_S_TSR_H

# include "tsr_s_world.h"

typedef struct s_tsr
{
	t_vec2i		size;
	t_vec3		pos;
	t_vec3		vel;
	t_vec2		ang;
	t_world		world;
	t_vec3		light;
	t_mbx_atlas	*font;
	t_mbx_region	*tex[256];
}	t_tsr;

#endif
