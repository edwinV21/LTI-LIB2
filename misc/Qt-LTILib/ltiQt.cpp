/*
 * Copyright (C) 2012
 * Pablo Alvarado-Moya
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
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

/**
 * \file   ltiQt.cpp
 *         Simple functions used with Qt to interface with the LTI-Lib
 *
 * \author Pablo Alvarado
 * \date   27.10.2012
 *
 * revisions ..: $Id: ltiQt.cpp,v 1.1 2012-10-28 03:18:27 alvarado Exp $
 */

#include <ltiQt.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 * Convert lti::image to QImage
 */
QImage convertImageToQImage(const lti::image& src) {
  const int rows=src.rows();
  const int cols=src.columns();
  QImage dest(cols,rows, QImage::Format_ARGB32_Premultiplied);

  for (int y=0;y<rows;++y) {
    const lti::vector<lti::rgbaPixel>& srcRow = src.getRow(y);
    QRgb* destRow = (QRgb*)dest.scanLine(y);
    for (int x=0;x<cols;++x) {
      const lti::rgbaPixel& px = srcRow.at(x);
      destRow[x] = qRgba(px.red,px.green,px.blue,255);
    }
  }

  return dest;
}

/*
 * Convert lti::image to QPixmap
 */
QPixmap convertImageToQPixmap(const lti::image& src) {
  QImage qtI = convertImageToQImage(src);
  return QPixmap::fromImage(qtI,0);
}
