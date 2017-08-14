/*
 * Copyright (C) 2010
 * Pablo Alvarado
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
 * \file   ltiViewer1D.h
 *         Contains the class lti::viewer1D used to visualized images and 
 *         channels.
 * \author Pablo Alvarado
 * \date   05.01.2005
 *
 * revisions ..: $Id: ltiViewer1D.h,v 1.1 2010-01-19 03:34:40 alvarado Exp $
 */

#ifndef _LTI_VIEWER_1_D_H_
#define _LTI_VIEWER_1_D_H_

#include "ltiRGBAPixel.h"
#include "ltiViewerBase.h"
#include "ltiPoint.h"
#include "ltiRGBPixel.h"
#include "ltiMatrix.h"
#include "ltiViewer1DPainter.h"

#include <limits>

namespace lti {

  /**
   * Class viewer1D.
   *
   * Viewer class to display vectors.  It can be used to display vectors from
   * the STL and the LTI-Lib as well, whose value_type is a scalar type 
   * that can be casted to double.
   *
   * As default behaviour you can get the configuration window pressing
   * the right button of the mouse, but you can change this behaviour setting
   * the parameters accordingly.
   *
   * If you like the way the viewer displays your data, and you would like
   * the images, then you may want to take a look at lti::viewer1DPainter, the
   * class responsible for painting the images you see.
   *
   * \section Example
   * 
   * A simple example of use follows:
   *
   * \code
   * lti::ivector vct(256,0);
   *
   * // fill the vector with whatever contents you want
   *
   * lti::viewer1D viewer("What a view");
   * viewer.show(vct);                         // show your favorite image
   *
   * lti::viewer1D::interaction action;
   * viewer.waitKeyPressed(action); // wait for a key being pressed
   * \endcode
   *
   * \warning: You may experience some problems while debugging your code if
   * you use viewers.  We are looking for the problem, but as a simple
   * workaround you just need to give the main loop enough time to initialize.
   * Just insert following lines before any forks, or threads or viewers are
   * created:
   *
   * \code
   *   guiServer::start();
   *   passiveWait(250000); // wait 1/4 of a second in debug mode
   * \endcode
   *
   * @ingroup gVisualization
   */
  class viewer1D : public viewerBase {
  public:

    /**
     * There are parameters related with the GUI, for example
     * which interaction configuration starts the config dialog, the
     * position of the window, etc.  There are other parameters that
     * configure options about the image painting, which depend on the
     * lower classes.  The former are inherited from viewerBase::parameters,
     * the latter from viewer1DPainter::parameters.
     */
    class parameters : virtual public parametersManager::parameters,
                       public viewerBase::parameters,
                       public viewer1DPainter::parameters {
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
       * Copy data of "other" parameters
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* newInstance() const;

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

      //
      // Attributes
      //
      
      /**
       * Filename used to save the images.
       *
       * Default value: tux.png
       */
      std::string filename;

    };
    // end of parameters class

    /*
     * Private data type to flag if the vector stored is floating point or
     * integer type.
     */
    enum eDataType {
      Unknown,
      Fixed,
      Float
    };

    /**
     * Information block
     * 
     * This is the parent class for all information blocks used to transmit
     * some data to the configuration dialogs, which are to be displayed in a
     * read-only manner.
     *
     * It has a simplified interface if compared with lti::object
     */
    class information : public viewerBase::information {
    public:
      /**
       * Default constructor
       */
      information();

      /**
       * Copy constructor
       */
      information(const information& other);

      /**
       * Virtual destructor
       */
      virtual ~information();

      /**
       * Copy method
       */
      information& copy(const information& other);

      /**
       * Clone 
       */
      virtual information* clone() const;


      /**
       * Initialize all attributes from the given vector
       */
      template<class V>
      void compute(const V& vct);

      /**
       * Data type being displayed.
       *
       * It indicates if the data contained was a floating point type (float or
       * double) or a fixed point one (byte, int, etc.).
       */
      eDataType type;

      /**
       * Size of the displayed image
       */
      int size;

      /**
       * Minimum value
       */
      double min;

      /**
       * Maximum value
       */
      double max;

      /**
       * Average value
       */
      double average;

      /**
       * Standard deviation 
       */
      double stdDeviation;     
    }; 
    // end of class information

    // ----------------------------------------------------------------------
    // viewer1D
    // ----------------------------------------------------------------------

    /**
     * Constructor
     */
    viewer1D(const std::string& title="Unknown");

    /**
     * Copy constructor.
     *
     * Create a new viewer, with its own window and configuration dialog, with
     * the same content as the other viewer.  If that viewer is still inactive,
     * this viewer will also be.
     */
    viewer1D(const viewer1D& other);

    /**
     * Constructor. Sets the given \p params
     */
    viewer1D(const parameters& params);

    /**
     * Destructor
     */
    ~viewer1D();

    /**
     * Returns a copy of this object
     */
    virtual viewer1D* clone() const;

    /**
     * Returns a copy of this object
     */
    virtual viewer1D* newInstance() const;

