/* -*- mode: c++ -*- */
/**
 * @file   ShapeList.h
 * @author Sebastien Fourey (GREYC)
 * @date   Aug 2007
 *
 * @brief  Classes ShapeList and Group
 *
 * \@copyright
 * This source code is part of the Board project, a C++ library whose
 * purpose is to allow simple drawings in EPS, FIG or SVG files.
 * Copyright (C) 2007 Sebastien Fourey <http://foureys.users.greyc.fr/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _BOARD_SHAPELIST_H_
#define _BOARD_SHAPELIST_H_

#include "board/Shapes.h"
#include "board/Tools.h"

#if __cplusplus<201100
#define override
#endif

namespace PlaneDraw {

struct Group;

/**
 * The ShapeList structure.
 * @brief A group of shapes
 */
struct ShapeList : public Shape {
  
  enum Direction { Top, Right, Bottom, Left };
  enum Alignment { AlignTop, AlignBottom, AlignCenter, AlignLeft, AlignRight };

  inline ShapeList( int depth = -1 );

  ShapeList( const ShapeList & other );

  ShapeList & operator=( const ShapeList & other );

#if __cplusplus > 201100

  ShapeList( ShapeList && other );

  ShapeList & operator=( ShapeList && other );

#endif


  /**
   * Create a ShapeList by repeating a shape (translation & scaling)
   * @param shape The shape to be repeated.
   * @param times The number of repetitions.
   * @param dx The x shift between two repetitions.
   * @param dy The y shift between two repetitions.
   * @param scale The scaling factor between two repetitions.
   */
  ShapeList( const Shape & shape,
             unsigned int times,
             double dx, double dy,
             double scale );

  /**
   * Create a ShapeList by repeating a shape (translation, scaling & rotation)
   * @param shape The shape to be repeated.
   * @param times The number of repetitions.
   * @param dx The x shift between two repetitions.
   * @param dy The y shift between two repetitions.
   * @param scaleX The x scaling factor between two repetitions.
   * @param scaleY The y scaling factor between two repetitions.
   * @param angle The rotation angle between two repetitions.
   */
  ShapeList( const Shape & shape,
             unsigned int times,
             double dx, double dy,
             double scaleX, double scaleY,
             double angle );

  ~ShapeList();

  /**
   * Returns the generic name of the shape (e.g., Circle, Rectangle, etc.)
   *
   * @return
   */
  const std::string & name() const;
  
  ShapeList & clear();

  ShapeList & rotate( double angle, const Point & center );

  ShapeList rotated( double angle, const Point & center );

  ShapeList & rotate( double angle );

  ShapeList rotated( double angle );

  ShapeList & translate( double dx, double dy );

  ShapeList translated( double dx, double dy );

  ShapeList & scale( double sx, double sy );

  ShapeList & scale( double s );

  ShapeList scaled( double sx, double sy );

  ShapeList scaled( double s );

  /**
   * Scales all the values (positions, dimensions, etc.) associated
   * with the shape.
   *
   * @param s The scaling factor.
   */
  void scaleAll( double s );
  
  void flushPostscript( std::ostream & stream,
                        const TransformEPS & transform ) const;
  
  void flushFIG( std::ostream & stream,
                 const TransformFIG & transform,
                 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
                 const TransformSVG & transform ) const;

  void flushTikZ( std::ostream & stream,
                  const TransformTikZ & transform ) const;

  Rect boundingBox(LineWidthFlag) const;
  
  virtual int minDepth() const;

  virtual int maxDepth() const;

  void shiftDepth( int shift );

  ShapeList * clone() const;
  
  /**
   * Adds a shape to the shape list. If the shape has no given depth
   * or is a compound shape (ShapeList) then it is placed on top of
   * the shapes stack. Otherwise, the shape depth is left unchanged.
   *
   * @param shape
   *
   * @return
   */
  ShapeList & operator<<( const Shape & shape );

  /**
   * Adds a shape to the list of shape, always preserving the shape's depth.
   *
   * @param shape
   *
   * @return
   */
  ShapeList & operator+=( const Shape & shape );
  

  /**
   * Append a shape beside the shapelist.
   *
   * @param shape A shape.
   * @param direction The direction where the shape should be appended.
   * @param alignment The alignement with the current shapelist.
   * @param margin A margin between the shapelist and the shape.
   * @param lineWidthFlag Should the line width be considered when computing bounding boxes.
   * @return The shapelist itself, after the shape has been appended.
   */
  ShapeList & append( const Shape & shape,
                      Direction direction,
                      Alignment alignment,
                      double margin = 0.0,
                      LineWidthFlag lineWidthFlag = UseLineWidth );


