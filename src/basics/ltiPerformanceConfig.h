/*
 * Copyright (C) 1998-2004
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */

/**
 * \file   ltiPerformanceConfig.h
 *         Definition of constants that are used to tweak the performance of
 *         some algorithm but need not be changed for every call of a
 *         method. Usually, these constants are architecture dependent,
 *         mostly cache. The default values in this file were determined on
 *         a P4 1600 with 512 RAM.
 * \author Peter Doerfler
 * \date   09.02.2004
 *
 * $Id: ltiPerformanceConfig.h,v 1.4 2004-11-19 16:27:50 gquiros Exp $
 */

#ifndef _LTI_PERFORMANCE_CONFIG_H_
#define _LTI_PERFORMANCE_CONFIG_H_

/**
 * Multiplication of two matrices in lti::matrix is faster if the
 * second matrix is transposed first when that matrix is large. Large
 * is defined below.
 */
#define _LTI_PERFORMANCE_MATRIX_MATRIX_MULTIPLY 65

/**
 * The QR decomposition algorithm (lti::qrDecomposition) accesses the
 * data matrix column wise. Depending on the size it is faster to
 * transpose first and transpose the results again.
 */
#define _LTI_PERFORMANCE_QR_DECOMPOSITION 50

/**
 * lti::sort uses quicksort down to a certain number of elements to
 * sort after which it uses a simple sorting algorithm. The value
 * below sets that threshold.
 */
#define _LTI_PERFORMANCE_SORT_STOP_QUICKSORT 10

/**
 * The segment size of the heap in a smallObjectList. Indicates the
 * amount of nodes that will be allocated at once when there is no
 * more available memory.
 */
#define _LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE 100

/**
 * Object size threshold value for lti::list (in bytes).
 * The list will be implemented as a smallObjectList for types
 * whose size is less than or equal to this value, and as a
 * std::list for all other types.
 */
#define _LTI_PERFORMANCE_LIST_OBJECT_SIZE_THRESHOLD 16

#endif
