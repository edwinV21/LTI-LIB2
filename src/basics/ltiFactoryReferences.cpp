/*
 * Copyright (C) 1998-2007
 * Pablo Alvarado
 * Electronics Engineering School, ITCR, Costa Rica
 *
 * This file is part of the Computer Vision and Robotics Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * \file   ltiFactoryReferences.cpp
 *         Class that ensures the existence of some references.
 * \author Pablo Alvarado
 * \date   14.01.2006
 */

/*
 * This file has been automatically generated from the sources of
 * the library with the script ./searchFactoryRefs
 */

#include "ltiAscii85Codec.h"
#include "ltiAsciiHexCodec.h"
#include "ltiBilinearDemosaicing.h"
#include "ltiBinarizationCodec.h"
#include "ltiFlateCodec.h"
#include "ltiGridSampling.h"
#include "ltiIdentityCodec.h"
#include "ltiIOBMP.h"
#include "ltiIOJPEG.h"
#include "ltiIOLTI.h"
#include "ltiIOPNG.h"
#include "ltiLinearKernel.h"
#include "ltiMergeHSIToImage.h"
#include "ltiMergeHSVToImage.h"
#include "ltiMergeRGBToImage.h"
#include "ltiMergeYIQToImage.h"
#include "ltiMergeYPbPrToImage.h"
#include "ltiMergeYUVToImage.h"
#include "ltiMlp.h"
#include "ltiNearestNeighborDemosaicing.h"
#include "ltiNoDemosaicing.h"
#include "ltiNormalDistribution.h"
#include "ltiOptimalGradientDemosaicing.h"
#include "ltiPolynomialKernel.h"
#include "ltiRadialKernel.h"
#include "ltiRunLengthCodec.h"
#include "ltiSiftSampling.h"
#include "ltiSigmoidKernel.h"
#include "ltiSliceSampling.h"
#include "ltiSplitImageToCIELab.h"
#include "ltiSplitImageToHSI.h"
#include "ltiSplitImageToHSV.h"
#include "ltiSplitImageToRGB.h"
#include "ltiSplitImageTorgI.h"
#include "ltiSplitImageToXYZ.h"
#include "ltiSplitImageToYCbCr.h"
#include "ltiSplitImageToYIQ.h"
#include "ltiSplitImageToYPbPr.h"
#include "ltiSplitImageToYUV.h"
#include "ltiUniformContinuousDistribution.h"
#include "ltiUniformDiscreteDistribution.h"
#include "ltiUnivariateContinuousDistribution.h"

#include "ltiFactoryReferences.h"

namespace lti {

  factoryReferences::factoryReferences() {
    // This constructor is called only once for each factory, and we do not
    // want to create unnecessarily the objects.  To avoid that the linker 
    // removes the references, we "fool" it with some heuristics...
    //
    // We construct a simple second order oscilator, with poles at +/- j,
    // and check for a value that will never occur
    static int y[3] = {0,0,2}; // y(n-2)=0,y(n-1)=0,y(n)=2

    if (y[2] == 1) { // this condition will never be fulfilled
      // however, since the linker cannot anticipate it, it has to keep
      // all references to the following objects.
      mlp::linearActFunctor mlp__linearActFunctor_;
      mlp::sigmoidFunctor mlp__sigmoidFunctor_;
      mlp::signFunctor mlp__signFunctor_;
      bilinearDemosaicing bilinearDemosaicing_;
      gridSampling gridSampling_;
      mergeHSIToImage mergeHSIToImage_;
      mergeHSVToImage mergeHSVToImage_;
      mergeRGBToImage mergeRGBToImage_;
      mergeYIQToImage mergeYIQToImage_;
      mergeYPbPrToImage mergeYPbPrToImage_;
      mergeYUVToImage mergeYUVToImage_;
      nearestNeighborDemosaicing nearestNeighborDemosaicing_;
      noDemosaicing noDemosaicing_;
      optimalGradientDemosaicing optimalGradientDemosaicing_;
      siftSampling siftSampling_;
      sliceSampling sliceSampling_;
      splitImageToCIELab splitImageToCIELab_;
      splitImageToHSI splitImageToHSI_;
      splitImageToHSV splitImageToHSV_;
      splitImageToRGB splitImageToRGB_;
      splitImageTorgI splitImageTorgI_;
      splitImageToXYZ splitImageToXYZ_;
      splitImageToYCbCr splitImageToYCbCr_;
      splitImageToYIQ splitImageToYIQ_;
      splitImageToYPbPr splitImageToYPbPr_;
      splitImageToYUV splitImageToYUV_;
      ascii85Codec ascii85Codec_;
      asciiHexCodec asciiHexCodec_;
      binarizationCodec binarizationCodec_;
#if HAVE_LIBZ
      flateCodec flateCodec_;
#endif
      identityCodec identityCodec_;
      ioBMP ioBMP_;
#if defined(HAVE_LIBJPEG) 
      ioJPEG ioJPEG_;
#endif
      ioLTI ioLTI_;
#if defined(HAVE_LIBPNG)
      ioPNG ioPNG_;
#endif
      runLengthCodec runLengthCodec_;
      linearKernel linearKernel_;
      normalDistribution normalDistribution_;
      polynomialKernel polynomialKernel_;
      radialKernel radialKernel_;
      sigmoidKernel sigmoidKernel_;
      uniformContinuousDistribution uniformContinuousDistribution_;
      uniformDiscreteDistribution uniformDiscreteDistribution_;
      univariateContinuousDistribution univariateContinuousDistribution_;
    }
    // update time
    y[0] = y[1];
    y[1] = y[2];
    y[2] = -y[0];
  }
}
