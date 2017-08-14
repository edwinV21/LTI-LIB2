/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiTimer.h
 *         Contains the class lti::timer to measure wall-clock times with
 *         microsecond precision.
 * \author Pablo Alvarado
 * \date   12.06.2000
 *
 * $Id: ltiTimer.h,v 1.7 2010-09-11 23:32:33 alvarado Exp $
 */

#ifndef _LTI_TIMER_H
#define _LTI_TIMER_H

#include "ltiObject.h"
#include "ltiMacroSymbols.h"
#include <string>

namespace lti {

  /** 
   * The timer allows to measure time with a precision of about 30us
   * on Linux systems and a value dependent on the performance counter
   * on Windows systems. It seems like the precision for CPU time is
   * 1ms on Windows systems. The elapsed time is returned in
   * microseconds.
   *
   * Depending on the eTimeType (setTimeType()) The CPU time (Cpu) or
   * wall-clock time (Wall) are measured. The default is Cpu.
   *
   * The maximum time that can be measured with this function in Wall
   * mode is 1 day (86.4E+09 microseconds).  To measure longer time
   * intervalls use the standard time() function.
   *
   * Example:
   *
   * \code
   *
   * lti::timer chron;
   *
   * chron.start();
   *
   * // do something
   *
   * chron.stop();
   *
   * std::cout << "something takes " << chron.getTime() << " microseconds\n";
   *
   * \endcode
   */
  class timer : public object {
  public:

    /**
     * Determines which time is measured.
     */
    enum eTimeType {
      Wall, /**< Use Wall-time */
      Cpu  /**< Use CPU time */
    };

    /**
     * Constructor to set the time type.
     *
     * If the time type is not given, CPU time is assumed
     */
    timer(const eTimeType& timeType=Cpu);

    /**
     * Copy constructor
     */
    timer(const timer& other);

    /**
     * Destructor
     */
    virtual ~timer();

    /**
     * Start the timer
     */
    void start();

    /**
     * Stop the timer
     */
    void stop();

    /** 
     * Get the elapsed time (in microsecond) between start() and stop() or the
     * actual time (if stop() is not been called yet!)
     *
     * @return Elapsed time
     */
    double getTime() const;

    /**
     * Sets the time type.
     * 
     * @param timeType the new eTimeType
     */
    void setTimeType(const eTimeType& timeType);

    /**
     * Returns the current time type.
     *
     * @return the current eTimeType.
     */
    const eTimeType& getTimeType();

    /**
     * Return a string containing the current time. Obviously this
     * always uses the Wall time.
     *
     * The format is the one return by the libc function \c ctime, for example
     * \code
     * Sat Jul 24 22:46:18 CEST 2004
     * \endcode
     * 
     */
    static std::string getDateAndTime();

    /**
     * Returns the current CPU time in usec. Don't use this function
     * for time measurement. Use start(), stop(), getTime() instead.
     */
    static double getCpuTime();

    /**
     * Class name
     */
    virtual const std::string& name() const;

    /**
     * Clone method.
     */
    virtual timer* clone() const;

    /**
     * New instance method.
     */
    virtual timer* newInstance() const;

  protected:

    /**
     * Time type used.
     */
    eTimeType timeType_;

    /**
     * Time at which start() was called.
     */
    double startTime_;

    /**
     * Time at which stop() was called.
     */
    double endTime_;

    /**
     * Flag to indicate if start() was called, but not stop().
     */
    bool started_;

    /**
     * Get actual time.
     *
     * The implementation of this method depends on the OS.
     */
    double getActualTime() const;

# ifdef _LTI_MSC_VER
  private:
    /**
     * \name MS VC++ version
     */
    //@{
    /**
     * Constant used to represent in a floating point representation the
     * value 2^32
     */
    static const double max32bit_;

    /**
     * Frequency (Hz) of the performance counter
     */
    double freq_;
    //@}
# endif

  };
}

#endif

