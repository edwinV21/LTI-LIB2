/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiLispStreamHandler.h
 *         Contains lti::lispStreamHandler a lisp-like ASCII ioHandler
 * \author Pablo Alvarado
 * \date   07.12.2000
 * 
 * $Id: ltiLispStreamHandler.h,v 1.4 2009-12-04 04:10:23 alvarado Exp $
 */

#ifndef _LTI_LISP_STREAM_HANDLER_H_
#define _LTI_LISP_STREAM_HANDLER_H_


#include "ltiObject.h"
#include "ltiIoHandler.h"
#include <string>
#include <map>
#include <list>

#include <iostream>

namespace lti {
  /**
   * The lispStreamHandler class offers an interface for the functor parameters
   * and other classes to read() and write() them in a LISP-like format.
   * This is the LTI-lib standard ioHandler.
   *
   * If the standard functions atof() and operator<< for numbers
   * consider the locale settings (this occurs in MS-VC++ or newer
   * glibc and gcc with Linux), then the read() and write() methods
   * will also be localed.  A typical problem with this "feature" is
   * that if you have a file saved with "LANG=C", the decimal
   * point is a point (e.g. 1.532), and when you try to read it in
   * another locale (for example LANG=de_DE.ISO-8859-1), then all numbers with
   * a decimal point won't be read correctly, because the method will be
   * expecting a decimal comma (e.g. 1,532).
   *
   * See also the read and write methods for the basic types for more
   * information
   *
   * \warning The given streams must be "ASCII" streams.  If they are
   * created with the binary flag, some weird effects can be expected.
   *
   * Example:
   * \code
   * // the lisp stream formatting object
   * lti::lispStreamHandler lsh;
   *
   * // Write example:
   *
   * // open a stream in text mode
   * std::ofstream out("testfile.bin");
   * 
   * // tell the lisp stream handler to use the given stream
   * lsh.use(out);
   *
   * lti::write(lsh,"anInteger",5);
   * lti::write(lsh,"aString","hello world");
   *
   * out.close();
   * 
   * // Read example
   *
   * // Open a stream in lisp mode
   * std::ifstream in("testfile.bin");
   *
   * lsh.use(in);
   * int i;
   * lti::read(lsh,"anInteger",i);
   *
   * std::string str;
   * lti::read(lsh,"aString",str);
   *
   * in.close();
   * \endcode
   *
   * @ingroup gStorable
   */
  class lispStreamHandler : public ioHandler {
  public:
    /**
     * default constructor
     */
    lispStreamHandler();

    /**
     * default constructor
     *
     * \warning The given stream must be an "ASCII" stream.  If it is
     * created with the binary flag, some weird effects can be expected.
     */
    lispStreamHandler(std::ostream& aStream);

    /**
     * default constructor
     *
     * \warning The given stream must be an "ASCII" stream.  If it is
     * created with the binary flag, some weird effects can be expected.
     */
    lispStreamHandler(std::istream& aStream);


    /**
     * default constructor.
     * This constructor uses the given string as input, as if read from a
     * stread.
     *
     * \warning Note that the given string is NOT a filename, but a
     * string containing the data "stream".  This is useful especially
     * for the JNI (java native interface).  You can of course also use
     * the std::ostringstream and std::istringstream with the other
     * constructors or with the method use().
     */
    lispStreamHandler(const char *aStream);

    /**
     * copy constructor
     */
    lispStreamHandler(const lispStreamHandler& other);

    /**
     * destructor
     */
    virtual ~lispStreamHandler();

    /**
     * indicate the output stream to be used
     *
     * Calling this method you will reinitialize the state of the
     * parser (see also clear()).
     */
    void use(std::ostream& aStream);

    /**
     * indicate the input stream to be used
     *
     * Calling this method you will reinitialize the state of the
     * parser (see also clear()).
     */
    void use(std::istream& aStream);

    /**
     * This method resets all internal state variables (for input and
     * output streams as well) in a way that the next operations
     * behave as if this handler were used for the first time.  This
     * allows the use of the same handler instance after reopening a
     * stream.  Alternativelly, you can just call the
     * use(std::istream&) or use(std::ostream&) members to clear the
     * input or output state variables only.  If you use this method
     * "within" a read/write process, the behaviour will be
     * unpredictable.
     */
    void clear();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    lispStreamHandler& copy(const lispStreamHandler& other);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    lispStreamHandler& operator=(const lispStreamHandler& other);

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual lispStreamHandler* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual lispStreamHandler* newInstance() const;

    /**
     * the begin token or tokens: here a "(".
     *
     */
    virtual bool writeBegin();

    /**
     * the end token or tokens: here a ")".
     */
    virtual bool writeEnd();

    /**
     * the begin token or tokens: here a "("
     */
    virtual bool readBegin();

    /**
     * the end token or tokens: here a ")"
     */
    virtual bool readEnd();

