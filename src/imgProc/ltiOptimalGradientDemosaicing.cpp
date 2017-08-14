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


/* 
 * \file   ltiOptimalGradientDemosaicing.cpp
 *         Contains the class optimalGradientDemosaicing, a Bayer
 *         demosaicing using an optimized combination of interpolation
 *         and gradients.
 * \author Peter Doerfler
 * \date   14.12.2004
 *
 * revisions ..: $Id: ltiOptimalGradientDemosaicing.cpp,v 1.7 2008-10-21 16:15:22 alvarado Exp $
 */

#include "ltiOptimalGradientDemosaicing.h"
#include "ltiFactory.h"

namespace lti {

  //register in bayerDemosaicing factory
  _LTI_REGISTER_IN_FACTORY(bayerDemosaicing,optimalGradientDemosaicing)

  // --------------------------------------------------
  // optimalGradientDemosaicing::parameters
  // --------------------------------------------------

  // default constructor
  optimalGradientDemosaicing::parameters::parameters()
    : bayerDemosaicing::parameters() {
    
  }

  // copy constructor
  optimalGradientDemosaicing::parameters::parameters(const parameters& other)
    : bayerDemosaicing::parameters() {
    copy(other);
  }

  // destructor
  optimalGradientDemosaicing::parameters::~parameters() {
  }

  // get type name
  const std::string& optimalGradientDemosaicing::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  optimalGradientDemosaicing::parameters&
  optimalGradientDemosaicing::parameters::copy(const parameters& other) {
    bayerDemosaicing::parameters::copy(other);

    

    return *this;
  }

