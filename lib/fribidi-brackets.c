/* fribidi-brackets.c - get bracketed character
 *
 * Copyright (C) 2004  Sharif FarsiWeb, Inc
 * Copyright (C) 2001, 2002, 2004  Behdad Esfahbod
 * Copyright (C) 1999, 2000  Dov Grobgeld
 *
 * This file is part of GNU FriBidi.
 * 
 * GNU FriBidi is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * GNU FriBidi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GNU FriBidi; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * For licensing issues, contact <license@farsiweb.info> or write to
 * Sharif FarsiWeb, Inc., PO Box 13445-389, Tehran, Iran.
 */
/* 
 * Author(s):
 *   Behdad Esfahbod, 2001, 2002, 2004
 *   Dov Grobgeld, 1999, 2000, 2017
 */

#include "common.h"

#include <fribidi-brackets.h>

#include "brackets.tab.i"
#include "brackets-type.tab.i"
#include <stdio.h>

FRIBIDI_ENTRY fribidi_boolean
fribidi_get_bracket (
  /* input */
  FriBidiChar ch,
  /* output */
  fribidi_uint8 *bracketed_type,
  FriBidiChar *bracketed_ch
)
{
  register fribidi_uint8 char_type;
  char_type = FRIBIDI_GET_BRACKET_TYPE (ch);
  if (char_type && bracketed_ch)
      *bracketed_ch = FRIBIDI_GET_BRACKETS (ch);

  if (bracketed_type)
    *bracketed_type = char_type;
  return char_type != 0;
}

/* Editor directions:
 * Local Variables:
 *   mode: c
 *   c-basic-offset: 2
 *   indent-tabs-mode: t
 *   tab-width: 8
 * End:
 * vim: textwidth=78: autoindent: cindent: shiftwidth=2: tabstop=8:
 */
