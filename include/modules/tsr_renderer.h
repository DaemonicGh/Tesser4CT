/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_renderer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:33:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/15 17:00:29 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_RENDERER_H
# define TSR_RENDERER_H

# include "mbx.h"
# include "types/tsr_s_tsr.h"
# include "modules/tsr_world.h"

void	vec3_yaw_rot(t_vec3 *v, double ang);
void	vec3_pitch_rot(t_vec3 *v, double ang);
void	setup_frag(t_mbx_region *buffer, t_tsr *tsr);

#endif
