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
 * \file   ltiViewer2D.h
 *         Contains the class lti::viewer2D used to visualized images and 
 *         channels.
 * \author Pablo Alvarado
 * \date   05.01.2005
 *
 * revisions ..: $Id: ltiViewer2D.h,v 1.8 2012-07-04 13:17:10 alvarado Exp $
 */

#ifndef _LTI_VIEWER_2_D_H_
#define _LTI_VIEWER_2_D_H_

#include "ltiRGBAPixel.h"
#include "ltiViewerBase.h"
#include "ltiPoint.h"
#include "ltiRGBPixel.h"
#include "ltiMatrix.h"
#include "ltiViewer2DPainter.h"


namespace lti {

  /**
   * Class viewer2D.
   *
   * Viewer class to display images, channels and masks.
   *
   * It handles the following general matrix types and their corresponding
   * derived classes:
   *
   * - lti::matrix<lti::rgbaPixel>: for example, color images.
   * - lti::matrix<float>: for example, lti::channel.
   * - lti::matrix<int32>: for example, labeled masks.
   * - lti::matrix<ubyte>: for example, lti::channel8.
   *
   * As default behaviour you can get the configuration window pressing
   * the right button of the mouse, but you can change this behaviour setting
   * the parameters accordingly.
   *
   * If you like the way the viewer displays your data, and you would like
   * the images, then you may want to take a look at lti::viewer2DPainter, the
   * class responsible for painting the images you see.
   *
   * \section Example
   * 
   * A simple example of use follows:
   *
   * \code
   * lti::image img;
   * lti::ioImage loader;
   *
   * loader.load("YourFavoriteImage.png",img); // load an image
   *
   * lti::viewer2D viewer("What a view");
   * viewer.show(img);                         // show your favorite image
   *
   * lti::viewer2D::interaction action;
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
  class viewer2D : public viewerBase {
  public:

    /**
     * There are parameters related with the GUI, for example
     * which interaction configuration starts the config dialog, the
     * position of the window, etc.  There are other parameters that
     * configure options about the image painting, which depend on the
     * lower classes.  The former are inherited from viewerBase::parameters,
     * the latter from viewer2DPainter::parameters.
     */
    class parameters : virtual public parametersManager::parameters,
                       public viewerBase::parameters,
                       public viewer2DPainter::parameters {
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

    /**
     * Types of data that can be displayed with the viewer2D
     */
    enum eDataType {
      Unknown,  /**< Unknown data type.  This is maybe an error */
      Image,    /**< Color image type       */
      Channel,  /**< Floating point channel */
      Channel8, /**< 8-bit channel          */
      Channel32 /**< 32-bit channel         */
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
       * Initialize all attributes from the given channel
       */
      void compute(const matrix<rgbaPixel>& mat);

      /**
       * Initialize all attributes from the given channel
       */
      void compute(const matrix<ubyte>& mat);

      /**
       * Initialize all attributes from the given channel
       */
      void compute(const matrix<float>& mat);

      /**
       * Initialize all attributes from the given channel
       */
      void compute(const matrix<int32>& mat);

      /**
       * Data type being displayed.
       *
       * It indicates if it is a color image a channel8, a channel or a label
       * mask.
       */
      eDataType type;

      /**
       * Size of the displayed image
       */
      ipoint size;

      /**
       * Minimum intensity value
       */
      float minI;

      /**
       * Maximum intensity value
       */
      float maxI;

      /**
       * Intensity average
       */
      float averageI;

      /**
       * Standard deviation of intensity
       */
      float stdDeviation;

      /**
       * Minimum RGB components
       */
      rgbaPixel minRGB;
      
      /**
       * Maximum RGB components
       */
      rgbaPixel maxRGB;

      /**
       * RGB average
       */
      frgbPixel averageRGB;

      /**
       * RGB covariance matrix
       */
      fmatrix covarianceRGB;

    protected:
      template <typename T>
      void computeScalars(const matrix<T>& mat);
      
    }; 
    // end of class information

    // ----------------------------------------------------------------------
    // viewer2D
    // ----------------------------------------------------------------------

    /**
     * Constructor with optional parameters
     * @param title Title of the window, usually shown at the top margin.
     * @param mapping Type of intensity value mapping.  Default: the original 
     *                data is shown as is.
     * @see lti::viewer2DPainter::eMappingType
     */
    viewer2D(const std::string& title="Unknown",
             const viewer2DPainter::eMappingType mapping =
               viewer2DPainter::Original);

    /**
     * Copy constructor.
     *
     * Create a new viewer, with its own window and configuration dialog, with
     * the same content as the other viewer.  If that viewer is still inactive,
     * this viewer will also be.
     */
    viewer2D(const viewer2D& other);

    /**
     * Constructor. Sets the given \p params
     */
    viewer2D(const parameters& params);

    /**
     * Destructor
     */
    ~viewer2D();

    /**
     * Returns a copy of this object
     */
    virtual viewer2D* clone() const;

    /**
     * Returns a copy of this object
     */
    virtual viewer2D* newInstance() const;

    /**
     * Returns a copy of this object
     */
    viewer2D& copy(const viewer2D& other);

    /**
     * Return a read-only copy of the parameters
     */
    const parameters& getParameters() const;

    /**
     * @name Show images or matrices
     */
    //@{

    /**
     * Show a two dimensional color image.
     * @param img the color image to be displayed.
     * @return true if successful, false otherwise
     */
    bool show(const matrix<rgbaPixel>& img);

    /**
     * Show a two dimensional channel with float values
     * @param chnl the channel to be displayed.
     * @return true if successful, false otherwise
     */
    bool show(const matrix<float>& chnl);

    /**
     * Show a two dimensional image mask with integer values
     * @param mask the mask to be displayed.
     * @return true if successful, false otherwise
     */
    bool show(const matrix<int32>& mask);

    /**
     * Show a two dimensional channel with 8 bit pixels.
     * @param chnl8 the fixed point channel to be displayed.
     * @return true if successful, false otherwise
     */
    bool show(const matrix<ubyte>& chnl8);

    //@}

    /**
     * The user can specify a viewer2DPainter object.
     *
     * Usually you will only use this method if you want to completely change
     * the way 2D data is displayed, beyond the configuration posibilities
     * through parameters, which is only possible if you create your own
     * lti::viewer2DPainter derived class.
     *
     * A clone of the given painter will be done.
     *
     * If you derive a class of lti::viewer2D and want to always set your own
     * painter, then you should only overload the method newDefaultPainter()
     */
    virtual bool setPainter(const viewer2DPainter& painter);

    /**
     * Save button pressed.
     *
     * Method called when the user pressed the save button.
     * Usually this method just call the viewer2D::saveButtonPressed();
     *
     * Note that if you call this method you can force from the code that the
     * displayed image is saved at the location specified in the parameters.
     */
    virtual bool saveButtonPressed(const std::string& filename);

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
    virtual viewer2DPainter* newDefaultPainter() const;

    /**
     * Get a writable reference to the internal information block
     *
     * This can return a wrong reference.  You should check validInformation()
     * first.
     */
    information& getInformation();


    /**
     * Ensure that the current information block has the given type and
     * the correct data size.  This saves time getting the information and
     * allows the viewer more speed if a sort of video-out use is planned.
     *
     * Returns false if it already had the given type or true if it was
     * changed.  This is important to know in order to setup the configuration
     * dialog accordingly.
     */
    bool ensureInformationType(const eDataType& type,
                               const ipoint& matrixSize);
    
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
     * Main window of the 2D viewer
     *
     * The only task for this class is to show a 2D color lti::image in the
     * window taking care of the scrollbars.  The corresponding painter
     * (lti::viewer2DPainter) is the one in charge of painting anything into
     * the lti::image.
     */
    class mainWindow : public viewerBase::mainWindow {
    public:
      /**
       * Main window constructor
       */
      mainWindow(viewer2D& parent);

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
       * Compute the new canvas size.
       *
       * Nothing but the computation has to be done here.  If it is applied or
       * not will be a decision of the setCanvasSize() method.
       *
       * The viewer2D computes the size as the product of zoom time the image
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
     * Configuration dialog for 2D data
     */
    class configDialog : public viewerBase::configDialog {
    public:
      /**
       * Construction
       */
      configDialog(viewer2D& parent);
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
       * Usually this method just call the viewer2D::saveButtonPressed();
       */
      virtual bool saveButtonPressed(const std::string& filename);

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
    // drawingAreaExposed can call a virtual function of the viewer2D, which
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
    viewer2DPainter* painter_;

    /**
     * Get data as an image for the window indicated by area
     *
     *
     */
    virtual bool getFormattedData(const irectangle& area,
                                  image& img);


    /**
     * Cast data_ as image
     */
    inline const matrix<rgbaPixel>* getDataAsImage() const;

    /**
     * Cast data_ as channel
     */
    inline const matrix<float>* getDataAsChannel() const;

    /**
     * Cast data_ as channel8
     */
    inline const matrix<ubyte>* getDataAsChannel8() const;

    /**
     * Cast data_ as channel32
     */
    inline const matrix<int32>* getDataAsChannel32() const;

  };
  
}

#endif