  /**
   * Insert a tiling based on a shape by repeating this shape along its
   * bounding box.
   *
   * @param shape A shape to be repeated.
   * @param topLeftCorner Position of the top left corner of the tiling.
   * @param columns Number of columns of the tiling.
   * @param rows Number of rows of the tiling.
   * @param spacing Spacing between rows and columns.
   * @param lineWidthFlag Should the line width be considered when computing bounding boxes.
   * @return A reference to the tiling that has been added, as a Group.
   */
  Group & addTiling( const Shape & shape,
                     Point topLeftCorner,
                     std::size_t columns,
                     std::size_t rows,
                     double spacing = 0.0,
                     LineWidthFlag lineWidthFlag = UseLineWidth );

  /**
   * A a repeated shape (with translation, scaling & rotation)
   * @param shape The shape to be repeated.
   * @param times The number of repetitions.
   * @param dx The x shift between two repetitions.
   * @param dy The y shift between two repetitions.
   * @param scaleX The x scaling factor between two repetitions.
   * @param scaleY The y scaling factor between two repetitions.
   * @param angle The rotation angle between two repetitions.
   */
  void repeat( const Shape & shape,
               unsigned int times,
               double dx, double dy,
               double scaleX = 1.0, double scaleY = 1.0,
               double angle = 0.0 );


  /**
   * Insert the shape at a given depth. If the shape is ShapeList or a Board,
   * then all shapes above it will be shifted.
   *
   * @param shape
   * @param depth
   *
   * @return
   */
  ShapeList & insert( const Shape & shape, int depth );

  /**
   * Duplicates the last inserted shape.
   *
   * @param copies The number of copies.
   */
  ShapeList & dup( std::size_t copies  = 1 );
  
  /**
   * Return the last inserted shape with its actual type, if specified (otherwise, a Shape &).
   *
   * @param position The position. 0 is the last inserted shape, 1 is the one before, etc.
   * @return A reference to the addressed shape.
   */
  template<typename T>
  T & last( const std::size_t position = 0 );

  Shape & last( const std::size_t position = 0 );

  /**
   * Convenience function that simply calls last(0).
   *
   * @param position
   *
   * @return
   */
  Shape & top();

  /**
   * @brief Accepts a visitor object.
   *
   * @param visitor A visitor object.
   */
  virtual void accept( ShapeVisitor & visitor );

  /**
   * @brief Accepts a visitor object.
   *
   * @param visitor A visitor object.
   */
  virtual void accept( const ShapeVisitor & visitor );

private:

  static const std::string _name; /**< The generic name of the shape. */
  
protected:

  void addShape( const Shape & shape, double scaleFactor );

  std::vector<Shape*> _shapes; /**< The vector of shapes. */
  int _nextDepth;              /**< The depth of the next figure to be added. */

  /**
   * Free the memory used by the shapes in the shape vector.
   */
  void free();
};

/**
 * The Group structure.
 * @brief A group of shapes. A group is basically a ShapeList except that
 * when rendered in either an SVG of a FIG file, it is a true compound element.
 */
struct Group : public ShapeList {
  
  Group( int depth = -1 )
    : ShapeList( depth ), _clippingPath( true /* closed path */ ) { }
  
  Group( const Group & other )
    : ShapeList( other ), _clippingPath( other._clippingPath ) { }

  ~Group() { }
  
  /**
   * Returns the generic name of the shape (e.g., Circle, Rectangle, etc.)
   *
   * @return
   */
  const std::string & name() const;

  Group & rotate( double angle, const Point & center );

  Group & rotate( double angle );
  
  Group rotated( double angle, const Point & center );
  
  Group rotated( double angle );
  
  Group & translate( double dx, double dy );
  
  Group translated( double dx, double dy );
  
  Group & scale( double sx, double sy );
  
  Group & scale( double s );
  
  Group scaled( double sx, double sy );
  
  Group scaled( double s );
  
  /**
   * Define a clipping rectangle for the group.
   *
   * @param x
   * @param y
   * @param width
   * @param height
   */
  void setClippingRectangle(  float x, float y,
                              float width, float height );

  /**
   * Define a clipping path for the group.
   *
   * @param points A path.
   */
  void setClippingPath(  const std::vector<Point> & points  );

  /**
   * Define a clipping path for the group.
   *
   * @param points A path.
   */
  void setClippingPath( const Path & path );

  void flushPostscript( std::ostream & stream,
                        const TransformEPS & transform ) const;
  
  void flushFIG( std::ostream & stream,
                 const TransformFIG & transform,
                 std::map<Color,int> & colormap ) const;

  void flushSVG( std::ostream & stream,
                 const TransformSVG & transform ) const;

  void flushTikZ( std::ostream & stream,
                  const TransformTikZ & transform ) const;

  Group & operator=( const Group & other );

  Group * clone() const;

  Rect boundingBox(LineWidthFlag) const;

private:
  static const std::string _name; /**< The generic name of the shape. */
  Path _clippingPath;
  static std::size_t _clippingCount;
};


#include "ShapeList.ih"

} // namespace PlaneDraw


#if __cplusplus<201100
#undef override
#endif


#endif /* _SHAPELIST_H_ */

