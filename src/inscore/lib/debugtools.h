/*
  Copyright (c) Grame 2010

  This library is free software; you can redistribute it and modify it under
  the terms of the GNU Library General Public License as published by the
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr
  
*/

#ifndef __debugtools__
#define __debugtools__

#include <iostream>
#include <set>
#include <vector>

namespace inscore
{

template <typename T>	std::ostream& operator << (std::ostream& out, const std::set<T>& s)
{
	typedef typename std::set<T>::const_iterator const_iterator;
	for (const_iterator i = s.begin(); i != s.end(); i++) 
		out << *i << " ";
	return out; 
}

template <typename T>	std::ostream& operator << (std::ostream& out, const std::vector<T>& v)
{
	for (unsigned int i =0; i < v.size(); i++) 
		out << v[i] << " ";
	return out; 
}

} // end namespace

#endif
