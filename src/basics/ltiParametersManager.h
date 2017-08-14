/*
 * Copyright (C) 1998-2005
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
 * \file   ltiParametersManager.h
 *         Contains the class lti::parametersManager, which provides a standard
 *         functionality for all classes that manage parameters (e.g. functors,
 *         viewers, painters, classifiers, etc.)
 * \author Pablo Alvarado
 * \date   06.01.2005
 *
 * $Id: ltiParametersManager.h,v 1.8 2009-04-19 04:05:08 alvarado Exp $
 */

#ifndef _LTI_PARAMETERS_MANAGER_H_
#define _LTI_PARAMETERS_MANAGER_H_

#include "ltiIoObject.h"
#include "ltiInvalidParametersException.h"
#include "ltiIoHandler.h"
#include "ltiStatus.h"

namespace lti {

  /**
   * Base class for all classes with parameters.
   *
   * Many classes in the LTI-Lib make use of internal parameter
   * objects.  This has proven to be a very flexible concept, not only
   * useful in the serialization of the configuration of algorithms
   * used, but also extremely practical in the reuse of functors and
   * parameterizable objects within other parameterizable objects.
   *
   * The parameters can then be set in this classes with help of
   * several methods, where the setParameters() is the most important.
   *
   * @see parametersManager::parameters
   *
   * \ingroup gInterfaces
   */
  class parametersManager {
  public:

    /**
     * Base class for all lti parameter objects
     */
    class parameters : public ioObject {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      virtual ~parameters();

      /**
       * Copy data of "other" parameters
       */
      parameters& copy(const parameters& other);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const = 0;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const = 0;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* newInstance() const = 0;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);
      
    protected:
      /**
       * Copy data of "other" parameters
       */
      parameters& operator=(const parameters& other);
    };

    /**
     * Default constructor
     */
    parametersManager();

    /**
     * Copy constructor
     */
    parametersManager(const parametersManager& other);

    /**
     * Destructor
     */
    virtual ~parametersManager();

    /**
     * Set parametersManager's parameters.
     *
     * This member makes a copy of \a theParam: the parametersManager
     * will keep its own copy of the parameters!
     *
     * Additionally, the updateParameters will be called, which in
     * some classes is used to initialize some Look-Up Tables, or
     * filters, etc., in order to improve the execution times later in
     * the apply methods.
     * 
     * Since the LTI-Lib follows a deep-copy philosophy, if you copy
     * the functor or other from parametersManager inherited class,
     * the copy will create an exact copy of the parameters, no matter
     * if they have been specified with useParameters() or with
     * setParameters().
     *
     * You should always prefer this method over useParameters().  The
     * latter one is used only in very special cases, when the user
     * needs to control an instance of parameters outside the
     * functor/classifier or other inherited class, and he/she wants
     * to take care of the memory management.
     *
     * This method should never be overloaded.  If you want to make some
     * precomputations when setting the parameters, please overload
     * updateParameters() instead.
     *
     * @see useParameters(),updateParameters()
     *
     * @return true if successful, false otherwise
     */
    virtual bool setParameters(const parameters& theParam);

    /**
     * Use the given parameters exactly as they are.  
     *
     * The difference of this method with setParameters() is that the
     * parameters will \b not be copied.  They will be used exactly as
     * they are given.  It is therefore your responsability to ensure
     * that the parameters instance you give exists as long as the
     * parameterManager instace is being used.
     *
     * Of course, if the parameters content is changed while the
     * derived functor/classifier makes its computations,
     * unpredictable behaviour has to be expected.  It is also your
     * responsibility to take care of all multi-threading details.
     *
     * Since the LTI-Lib follows a deep-copy philosophy, if you copy
     * the classes inherited from parametersManager, the copy will
     * create an exact copy of the parameters, no matter if they have
     * been specified with useParameters() or with setParameters().
     *
     * The manager will not administrate the instance given through
     * this method.  If you create it with new, you have to take care
     * about the deletion.
     *
     * If you are not sure what to use, always prefer setParameters()
     * since it is more secure than useParameters(), as the
     * parametersManager will take care of all memory issues.
     *
     * This method always calls updateParameters().
     *
     * \warning Use this method only if you really know what you are doing.
     *          If you are not sure, use setParameters() instead.
     *
     * Note that the given parameters instance can be altered in the
     * derived classes, specially if some non-const shortcuts are used
     * or the read methods are called (another reason to prefer
     * setParameters()).
     *
     * @return true if successful, false otherwise
     */
    virtual bool useParameters(parameters& theParam);