  // alias for copy method
  optimalGradientDemosaicing::parameters&
  optimalGradientDemosaicing::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone method
  optimalGradientDemosaicing::parameters* optimalGradientDemosaicing::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  optimalGradientDemosaicing::parameters* optimalGradientDemosaicing::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool optimalGradientDemosaicing::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
    }

    b = b && bayerDemosaicing::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool optimalGradientDemosaicing::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
    }

    b = b && bayerDemosaicing::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // optimalGradientDemosaicing
  // --------------------------------------------------

  // default constructor
  optimalGradientDemosaicing::optimalGradientDemosaicing()
    : bayerDemosaicing() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  optimalGradientDemosaicing::optimalGradientDemosaicing(const parameters& par)
    : bayerDemosaicing() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  optimalGradientDemosaicing::optimalGradientDemosaicing(const optimalGradientDemosaicing& other)
    : bayerDemosaicing() {
    copy(other);
  }

  // destructor
  optimalGradientDemosaicing::~optimalGradientDemosaicing() {
  }

  // returns the name of this type
  const std::string& optimalGradientDemosaicing::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  optimalGradientDemosaicing& 
  optimalGradientDemosaicing::copy(const optimalGradientDemosaicing& other) {
    bayerDemosaicing::copy(other);

    return (*this);
  }

  // alias for copy member
  optimalGradientDemosaicing&
  optimalGradientDemosaicing::operator=(const optimalGradientDemosaicing& other) {
    return (copy(other));
  }

  // clone member
  optimalGradientDemosaicing* optimalGradientDemosaicing::clone() const {
    return new optimalGradientDemosaicing(*this);
  }

  // newInstance member
  optimalGradientDemosaicing* optimalGradientDemosaicing::newInstance() const {
    return new optimalGradientDemosaicing();
  }

  // return parameters
  const optimalGradientDemosaicing::parameters&
  optimalGradientDemosaicing::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  
  // On copy apply
  bool optimalGradientDemosaicing::apply(const matrix<ubyte>& src,
                                         image& dest) const {


    const eBayerPattern pat=getParameters().bayerPattern;

    if (pat==NoBayer) {
      return generateGreyImage(src, dest);
    }

    bool blueRow;
    bool blueLeft;

    switch (pat) {
      case RGGB:
        blueRow=false;
        blueLeft=false;
        break;
      case BGGR:
        blueRow=true;
        blueLeft=true;
        break;
      case GRBG:
        blueRow=false;
        blueLeft=true;
        break;
      case GBRG:
        blueRow=true;
        blueLeft=false;
        break;
      default:
        // severe bug
        setStatusString
          ("unknown bayerPattern in optimalGradientDemosaicing\n");
        dest.clear();
        return false;
    }


    // the destination image is two pixels smaller all around
    const int scols=src.columns();
    const int scols2=2*scols;
    const int scolsm=scols-1;
    const int scolsp=scols+1;
    const int dcols=scols-4;
    const int drows=src.rows()-4;
    dest.allocate(drows, dcols);
    vector<rgbaPixel>::iterator dit, dite;
    matrix<ubyte>::const_iterator sit=src.begin()+scols2+2;
    int i;
    const double fac=3./16.;


    for (i=0; i<drows; i++) {
      dit = dest.getRow(i).begin();
      dite = dest.getRow(i).end();

      if (blueRow) {

        if (blueLeft) {
          if (dit!=dite) {
            const ubyte b=*sit;
            const int blaplace=4*b-
              ((*(sit-scols2))+(*(sit-2))+(*(sit+2))+(*(sit+scols2)));
            const double gmean=0.25*((*(sit-scols))+(*(sit-1))
                                    +(*(sit+1))+(*(sit+scols)));
            const double rmean=0.25*((*(sit-scolsp))+(*(sit-scolsm))
                                    +(*(sit+scolsm))+(*(sit+scolsp)));
            (*dit).set(clip(rmean+fac*blaplace), 
                       clip(gmean+0.125*blaplace),
                       b);
            ++dit;
            ++sit;
          }
        }

        for (; dit!=dite; ++dit, ++sit) {
          const ubyte g=*sit;
          const int glaplace=5*g-
            ((*(sit-scolsp))+(*(sit-scolsm))
             +(*(sit+scolsm))+(*(sit+scolsp)));
          const int gcol=(*(sit-scols2))+(*(sit+scols2));
          const int grow=(*(sit-2))+(*(sit+2));
          const double bmean=0.5*((*(sit-1))+(*(sit+1)));
          const double rmean=0.5*((*(sit-scols))+(*(sit+scols)));
          (*dit).set(clip(rmean+0.125*(glaplace-gcol+0.5*grow)),
                     g,
                     clip(bmean+0.125*(glaplace-grow+0.5*gcol)));
          ++dit; ++sit;

          if (dit!=dite) {
            const ubyte b=*sit;
            const int blaplace=4*b-
              ((*(sit-scols2))+(*(sit-2))+(*(sit+2))+(*(sit+scols2)));
            const double gmean=0.25*((*(sit-scols))+(*(sit-1))
                                    +(*(sit+1))+(*(sit+scols)));
            const double rmean2=0.25*((*(sit-scolsp))+(*(sit-scolsm))
                                    +(*(sit+scolsm))+(*(sit+scolsp)));
            (*dit).set(clip(rmean2+fac*blaplace), 
                       clip(gmean+0.125*blaplace),
                       b);
          } else {
            break;
          }
        }


      } else {

        if (blueLeft) {
          if (dit!=dite) {
            const ubyte g=*sit;
            const int glaplace=5*g-
              ((*(sit-scolsp))+(*(sit-scolsm))
               +(*(sit+scolsm))+(*(sit+scolsp)));
            const int gcol=(*(sit-scols2))+(*(sit+scols2));
            const int grow=(*(sit-2))+(*(sit+2));
            const double rmean=0.5*((*(sit-1))+(*(sit+1)));
            const double bmean=0.5*((*(sit-scols))+(*(sit+scols)));
            (*dit).set(clip(rmean+0.125*(glaplace-grow+0.5*gcol)),
                       g,
                       clip(bmean+0.125*(glaplace-gcol+0.5*grow)));
            ++dit;
            ++sit;
          }
        }

        for (; dit!=dite; ++dit, ++sit) {
          const ubyte r=*sit;
          const int rlaplace=4*r-
            ((*(sit-scols2))+(*(sit-2))+(*(sit+2))+(*(sit+scols2)));
          const double gmean=0.25*((*(sit-scols))+(*(sit-1))
                                  +(*(sit+1))+(*(sit+scols)));
          const double bmean=0.25*((*(sit-scolsp))+(*(sit-scolsm))
                                  +(*(sit+scolsm))+(*(sit+scolsp)));
          (*dit).set(r, 
                     clip(gmean+0.125*rlaplace),
                     clip(bmean+fac*rlaplace));
          ++dit; ++sit;

          if (dit!=dite) {
            const ubyte g=*sit;
            const int glaplace=5*g-
              ((*(sit-scols2))+(*(sit-2))+(*(sit+2))+(*(sit+scols2)));
            const int gcol=(*(sit-scols2))+(*(sit+scols2));
            const int grow=(*(sit-2))+(*(sit+2));
            const double rmean=0.5*((*(sit-1))+(*(sit+1)));
            const double bmean2=0.5*((*(sit-scols))+(*(sit+scols)));
            (*dit).set(clip(rmean+0.125*(glaplace-grow+0.5*gcol)),
                       g,
                       clip(bmean2+0.125*(glaplace-gcol+0.5*grow)));
          } else {
            break;
          }
        }
                     
      }
      //advance to next row
      sit+=4;
      blueRow = blueRow ? false : true;

    }

    return true;
  }



}