    /**
     * Returns a copy of this object
     */
    viewer1D& copy(const viewer1D& other);

    /**
     * Return a read-only copy of the parameters
     */
    const parameters& getParameters() const;

    /**
     * @name Show images or matrices
     */
    //@{

    /**
     * Show the given vector
     * @param vct the vector be displayed.
     * @return true if successful, false otherwise
     */
    template<class V>
    bool show(const V& vct);

    //@}

    /**
     * The user can specify a viewer1DPainter object.
     *
     * Usually you will only use this method if you want to completely change
     * the way 1D data is displayed, beyond the configuration posibilities
     * through parameters, which is only possible if you create your own
     * lti::viewer1DPainter derived class.
     *
     * A clone of the given painter will be done.
     *
     * If you derive a class of lti::viewer1D and want to always set your own
     * painter, then you should only overload the method newDefaultPainter()
     */
    virtual bool setPainter(const viewer1DPainter& painter);

    /**
     * Save button pressed.
     *
     * Method called when the user pressed the save button.
     * Usually this action just activates calling viewer1D::saveButtonPressed();
     *
     * Note that if you call this method you can force from the code that the
     * displayed image is saved at the location specified as argument.
     */
    virtual bool saveButtonPressed(const std::string& filename);

    /**
     * Save all button pressed.
     *
     * Method called when the user pressed the save all button.  Usually this
     * action just activates calling viewer1D::saveAllButtonPressed();
     *
     * Note that if you call this method you can force from the code that the
     * displayed image is saved at the location specified as argument.
     */
    virtual bool saveAllButtonPressed(const std::string& filename);

    /**
     * Get a read-only reference to the internal information block.
     *
     * This can return a wrong reference.  You should check validInformation()
     * first.
     */
    const information& getInformation() const;

  protected:

    /**
     * Return a pointer to a new instance of the default painter you want
     * to use.
     */
    virtual viewer1DPainter* newDefaultPainter() const;

    /**
     * Get a writable reference to the internal information block
     *
     * This can return a wrong reference.  You should check validInformation()
     * first.
     */
    information& getInformation();


    /**
     * Ensure that the current information block has the correct data size.
     * This saves time getting the information and allows the viewer more speed
     * if a sort of video-out use is planned.
     *
     * Returns false if it already had the given type or true if it was
     * changed.  This is important to know in order to setup the configuration
     * dialog accordingly.
     */
    bool ensureInformationType(const eDataType& type,
                               const int& vectorSize); // REMOVE?
    
    /**
     * This virtual method is called just before the configuration dialog
     * is to be displayed, in order to update the information block to the
     * data being processed.
     */
    virtual bool updateInformation();

    /**
     * Determine if the given position is valid
     */
    virtual bool validPosition(const ipoint& position) const;

    /**
     * Mouse/Keyboard interaction handler.
     *
     * This virtual function can be completely overriden and is called each
     * time some interaction with the main window occurs.  The default 
     * behaviour is to display the position of the mouse pointer within the
     * window in the status bar.
     *
     * Usually, derived classes override this method to intercept the
     * interaction used to display some information under the mouse pointer.
     */
    virtual bool onKMInteraction(const interaction& action,
                                 const ipoint& position);

    /**
     * Protect access to critical data
     */
    mutex lock_;

    //
    // ----------------------------------------------------------------------
    //

    /**
     * Main window of the 1D viewer
     *
     * The only task for this class is to show a 1D color lti::image in the
     * window taking care of the scrollbars.  The corresponding painter
     * (lti::viewer1DPainter) is the one in charge of painting anything into
     * the lti::image.
     */
    class mainWindow : public viewerBase::mainWindow {
    public:
      /**
       * Main window constructor
       */
      mainWindow(viewer1D& parent);

      /**
       * Get information block of the parent class
       */
      const information& getInformation() const;

      /**
       * Get data as an image for the window indicated by area
       */
      bool getFormattedData(const irectangle& area,
                            image& img);

      /**
       * Get all data as an image
       */
      bool getFormattedData(image& img);

      /**
       * Compute the new canvas size.
       *
       * Nothing but the computation has to be done here.  If it is applied or
       * not will be a decision of the setCanvasSize() method.
       *
       * The viewer1D computes the size as the product of zoom times the image
       * size.
       */
      virtual ipoint computeCanvasSize() const;

      /**
       * Method called when the user pressed the save button.
       *
       * (Implemented in toolkit dependent files, as it needs some
       * collaboration with the GUI to detect the current window configuration)
       */
      virtual bool saveButtonPressed(const std::string& filename);

      /**
       * Method called when the user pressed the save button.
       *
       * (Implemented in toolkit dependent files, as it needs some
       * collaboration with the GUI to detect the current window configuration)
       */
      virtual bool saveAllButtonPressed(const std::string& filename);

    protected:
      /**
       * Private class used to contain all internals based on each specific
       * widget toolkit
       */
      class tk;

