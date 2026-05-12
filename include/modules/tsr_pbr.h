/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_pbr.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 17:53:07 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/12 02:45:47 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_PBR_H
# define TSR_PBR_H

# include "mbx.h"

typedef struct s_pbr
{
	t_mbx_region	*col_tex;
	t_mbx_region	*nrm_tex;
}	t_pbr;

#endif