    /**
     * Use the given parameters as they are but take control of the memory
     * management.
     *
     * This method is very similar to useParameters() and therefore you must
     * pay attention to all warnings specified in the documentation of that
     * method.  The main difference is that this method takes control of the
     * memory management of the given parameters instance.  In other words,
     * this method is used in extremely time critical situations when you must
     * avoid that the parameters are copied, but you also want this manager
     * to administrate the memory allocation for the given object.
     *
     * This method always calls updateParameters(), task that will determine if
     * here it is reported success or not.  This means, even if the method
     * returns false, the instance will have acquired control of the parameters
     * instance and you must not remove the given instance yourself.
     *
     * \warning Use this method only if you really know what you are doing.
     *          If you are not sure, use setParameters() instead.
     *
     * @return true if successful, false otherwise
     */
    virtual bool attachParameters(parameters& theParam);


    /**
     * Update parameters.
     *
     * Some functors (or other classes inherited from
     * parametersManagers) make some pre-computations when the
     * parameters are set.  For example, they initialize look-up
     * tables, build some filter kernels, initialize some data
     * structures, etc.  This job is always done by updateParameters().
     *
     * In principle, you only need to overload this method if you need such
     * initializations, as setParameters() and useParameters() will always
     * call this method.
     *
     * For disobedient people, who ignore the recomendation of using
     * setParameters() over useParameters(), this method should be
     * called if attributes of the parameters instance are changed,
     * before calling the apply() method.  Otherwise, the internal
     * functor state will be inconsistent with the parameters.
     *
     * The useParameters() interface is provided to solve some
     * critical efficiency issues in very complex functor structures
     * and helping in the implementation of GUI based tools, in which you maybe
     * want to change parameters on-line in an efficient way.
     *
     * \warning If you call an apply() method of a functor or other
     * parameter dependent methods in classifiers, in which you
     * changed the attributes of the parameters instance without
     * updating them, you have to expect an unpredictible behaviour:
     * segmentation fault, system crash, blue screens, etc. can occur
     * as the sizes of internal structures may not be consistent with
     * the given parameters.  This problem does not exist if you
     * indicate the parameters to be used with setParameters(), as the
     * parametersManager will manage an instance of its own.
     *
     * A typical updateParameters() reimplementation will look something like
     * \code
     *   bool yourClass::updateParameters() {
     *     // get the set of parameters just indicated by the user
     *     const parameters& par = getParameters();
     *     
     *     // do whatever you need to initialize with par
     *     bool success=true;
     *     ...
     *     // return true only if everything was ok.
     *     return success;
     *   }
     * \endcode
     *
     * The default implementation just returns true if the parameters have been
     * set before, which is always the normal case.
     */
    virtual bool updateParameters();

    /**
     * Returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * Returns true if the parameters are valid
     */
    virtual bool validParameters() const;

    /**
     * Copy data of "other" parametersManager.
     * Please note that the status string will _NOT_ be copied!
     */
    parametersManager& copy(const parametersManager& other);

    /**
     * Write the parametersManager in the given ioHandler. The default
     * implementation is to write just the parameters object.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,
                       const bool complete=true) const;

    /**
     * Read the parametersManager from the given ioHandler. 
     *
     * The default implementation is to read just the parameters object.
     *
     * Since this virtual method needs to know the exact type of the
     * parameters to call the proper read method, it will just assume
     * that the current parametersManager instance has a valid, consistent
     * parameter set.  If this is not the case, you need to
     * reimplement the read method to set first a dummy parameter
     * object or the correct type.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

  protected:
    /**
     * Returns current parameters. (non const! -> protected)
     */
    parameters& getRWParameters();

  private:

    /**
     * This is private to avoid default implementation.
     * 
     * alias for copy (MUST be reimplemented by each class or an
     * exception will be thrown!)
     */
    parametersManager& operator=(const parametersManager& other);

    /**
     * Current parameters.
     */
    parameters* params_;

    /**
     * Flag that indicates if the parametersManager owns the parameters or not
     */
    bool ownParams_;

  };
} // namespace lti

#endif

