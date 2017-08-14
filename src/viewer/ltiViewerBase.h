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
 * \file   ltiViewerBase.h
 *         Contains the abstract base class for all LTI-Lib viewers.
 * \author Pablo Alvarado
 * \date   05.01.2005
 *
 * revisions ..: $Id: ltiViewerBase.h,v 1.16 2012-08-12 02:01:59 alvarado Exp $
 */

#ifndef _LTI_VIEWER_BASE_H_
#define _LTI_VIEWER_BASE_H_

#include "ltiParametersManager.h"
#include "ltiMutex.h"
#include "ltiSemaphore.h"
#include "ltiPoint.h"

namespace lti {
  /**
   * Class viewerBase.
   *
   * Abstract class, parent for all LTI-Lib viewers.

   * It provides some basic interfacing that all viewers must support.
   * The management of the viewer parameters is also provided here
   * (similar to the functor concept).
   *
   * The architecture of the class tree is supposed to support a strong
   * encapsulation of the GUI toolkit specifics, which won't be found here,
   * but in the internal (here still undefined) classes 'tk', which are
   * defined in the files corresponding to the toolkit (like
   * ltiViewerBaseGtk.h)
   */
  class viewerBase : public ioObject,
                     public status,
                     public parametersManager {
  public:
    /**
     * Interaction events recognized by the interface
     */
    enum eInteractionType {
      Idle=0,         /**< No interaction reported */
      KeyPressed,     /**< Key Pressed             */
      KeyReleased,    /**< Key Released            */
      ButtonPressed,  /**< Mouse Button Pressed    */
      ButtonReleased, /**< Mouse Button Released   */
      MouseMoved,     /**< Mouse Pointer Moved     */
      Closed          /**< Window has been closed  */
    };
    
    /**
     * Mouse button symbols
     */
    enum eMouseButton {
      NoButton    =0,  /**< No button pressed       */
      LeftButton  =1,  /**< Left mouse button       */
      MiddleButton=2,  /**< Middle mouse button     */
      RightButton =4,  /**< Right mouse button      */
      WheelUp     =8,  /**< Mouse wheel turned up   */
      WheelDown   =16, /**< Mouse wheel turned down */
      OtherButton =32  /**< Any other mouse button  */
    };

    /**
     * Key Modifiers.
     * This flags can be combined with the bitwise logic operators.
     */
    enum eKeyModifiers {
      NoModifier = 0, /**< No modifier   */
      ShiftKey = 1,   /**< Shift key     */
      CtrlKey = 2,    /**< Control key   */
      AltKey = 4      /**< Alternate key */
    };

    /**
     * Key Names.
     *
     * Names for some special keys
     */
    enum eKeyNames {
      BackspaceKey= 65288, /**< Backspace key */
      TabKey      = 65289, /**< Tab key       */
      EnterKey    = 65293, /**< Enter key     */
      EscKey      = 65307, /**< Esc key       */
      HomeKey     = 65360, /**< Home key      */
      LeftKey     = 65361, /**< Left key      */
      UpKey       = 65362, /**< Up key        */
      RightKey    = 65363, /**< Right key     */
      DownKey     = 65364, /**< Down key      */
      PageUpKey   = 65365, /**< Page up key   */
      PageDownKey = 65366, /**< Page down key */
      EndKey      = 65367, /**< End key       */
      InsertKey   = 65379, /**< Insert key    */
      DeleteKey   = 65535, /**< Delete key    */
      F1Key  = 65470,      /**< F1 key        */
      F2Key  = 65471,      /**< F2 key        */
      F3Key  = 65472,      /**< F3 key        */
      F4Key  = 65473,      /**< F4 key        */
      F5Key  = 65474,      /**< F5 key        */
      F6Key  = 65475,      /**< F6 key        */
      F7Key  = 65476,      /**< F7 key        */
      F8Key  = 65477,      /**< F8 key        */
      F9Key  = 65478,      /**< F9 key        */
      F10Key = 65479,      /**< F10 key       */
      F11Key = 65480,      /**< F11 key       */
      F12Key = 65481       /**< F12 key       */
    };

    /**
     * Interaction structure
     *
     * An interaction of the user with the main window is usually
     * given through the mouse or the keyboard.  The key or mouse
     * button pressed, the modifiers used (e.g. shift, alt or ctrl
     * keys) and the kind of interaction (mouse or keyboard) are
     * packed in objects of this type.
     *
     * @see eInteractionType, eMouseButton, eKeyModifiers, eKeyNames
     */
    struct interaction {
      /**
       * Type of interaction
       *
       * This attribute tells you if it was a keyboard, mouse or windows closed
       * interaction.
       *
       * @see eInteractionType
       */
      eInteractionType action;

      /**
       * Key or mouse button pressed (or released).
       *
       * The library defines several constants to improve readability of your
       * code.  
       *
       * Example:
       * \code
       * lti::viewerBase::interaction action;
       * ...
       * switch (action.key) {
       * case lti::viewerBase::TabKey:
       *   // do something if tab key is pressed
       *   break;
       * case lti::viewerBase::PageUpKey:
       *   // do something if page-up key was pressed
       * case lti::viewerBase::DownKey:
       *   // do something if the down key was pressed
       *   break;
       * case 'A':
       *   // do something if a capital 'A' was pressed
       *   break;
       * ...
       * default:
       *   // and for anything else...
       * }
       * \endcode
       *
       * @see eKeyNames
       */
      int key;

      /**
       * Modifiers used (shift, ctrl or alt).
       *
       * You can check for a modifier with bitwise masking.
       *
       * Example:
       * \code
       * lti::viewerBase::interaction action;
       *
       * ...
       * if ((action.modifiers &
       *      (lti::viewerBase::ShiftKey + lti::viewerBase::CtrlKey)) != 0) {
       *   // do something if the control+shift keys were pressed
       * }
       * \endcode
       *
       * @see eKeyModifiers
       */
      int modifiers;

      /**
       * Default constructor initializes with right-button press action
       */
      interaction();

      /**
       * Constructor that initialize the structure with default values
       */
      interaction(const eInteractionType action,
                  const int k,
                  const int m);

      
      /**
       * Compare with the interaction type, which is an usual operation.
       */
      inline bool operator==(const eInteractionType ia) const {
        return (action == ia);
      }
      
      /**
       * Compare with the interaction type, which is an usual operation.
       */
      inline bool operator!=(const eInteractionType ia) const {
        return (action != ia);
      }

      /**
       * Compare with the interaction type, which is an usual operation.
       */
      inline bool operator==(const interaction& ia) const {
        return ((action    == ia.action)  &&
                (key       == ia.key)     &&
                (modifiers == ia.modifiers));
      }
      
      /**
       * Compare with the interaction type, which is an usual operation.
       */
      inline bool operator!=(const interaction& ia) const {
        return ((action    != ia.action)  ||
                (key       != ia.key)     ||
                (modifiers != ia.modifiers));
      }

    };
    // end of struct interaction

    /**
     * Parameters of the lti::viewerBase class.
     *
     * There are parameters related with the GUI, for example which
     * interaction type starts the config dialog, the position and
     * size of the window, etc.  There are other parameters related
     * with the visualization of the data, which are defined in the
     * functors that do the painting jobs for the viewers
     * (e.g. lti::viewer1DPainter.h, lti::viewer2DPainter.h,
     * lti::viewer3DPainter.h).  This functionality separation permits
     * to use the painters without the GUI interfaces, in case the
     * user wants to provide the same viewer abilities in his own GUI.
     * The functors of the derived viewers will have parameters that
     * inherit from this class and from the painters as well.
     *
     * possible solution: the gui parameters follow, and they DO NOT
     * inherit from functor.  The other parameters can be found in the
     * derived viewer classes so that their parameters can inherit
     * from the current parameters AND from the painter functors' parameters.
     *
     * This way, the viewers can make use of the functors using their
     * own parameters, but a painter's parameters alone makes no sense here.
     */
    class parameters : virtual public parametersManager::parameters {
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

    public:
      //
      // Attributes
      //

      /**
       * Title for the main window
       */
      std::string title;

      /**
       * What must happen in order to activate the configuration dialog.
       *
       * Default value: {ButtonPressed,RightButton,NoModifier}
       */
      interaction actionForConfig;

      /**
       * Position of the window.
       *
       * Default value: (0,0)
       */
      ipoint position;

      /**
       * Size of the window in pixels.  
       *
       * The size corresponds to the display area, which is a little bit
       * smaller (depending on theme, window manager, etc.) than the whole
       * size of the viewer window.
       * 
       * If you give negative values here, the size will be automatically set.
       *
       * Default value: (-1,-1)
       */
      ipoint size;
    };
    // end of class parameters

    /**
     * Information block
     * 
     * This is the parent class for all information blocks used to transmit
     * some data to the configuration dialogs, which are to be displayed in a
     * read-only manner.
     *
     * It has a simplified interface if compared with lti::object
     */
    class information {
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
      virtual information* clone() const = 0;
    }; 
    // end of class information

    /**
     * @name Interaction functions.
     *
     * The interaction activity with the viewer is not considered to
     * alter its state.  So you can have a constant viewer (for
     * example, you give the viewer as const argument to a method,
     * where the viewer already shows something) and make some
     * interaction with it.  You can query for interaction with the
     * viewer without problems.
     */
    //@{
    /**
     * The most complete interaction function reports almost anything.
     *
     * @param action interaction object where all information about
     *        key/button, modifiers and type of interaction is stored.
     * @param position position where the mouse was clicked 
     * @param onlyIfValid if true, this method waits until the mouse
     *        pointer and focus of the window lies on the main viewer window.
     *        If false, only the focus is required to report any interaction
     * @return true if an interaction took place, false if the function returns
     *         due to the destruction of the viewer object (for example, the
     *         user closed the window).
     */
    virtual bool waitInteraction(interaction& action,
                                 ipoint& position,
                                 const bool onlyIfValid = true) const;


    /**
     * Waits until a mouse button is pressed.
     * 
     * @param action interaction object where all information about
     *        key/button, modifiers and type of interaction is stored.
     * @param position position where the mouse was clicked 
     * @param onlyIfValid if true, this method waits until the mouse
     *        pointer and focus of the window lies on the main viewer window.
     *        If false, only the focus is required to report any interaction
     * @return true if an interaction took place, false if the function returns
     *         due to the destruction of the viewer object (for example, the
     *         user closed the window).
     */
    virtual bool waitButtonPressed(interaction& action,
                                   ipoint& position,
                                   const bool onlyIfValid = true) const;

    /**
     * Waits until a mouse button is released.
     * 
     * @param action interaction object where all information about
     *        key/button, modifiers and type of interaction is stored.
     * @param position position where the mouse was clicked 
     * @param onlyIfValid if true, this method waits until the mouse
     *        pointer and focus of the window lies on the main viewer window.
     *        If false, only the focus is required to report any interaction
     * @return true if an interaction took place, false if the function returns
     *         due to the destruction of the viewer object (for example, the
     *         user closed the window).
     */
    virtual bool waitButtonReleased(interaction& action,
                                    ipoint& position,
                                    const bool onlyIfValid = true) const;

    /**
     * Waits until a keyboard key is pressed.
     *
     * \b Note. This get triggered for key down and for key up. In a loop that
     * executes quickly and uses waitKey() to stop for viewing after each
     * iteration you might get the impression that two steps are taken at
     * once. This is because iteration time is faster than the time it takes
     * to lift the finger. Consider using waitKeyPressed instead.
     * 
     * @param action interaction object where all information about
     *        key/button, modifiers and type of interaction is stored.
     * @return true if an interaction took place, false if the function returns
     *         due to the destruction of the viewer object (for example, the
     *         user closed the window).
     */
    virtual bool waitKey(interaction& action);

    /**
     * Waits until a keyboard key is pressed down.
     * 
     * @param action interaction object where all information about
     *        key/button, modifiers and type of interaction is stored.
     * @return true if an interaction took place, false if the function returns
     *         due to the destruction of the viewer object (for example, the
     *         user closed the window).
     */
    virtual bool waitKeyPressed(interaction& action);

    /**
     * Waits until a keyboard any key is pressed down.
     * 
     * @return true if an interaction took place, false if the function returns
     *         due to the destruction of the viewer object (for example, the
     *         user closed the window).
     */
    virtual bool waitKeyPressed();

    /**
     * Waits until the viewer window is closed by the user.
     */
    virtual bool waitClosed();

    /**
     * Returns the information available about the last action
     */
    virtual void getLastAction(interaction& action,
                               ipoint& position) const;
    //@}

    /**
     * Construct a viewerBase
     */
    viewerBase();
    
    /**
     * Copy constructor
     */
    viewerBase(const viewerBase& other);

    /**
     * Virtual destructor
     */
    ~viewerBase();

    /**
     * Name of the class
     */
    virtual const std::string& name() const;

    /**
     * copy all attributes of the viewerBase
     */
    viewerBase& copy(const viewerBase& other);

    /**
     * Hide the current window and all its configuration dialogs if displayed.
     *
     * @return true if window was hidden or false if it was already hidden or
     *         it has not been shown up yet.
     */
    bool hide();

    /**
     * Update parameters
     *
     * Overload necessary to transfer some data to the corresponding windows
     */
    virtual bool updateParameters();

    /**
     * Return current parameter's instance
     */
    const parameters& getParameters() const;

    /**
     * Change the title of the window
     */
    bool setTitle(const std::string& title);

  protected:
    
    /**
     * Return current parameter's instance
     */
    parameters& getRWParameters();

    /**
     * Parent class for all configuration classes.
     *
     * The "parent" of a configuration dialog is its corresponding main window,
     * which is set at construction time.  This way, the configuration has the
     * binding necessary to manage all information exchange.
     *
     * The specific configDialog instance for a viewer is indicated with the
     * method viewerBase::useConfig(). TODO: What to expect from the memory
     * management?
     */
    class configDialog {
      friend class viewerBase;
    public:
      /**
       * Construction
       */
      configDialog(viewerBase& parent);

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
      virtual bool useInformation(const information* dataInfo);

      /**
       * Use the given information block.
       *
       * This method does not copy the information block, it just stores a
       * reference to the given instance, but it overtakes the memory
       * management.
       *
       * The implementation of this method is based on useInformation(), so
       * that you may want to overload just that method.
       */
      virtual bool attachInformation(const information* dataInfo);

      /**
       * Get a read-only reference to the data block
       */
      const information& getInformation() const;

      /**
       * Check if an information block has been assigned or not
       */
      bool validInformation() const;

      /**
       * Set a copy of the given parameters.
       *
       * Here at the base class this method just set-ups the pointer
       * making a copy of the given parameters.
       * 
       * Inherited classes have to set also the GUI elements of the
       * configuration dialog to the corresponding parameters.
       */
      virtual bool setParameters(const parameters& par);

      /**
       * Get read-only reference to the internal parameters
       */
      const parameters& getParameters() const;

      /**
       * Get a writable reference to the internal parameters
       */
      parameters& getParameters();

      /**
       * Get the internal copy of the parameters and eliminate the internal
       * reference.
       */
      parameters* detachParameters();
      
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
       * Copy method.
       *
       * Copies everything, but the parent main window
       */
      configDialog& copy(const configDialog& other);

      /**
       * This clone requires the parent window
       */
      virtual configDialog* clone(viewerBase& parent) const;

    private:
      /**
       * Data block.
       *
       * The memory block is managed by the class and corresponds to the
       * meta information about the data being displayed.
       */
      const information* data_;

      /**
       * Flag of data ownership
       *
       * This flag is true if data_ belongs to this class, or false if
       * it is just a reference to some external data.
       */
      bool ownsData_;

    protected:
      /**
       * Reference to the parent viewer
       */
      viewerBase* parent_;

      /**
       * Parameters of the configuration dialog.  They are a copy of the
       * given parameters, and they have to be removed by this class.
       */
      parameters* params_;

      /**
       * Private class used to contain all internals based on each specific
       * widget toolkit
       */
      class tk;

      /**
       * The toolkit instance created only at construction time through the
       * initToolkit() method and destroyed in the destructor through the
       * destroyToolkit() method.
       */
      mutable tk* tk_;

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
       * Virtual function called when the user clicks the cancel button
       */
      virtual bool onCancelButtonClicked();

      /**
       * Virtual function called when the user clicks the apply button
       */
      virtual bool onApplyButtonClicked();

      /**
       * Virtual function called when the user clicks the ok button
       */
      virtual bool onOkButtonClicked();

      /**
       * Disable operator=
       */
      configDialog& operator=(const configDialog& other);
    };

    /**
     * Parent class of all main windows.
     */
    /* Maybe with the proper interfacing we don't need more main windows:
     * it just need to display given 2D data in a window (plain drawing).
     *
     * The question is: should we take care of partial zooming or we leave
     * that task the toolkit:
     * Since I have had the problem of huge zoomed images, taking care of the
     * zoom is a good idea, but much more work.
     *
     * The other thing is what should be made available for 3D viewers:
     * for this I need to check of 3D libraries (VTK was not good) or just
     * create our own framework:
     *
     * 3D points / lines / triangles / polygons / delauney triangulation
     * faces / objects / scenes / cameras / coordinate systems.
     *
     * Since the focus is on vision, we don't really need the more
     * complex issues.
     *
     * On the other hand, checking for other libraries and making the LTI-Lib
     * structures compatible with them saves lots of work and let those other
     * people do a better dedicated job on the visualization and 3D analysis.
     *
     * Pros: perfect matching with other LTI-Lib functors
     * Contras: a HUGE LOT of work.
     *
     * In other words: since it is not really known what is to be
     * drawn on this window, the interface has to be very general.
     */
    class mainWindow {
      friend class viewerBase;

    public:
      
      /**
       * Create an "empty" window.
       * 
       * The constructor creates just the object, without interacting
       * with the GUI, to save some time (maybe the users still do not
       * want to show any data.  This is the one and only constructor
       * and requires to know who is the parent viewer, in order to
       * make possible the communication with it.
       */
      mainWindow(viewerBase& parent);

      /**
       * Copy constructor.
       *
       * This makes a deep copy except of the parent, which has to be
       * given explicitely.
       */
      mainWindow(const mainWindow& other,viewerBase& parent);

      /**
       * Destructor
       */
      virtual ~mainWindow();
      
      /**
       * Clone 
       *
       * A new mainWindow is created with everything identical to the current
       * one expect for the parent viewer, which has to be given explicitely
       */
      virtual mainWindow* clone(viewerBase& parent) const;

      /**
       * Get information hold by the viewerBase
       */
      const information& getInformation() const;

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
       * This is called just once for a viewer.
       */
      virtual bool build();

      /**
       * Shows the main window.
       *
       * If it is not constructed yet, then the main window is constructed.
       */
      virtual bool show();

      /**
       * Hides the main window
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

      /**
       * Display in the status bar the given text
       */
      virtual bool statusBar(const std::string& txt);

      /**
       * Resize canvas.
       *
       * Update the size of the area used to display data.
       *
       * The "canvas" may receive different names in different toolkits.
       * In the GTK implementation is a gtk-drawing-area, but QT and Windows
       * might use other similar names.
       *
       * The canvas is usually as big as the image being displayed or as the
       * largest possible projection plane in rendering 3D scenes.
       */
      virtual bool setCanvasSize(const ipoint& newSize);
      //@}

      /**
       * Return the by now registered canvas size.
       *
       * This method does not return the real canvas size, but an internal
       * value stored the last time the canvas size was changed. 
       */
      virtual const ipoint& getCanvasSize() const;
      
      /**
       * Compute the new canvas size.
       *
       * Nothing but the computation of the "optimal" size has to be in an
       * overloaded method.  The setCanvasSize() method decides if the change
       * takes place or not.
       */
      virtual ipoint computeCanvasSize() const;

      /**
       * Keyboard/Mouse slot.
       *
       * Virtual function called by the toolkit when there is some
       * sort of interaction with the main window.  This function then
       * calls parentViewer.setKMInteraction() with the appropriate
       * parameters and checks if it is the key/button configuration
       * that starts the configuration dialog.
       */
      virtual bool onKMInteraction(const interaction& action,
                                   const ipoint& position);

      /**
       * Called when the main window was closed.
       *
       * The default implementation just generates the proper
       * event (for waitInteration()) and hides everything.
       */
      virtual bool closeWindow();

      /**
       * Change the parameters being used.
       *
       * This method is implemented in the toolkit dependent part, as
       * it is the toolkit the one that knows how to set all parameters.
       *
       * Note that the given reference can/will be modified by the toolkit,
       * since the user can change the geometry of the window, and that will
       * be reflected in the parameters.
       */
      virtual bool updateParameters(parameters& params);

      /**
       * Get a read-only reference to the parameters
       */
      const viewerBase::parameters& getParameters() const;

    protected:
      
      /**
       * Private class used to contain all internals for a specific
       * widget toolkit
       */
      class tk;

      /**
       * The toolkit instance created only at construction time and
       * destroyed in the destructor.
       */
      mutable tk* tk_;

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
      mainWindow& operator=(const mainWindow& other);
      
      /**
       * Reference to the parent window
       */
      viewerBase* parent_;

      /**
       * Parameters of the viewer
       */
      parameters* params_;

      /**
       * Last hint about the size of the canvas, i.e. the area which
       * is going to be used by the used toolkit to display the information
       */
      ipoint canvasSize_;
    };
    // end of class mainWindow

  private:
    /**
     * Function called by the main window if the user interacts with it.
     *
     * It takes care of the semaphore management to coordinate with the
     * waitInteraction() and similar methods.
     */
    bool setKMInteraction(const interaction& action,
                          const ipoint& position);

  protected:
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
     * Return a read-only reference to the configuration dialog
     */
    const configDialog& getConfigDialog() const;

    /**
     * Return a writable reference to the configuration dialog
     */
    configDialog& getConfigDialog();

    /**
     * Give the configuration dialog to be used.
     *
     * The memory management is responsibility of the user.
     * @see setConfig()
     */
    virtual bool useConfig(configDialog* dlg);

    /**
     * Give the configuration dialog to be used.
     *
     * The memory management is taken from the user.
     * @see useConfig()
     */
    virtual bool attachConfig(configDialog* dlg);

    /**
     * Function called if the configuration dialog is to be shown.
     *
     * The derived classes have to set the data of the configuration dialog
     */
    virtual bool showConfig();

    /**
     * Function called if the user pressed "apply" or "ok" on the configuration
     * dialog, which is normally used to apply all changes in the dialog
     * parameters to the displayed data.
     * 
     *
     * @param keepConfigData if true, the configuration dialog has to
     *                       keep its data, which is desired when "apply" is
     *                       pressed.
     *                       if false, the parameters won't be copied, but
     *                       just transfered to the viewer.
     */
    virtual bool updateFromConfig(const bool keepConfigData);

    /**
     * Function called if for some reason the configuration dialog has to
     * be hidden.
     */
    virtual bool hideConfig();

    /**
     * Ensure that a main window of the proper type has been created.
     * 
     * @return true if a new mainWindow instance was created or false if
     *         it already existed.
     */
    virtual bool ensureMainWindow();

    /**
     * Returns true if the mainWindow has been set.
     */
    bool validMainWindow() const;

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
     * Set mainWindow
     *
     * Exactly the pointed object will be utilized, without copying it, and
     * the memory management is assumed to be done by the user himself.
     *
     * @see attachMainWindow
     *
     * @return true if the new object was simply assigned, or false if a
     *              previous object had to be removed first.
     */
    bool useMainWindow(mainWindow* newWnd);

    /**
     * Set mainWindow
     *
     * Exactly the pointed object will be utilized, without copying it, and the
     * memory management is taken from the user.  
     *
     * @warning The user should never remove the given instance.
     *
     * @see useMainWindow()
     *
     * @return true if the new object was simply assigned, or false if a
     *              previous object had to be removed first.
     */
    bool attachMainWindow(mainWindow* newWnd);

    /**
     * Returns true if the information block has been set or false otherwise
     */
    bool validInformation() const;

    /**
     * Make a copy of the given information block and store it internally.
     *
     */
    bool setInformation(const information& info);

    /**
     * Get a read-only reference to the internal information block.
     *
     * This can return a wrong reference.  You should check validInformation()
     * first.
     */
    const information& getInformation() const;

    /**
     * This virtual method is called just before the configuration dialog
     * is to be displayed, in order to update the information block to the
     * data being processed.
     */
    virtual bool updateInformation();

    /**
     * Get a writable reference to the internal information block
     *
     * This can return a wrong reference.  You should check validInformation()
     * first.
     */
    information& getInformation();


  private:
    /**
     * Pointer to the main window
     */
    mainWindow* mainWnd_;

    /**
     * Flag for ownership of the mainWindow
     */
    bool ownsMainWnd_;

    /**
     * Pointer to the main window
     */
    configDialog* configDlg_;

    /**
     * Flag for ownership of the configDlg_
     */
    bool ownsConfigDlg_;

    /**
     * Pointer to used information block
     */
    information* info_;

    /**
     * Flag for ownership of the information block
     */
    bool ownsInfo_;

    /**
     * Disable operator= until it is implemented
     */
    viewerBase& operator=(const viewerBase& other);

    /**
     * Protect access to flags for interaction
     */
    mutable mutex lock_;

    /**
     * Flag that indicates if the viewer is waiting for some user interaction
     */
    mutable bool waitingInteraction_;

    /**
     * Semaphore that counts the number of threads that are waiting for
     * a user interaction.
     */
    mutable semaphore kmSem_;

    /**
     * Bitwise combination of the used modifiers (see eKeyModifiers)
     */
    int modifiers_;

    /**
     * Key pressed
     */
    int key_;

    /**
     * Mouse button pressed
     */
    eMouseButton button_;

    /**
     * Position where the button/key were pressed
     */
    ipoint pos_;

    /**
     * Last interaction type reported
     */
    eInteractionType action_;

  };

  // two global io functions
  bool read(ioHandler& handler,
            viewerBase::interaction& obj, 
            bool complete=true);
  bool write(ioHandler& handler,
             const viewerBase::interaction& obj, 
             bool complete=true);
}

#endif
