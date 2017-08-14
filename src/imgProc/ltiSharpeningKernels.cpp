/*
 * Copyright (C) 2007
 * Instituto Tecnológico de Costa Rica
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
 * \file   ltiSharpeningKernels.cpp
 *         Contains kernels for image sharpening 
 * \author Miguel Angel Aguilar Ulloa
 * \date   16.10.2007
 *
 * $Id: ltiSharpeningKernels.cpp,v 1.1 2008-02-18 00:17:23 alvarado Exp $
 */

#include "ltiSharpeningKernels.h"
#include "ltiGaussKernels.h"

namespace lti {
  // constructor
  sharpeningKernels::sharpeningKernels(const int kernelSize, const float A, 
   const eKernelType kernelType, const eNeighborType neighborType, const double& variance) 
    : kernel2D<float>() {
    generate(kernelSize, A, kernelType, neighborType, variance);
  }

  // constructor
  sharpeningKernels::sharpeningKernels(const int kernelSize, const float A, 
   const eKernelType kernelType, const double& variance) 
    : kernel2D<float>() {
    generate(kernelSize, A, kernelType, FourNeighbor, variance);
  }

  // generate filter
  void sharpeningKernels::generate(const int kernelSize, const float A, 
    const eKernelType kernelType, const eNeighborType neighborType, const double& variance) {
    
    // halfKernel
    int halfKernel = kernelSize/2;
    
    // choose the type of kernel
    switch(kernelType) {
      
      case Laplacian: {        
        // choose the type of neighbor
        switch(neighborType) {
          // four neighbor
          case FourNeighbor: {

            // resize kernel
            this->resize(-halfKernel,-halfKernel,halfKernel,halfKernel,0.0f,Init);

            // colum of -1's at center of kernel
            for(int row = -halfKernel; row <= halfKernel; ++row) {
              this->at(row,0) = -1.0f;
            }
            // row of -1's at center of kernel
            for(int col = -halfKernel; col <= halfKernel; ++col) {
              this->at(0,col) = -1.0f;
            }
            // center value
            this->at(0,0)=static_cast<float>(A + halfKernel*4);
            return;
          }
          // eight neighbor
          case EightNeighbor: {            
            // resize kernel
            this->resize(-halfKernel,-halfKernel,halfKernel,halfKernel,-1.0f,Init);
            // center value
            this->at(0,0)=static_cast<float>(A+sqrt(kernelSize)-1);
            return;
          }
        }
      }
      
      case Gaussian: {
        // allocate the space for the kernel
        this->resize(-halfKernel,-halfKernel,halfKernel,halfKernel,0.0f,Init);
        // define the gaussian kernel
        gaussKernel2D<float> gauss(kernelSize,variance); 
        // define a kernel2D
        kernel2D<float> kern;
        // cast from gaussian kernel to kernel2D
        kern.castFrom(gauss);
        // complement kernel
        for(int row = -halfKernel; row<= halfKernel; ++row) {
          for(int col = -halfKernel; col <= halfKernel; ++col) {
            this->at(row,col) = static_cast<float>(-kern.at(row,col));
          }
        }
        // central value
        this->at(0,0) = A + kern.at(0,0);
        return;
      }
 
      default: {
        clear();
      }
    }
  }

}