      /**
       * Initialize toolkit dependent class
       *
       * This is called at construction time but is implemented by the 
       * toolkit being used.
       */
      virtual void initToolkit();

      /**
       * Destroy toolkit dependent class
       *
       * This is called at destruction time and is implemented by the 
       * toolkit being used.
       */
      virtual void destroyToolkit();

    };


    /**
     * Ensure that a main window of the proper type has been created.
     * 
     * @return true if a new mainWindow instance was created or false if
     *         it already existed.
     */
    virtual bool ensureMainWindow();

    /**
     * Return the main window as read-only reference.
     *
     * This will throw an assertion if the mainWindow has not been set yet.
     */
    const mainWindow& getMainWindow() const;

    /**
     * Return the main window as reference.
     *
     * This will throw an assertion if the mainWindow has not been set yet.
     */
    mainWindow& getMainWindow();

    /**
     * Configuration dialog for 1D data
     */
    class configDialog : public viewerBase::configDialog {
    public:
      /**
       * Construction
       */
      configDialog(viewer1D& parent);
      /**
       * Sort of copy constructor but with a new parent
       */
      configDialog(const configDialog& other,viewerBase& parent);

      /**
       * Destruction
       */
      virtual ~configDialog();

      /**
       * Use the given information block.
       *
       * This method does not copy the information block, it just stores a
       * reference to the given instance.
       *
       * Inherited methods should call this one and transfer all data in the
       * information block to the GUI elements.
       */
      virtual bool useInformation(const viewerBase::information* dataInfo);

      /**
       * Get a read-only reference to the data block
       */
      const information& getInformation() const;
     
      /**
       * Set a copy of the given parameters.
       *
       * This method just calls for the base class one and then indicates
       * the toolkit instance to update its GUI elements.
       */
      virtual bool setParameters(const parameters& par);
      

      /**
       * @name Toolkit dependent functions
       */
      //@{
      /**
       * Constructs the main window.
       *
       * You usually don't need to call this method directly, as 
       * it is called by show() if necessary.
       *
       * This is called just once for the configuration dialog at its
       * construction time.
       * 
       * In inherited classes it is usually employed to build the basic
       * structure of the configuration dialog
       */
      virtual bool build();

      /**
       * Show the configuration dialog
       */
      virtual bool show();

      /**
       * Hides the configuration dialog
       */
      virtual bool hide();

      /**
       * Destroys the widget completely.
       *
       * Before the widget is destroyed, the window is hidden, and the
       * parent viewer is signalized accordingly in order to destroy
       * the configuration dialog.
       */
      virtual bool destroy();
      //@}


      /**
       * Save button pressed.
       *
       * Method called when the user pressed the save button.
       * Usually this method just calls viewer1D::saveButtonPressed();
       */
      virtual bool saveButtonPressed(const std::string& filename);

      /**
       * Save all button pressed.
       *
       * Method called when the user pressed the save all button.
       * Usually this method just calls viewer1D::saveAllButtonPressed();
       */
      virtual bool saveAllButtonPressed(const std::string& filename);

      /**
       * Copy method.
       *
       * Copies everything, but the parent main window
       */
      configDialog& copy(const configDialog& other);

      /**
       * This clone requires the parent window
       */
      virtual configDialog* clone(viewerBase& parent) const;

    protected:
      /**
       * Private class used to contain all internals based on each specific
       * widget toolkit
       */
      class tk;

      /**
       * Get the toolkit with the proper type
       */
      tk* getToolkit();

    private:

      /**
       * Initialize toolkit dependent class
       *
       * This is called at construction time but is implemented by the 
       * toolkit being used.
       */
      virtual void initToolkit();

      /**
       * Destroy toolkit dependent class
       *
       * This is called at destruction time and is implemented by the 
       * toolkit being used.
       */
      virtual void destroyToolkit();

      /**
       * Disable operator=
       */
      configDialog& operator=(const configDialog& other);
    };

    /**
     * Return a read-only reference to the configuration dialog
     */
    const configDialog& getConfigDialog() const;

    /**
     * Return a writable reference to the configuration dialog
     */
    configDialog& getConfigDialog();

  protected:

    // TODO: may be eliminated if we use the other way around: 
    // drawingAreaExposed can call a virtual function of the viewer1D, which
    // creates the image and then calls something of mainWindow which calls
    // something of tk which does the real job
   
    friend class mainWindow; // it has to get silent access to the data_

    /**
     * Pointer to the last object shown
     */
    const container* data_;

    /**
     * The painter
     */
    viewer1DPainter* painter_;

    /**
     * Get data as an image for the window indicated by area
     *
     */
    virtual bool getFormattedData(const irectangle& area,
                                  image& img);

    /**
     * Get data as an image for the window indicated by area
     *
     */
    virtual bool getFormattedData(image& img);


    /**
     * Cast data_ as image
     */
    inline const ivector* getDataAsIntegers() const;

    /**
     * Cast data_ as channel
     */
    inline const dvector* getDataAsDoubles() const;

  };
  
}

#include "ltiViewer1D_template.h"

#endif
