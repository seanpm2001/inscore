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

#include "VSceneView.h"

#include "IScene.h"
#include "IColor.h"
#include "INScore.h"

#ifdef EMCC
#include "wasmglue.h"
#endif

using namespace std;

namespace inscore
{

//------------------------------------------------------------------------------------------------------------------------
VSceneView::VSceneView()
{
	#ifdef EMCC
	fDocSize = docGetSize();
	#endif

}

//------------------------------------------------------------------------------------------------------------------------
VSceneView::~VSceneView()
{
}

//------------------------------------------------------------------------------------------------------------------------
TFloatRect VSceneView::referenceRect(VObjectContext item) const
{
	return TFloatRect( TFloatPoint(0,fDocSize.width()), TFloatPoint(0,fDocSize.height()));
}

//------------------------------------------------------------------------------------------------------------------------
void VSceneView::updateView( IScene * scene )
{
    if (scene->getDeleted()) return;
}

} // end namespoace