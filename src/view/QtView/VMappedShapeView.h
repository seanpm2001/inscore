/*

  INScore Project

  Copyright (C) 2009,2010  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 11 cours de Verdun Gensoul, 69002 Lyon - France
  research@grame.fr

*/


#ifndef __VMappedShapeView__
#define __VMappedShapeView__

#include "VShapeView.h"

namespace inscore
{

/*!
\addtogroup ITLView
@{
*/

//--------------------------------------------------------------------------
/**
*	\brief a graphic view of a IShapeMap.
*/
class VMappedShapeView: public VShapeView
{
	public :
		using VShapeView::updateLocalMapping;
		
		virtual ~VMappedShapeView() {}
		
		virtual void updateLocalMapping (IShapeMap* shapeMap)	{ updateGraphic2GraphicMapping(shapeMap); }
		static void updateGraphic2GraphicMapping (IShapeMap* object);

	protected:	
		VMappedShapeView(QGraphicsScene * scene , QAbstractGraphicsShapeItem * item): VShapeView(scene,item) {}

};

/*!@} */

} // end namespoace

#endif
