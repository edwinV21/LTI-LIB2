/*
 * Copyright (C) 1998-2006
 * Electronics Engineering School, ITCR, Costa Rica
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
 * \file   ltiFactoryReferences.h
 *         Class that ensures the existence of some references.
 * \author Gustavo Quiros
 * \date   08.07.2004
 *
 * $Id:
 */

#ifndef _LTI_FACTORY_REFERENCES_H_
#define _LTI_FACTORY_REFERENCES_H_


namespace lti {

  /**
   * Factory references.
   *
   * This class is sort of a place holder for references to classes that
   * can be constructed by means of lti::factory<T>.
   *
   * It is constructed statically by each factory and therefore the
   * constructor has to be written in a way that forces the references
   * to all constructable classes.
   *
   * The Makefile takes care of updating the source file automatically.<
   */
  class factoryReferences {   
  public:
    /**
     * Constructor has to create the references.
     */
    factoryReferences();
  };

}  

#endif