    /** @name write members
     */
    //@{
    /**
     * write a std::string.
     * This method will write just the string if it does not contain spaces.
     * Otherwise the string will be enclosed by quotes.
     */
    virtual bool write(const std::string& data);

    /**
     * write a character string.
     * This method will write just the string if it does not contain spaces.
     * Otherwise the string will be enclosed by quotes.
     */
    virtual bool write(const char* data);

    /**
     * write a double value.
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool write(const double data);

    /**
     * write a float value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool write(const float data);

    /**
     * write an integer value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool write(const int data);

    /**
     * write an unsigned integer value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool write(const unsigned int data);

    /**
     * write a char
     *
     * The char is writen as numerical code if there is no standard ASCII
     * representation.  If there is an ASCII representation (i.e. its value is
     * greater or equal 32 (' ') and smaller or equal 127 ('~'), then the
     * ASCII character will be writen enclosed in single quotes.
     */
    virtual bool write(const char data);

    /**
     * write an 8-bit signed value
     */
    virtual bool write(const byte data);

    /**
     * write a unsigned 8-bit value
     */
    virtual bool write(const ubyte data);

    /**
     * write a boolean
     */
    virtual bool write(const bool data);

    /**
     * write a long
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool write(const long data);

    /**
     * write an unsigned long
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool write(const unsigned long data);

    /**
     * write a short
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool write(const short data);

    /**
     * write an unsigned short
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool write(const unsigned short data);
    //@}

    /** @name read members
     */
    //@{
    /**
     * read a std::string
     *
     * The string should be quoted (i.e. between '"' and '"'), expect
     * if its a single word.  The two-char sequences '\"' and '\\' will
     * be replaced by '"' and '\' respectivelly.
     */
    virtual bool read(std::string& data);

    /**
     * read a double value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(double& data);

    /**
     * read a float value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(float& data);

    /**
     * read an integer value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(int& data);

    /**
     * read an unsigned int value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(unsigned int& data);

    /**
     * Read a char value.
     *
     * The char can be given in two ways:
     * - a numerical representation in decimal code (e.g. 65 represents 'A').
     * - a character representation enclosed in sigle quotes (e.g. 'A')
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(char& data);

    /**
     * read a signed byte value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(byte& data);


    /**
     * read an unsigned byte value
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(ubyte& data);

    /**
     * read a boolan
     */
    virtual bool read(bool& data);

    /**
     * read a long
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(long& data);

    /**
     * read an unsigned long
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(unsigned long& data);

    /**
     * read a short
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(short& data);

    /**
     * read an unsigned short
     *
     * \warning Be aware of the locale you are using.  The number must be
     * read with the same locale used when it was written
     */
    virtual bool read(unsigned short& data);
    //@}

    /**
     * write a std::string
     */
    virtual bool writeSymbol(const std::string& data);

    /**
     * read a symbol in the given std::string
     */
    virtual bool readSymbol(std::string& data);

    /**
     * try to read the given symbol from the handler.
     * If present, returns true and the token is removed from the
     * handler, if not present returns false and leaves the stream as
     * is...
     * @param data the symbol to be readed
     */
    virtual bool trySymbol(const std::string& data);

    /**
     * write comment writes the input data without any preprocessing,
     * just ensuring that the comment format is given
     */
    virtual bool writeComment(const std::string& data);

    /**
     * write comment writes the input data without any preprocessing,
     * just ensuring that the comment format is given
     */
    virtual bool writeComment(const char* data);

    /**
     * try to read the begin token from the handler.
     * If present, returns true and the token is removed from the
     * handler, if not present returns false and leaves the handle as
     * it was before calling this member...
     * This is useful in trees or other complicated data structures.
     */
    virtual bool tryBegin();

    /**
     * try to read the end token from the handler.
     * If present, returns true and the token is removed from the
     * handler, if not present returns false and leaves the handle as
     * it was before calling this member...
     * This is usually used when reading lists of data, where the number of
     * elements is unknown.
     */
    virtual bool tryEnd();

    /**
     * write spaces (default value 1).
     * A space-token is a char with value 32.
     */
    virtual bool writeSpaces(const int s=1);

    /**
     * write end of line
     */
    virtual bool writeEOL();

    /**
     * write key/value separator.
     * In this case the key/value separator is a space
     */
    virtual bool writeKeyValueSeparator();

    /**
     * write key/value separator.
     * In this case the data separator is a space
     */
    virtual bool writeDataSeparator();

    /**
     * write key/value separator
     * A space is expected
     */
    virtual bool readKeyValueSeparator();

    /**
     * write key/value separator
     * A space is expected
     */
    virtual bool readDataSeparator();

    /**
     * if the input stream is at the end of file return true, otherwise false
     * if the stream hasn't been set yet, this function also returns true.
     */
    virtual bool eof();

