/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017,2018,2019,2022 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 * Copyright © 2021 W. J. van der Laan <laanwj@protonmail.com>
 *
 * This file is part of GNU Mes.
 *
 * GNU Mes is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * GNU Mes is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <mes/lib.h>
#include <linux/syscall.h>
#include <arch/syscall.h>
#include <fcntl.h>

int
unlink (char const *file_name)
{
  long long_file_name = cast_charp_to_long (file_name);
#if defined (SYS_unlink)
  return _sys_call1 (SYS_unlink, long_file_name);
#elif defined (SYS_unlinkat)
  return _sys_call3 (SYS_unlinkat, AT_FDCWD, long_file_name, 0);
#else
#error No usable unlink syscall
#endif
}