    /**
     * restore all the information in the handler taken in the actual
     * level.  If "complete" is true, the begin-token is also restored
     */
    virtual bool restoreLevel();

    /**
     * Overload of context status information.
     *
     * This function should help the users to find errors in their files.
     * It just inserts some contextual information into the status string
     * to help localizing wrong data.
     *
     * It is useful for streams that can be edited by hand, because the
     * users will make errors!
     */
    virtual void appendContextStatus() const;


  protected:

    /**
     * pointer to the input stream
     */
    std::istream* inStream;

    /**
     * the input stream will be cached into this string
     */
    std::string inString;

    /**
     * actual reading position in the input string
     */
    std::string::size_type inStringPos;

    /**
     * pointer to the output stream
     */
    std::ostream* outStream;

    /**
     * flag to control spaces supression (for example, there is no need for
     * spaces between parenthesis)
     */
    bool supressSpaces;

    /**
     * flag to indicate if an EOL is needed
     */
    bool tryEOL;

    /**
     * opening char.  Usually "("
     */
    static const char openChar;

    /**
     * closing char.  Usually ")"
     */
    static const char closeChar;

    /**
     * separator char.  Usually a space
     */
    static const char separator;

    /**
     * comment char.  Usually ';'
     */
    static const char commentChar;

    /**
     * string char.  Usually '\"'
     */
    static const char stringChar;

    /**
     * quote char.  Usually "'"
     */
    static const char quoteChar;

    /**
     * type for the cache of each level, where the value for each symbol
     * is stored
     */
    typedef std::map<std::string,std::string> cacheType;

    /**
     * type for each element of the stack
     */
    struct stackElement {
      /**
       * default constructor
       */
      stackElement();
      /**
       * contains the symbol/value table
       */
      cacheType cache;

      /**
       * specify if the input stream contains no more data for this
       * level
       */
      bool complete;

      /**
       * level for the cache
       */
      int level;
    };

    /**
     * the data stack.
     * All readed symbols and their values are stored here temporarly
     * to allow different sorting of the symbols in the stream
     */
    std::list<stackElement> stack;

    /**
     * Types of tokens
     */
    enum eTokenId {
      BeginToken, /**< Denotes begin of a level */
      EndToken,   /**< Denotes end of a level */
      SymbolToken,/**< Some atomic token */
      StringToken,/**< A string token */
      ErrorToken  /**< Unrecognized token */
    };

    /**
     * read next token from the input string or from the input stream
     * @see getNextTokenFromString.
     */
    eTokenId getNextToken(std::string& token,const bool justTry = false);

    /**
     * read next token from the given string
     * this member get the next token into the given string
     *
     * Following tokens are recognized:
     *
     * beginToken := '('
     *
     * endToken   := ')'
     *
     * symbolToken := {alpha | digit} | float
     *
     * stringToken := quote {alpha | digit | other | escape quote} quote
     *
     * ErrorToken
     *
     * where
     *
     * alpha = "A" | "B" | ... "Z" | "a" | "b" | ... "z"
     *
     * digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" |
     *
     * other = "!" | "," | "$" | "%" | "&" | "/" | "[" | "]" | "{" | "}" |
     *         "?" | "'" | "#" | "+" | "*" | "-" | "." | "," | ":" | ";"
     *
     * quote    = '"'
     *
     * escape   = '\'
     *
     * float    = ['+' | '-'] {digit} '.' {digit} [exponent]
     *
     * exponent = eid ['+' | '-'] digit {digit}
     *
     * eid      = 'E' | 'e'
     *
     * comments will be ignored!  They begin with a ';'
     * The ErrorToken is not a real token.  This is the way to signalized that
     * no other token could be readed!
     * If justTry = true the source string will remain untouched
     */
    eTokenId getNextTokenFromString(std::string& src,
                                    std::string::size_type& srcPos,
                                    std::string& token,
                                    const bool justTry = false);


    /**
     * get the next "garbageThreshold" characters from the given
     * stream, and ensures that the last readed character is a delimiter
     */
    void getNextLine(std::string& newline);

    /**
     * this filter eliminates all comments at the beginning of inString
     */
    bool commentFilter();

    /**
     * returns true if the given char can indicate the end of a token
     */
    inline bool isTokenDelimiter(const char& c);

    /**
     * complete actual level reads the data from the input string and
     * input stream until the actual level has been readed.
     */
    bool completeLevel(std::string& restOfLevel);

  private:
    /**
     * look-up-table to accellerate the check for a delimiter
     */
    static const bool* delimiters;

    /**
     * initialize LUT for delimiters
     */
    static void initializeDelimiters();

    /**
     * the size of the garbage size allowed before the data is really deleted!
     */
    static const int garbageThreshold;

    /**
     * a buffer of the garbageThreshold size used to read lines
     */
    char* buffer;
  };
}

#endif


