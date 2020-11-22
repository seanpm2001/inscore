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


#pragma once

//#include "IFaustSignal.h"
#include "IMessageHandlers.h"
#include "IRectShape.h"

namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

class Updater;
class IFaustProcessor;
typedef class libmapping::SMARTP<IFaustProcessor>	SIFaustProcessor;

class FaustProcessorUIElement {
	std::string	fAddress;
	std::string fType;
	std::string fLabel;
	float	fValue, fMin, fMax, fStep;
	int fNode;
	
	public:
				 FaustProcessorUIElement(int node, std::string type, std::string label, std::string address, float init, float min, float max, float step) :
				  fNode(node), fType(type), fLabel(label), fAddress(address), fValue(init), fMin(min), fMax(max), fStep(step) {}
		virtual ~FaustProcessorUIElement() {}

		

};


class TFaustParamUpdate {
	public:
	std::string	address;
	float value;
	TFaustParamUpdate (const std::string& a, float v) : address(a), value(v) {}
};
class TFaustKeysUpdate {
	public:
	enum { kKeyOn, kKeyOff };
	int type, chan, pitch, velocity;
	TFaustKeysUpdate (int type, int chan, int pitch, int vel) : type(type), chan(chan), pitch(pitch), velocity(vel) {}
};

//--------------------------------------------------------------------------
/*!
	\brief a Faust processor (that is also a parallel signal)
*/
class IFaustProcessor : public IRectShape
{
	std::string	fDspCode;
	int fVoices = 0;
	int fNumInputs = 0;
	int fNumOutputs = 0;
	bool fPlaying = false;

	typedef std::map<std::string,float> TParamsValues;
	TParamsValues fParamValues;
		
	std::string address2msg 	(const char * address) const;
	void 		addMsgHandler 	(const std::string& address, const std::string& name, float min, float max);
	float 		getParamValue 	(const std::string& address) const;

    public:
		typedef std::vector<TFaustParamUpdate> TNewValues;
		typedef std::vector<TFaustKeysUpdate>  TKeyValues;
		
		static const std::string kFaustProcessorType;
		static SIFaustProcessor create(const std::string& name, IObject * parent)	{ return new IFaustProcessor(name, parent); }
		virtual void	accept (Updater* u);

		std::string getCode () const 		{ return fDspCode; }
		int			getVoices () const		{ return fVoices; }
		bool		playing() const			{ return fPlaying; }
		void		setIONums (int inputs, int outputs)  { fNumInputs = inputs; fNumOutputs = outputs; }
		void	 	setFaustUI (std::string type, std::string label, std::string address, float init, float min, float max, float step);
		void	 	setParamValue (const std::string& address, float val);

		const TNewValues& 	getChangedValues() const	{ return fNewValues; }
		const TKeyValues& 	getKeyValues() const		{ return fKeyValues; }
		void 				clearChangedValues() 		{ fNewValues.clear(); fKeyValues.clear(); }

	protected:
		std::vector<TFaustParamUpdate> 	fNewValues;
		std::vector<TFaustKeysUpdate> 	fKeyValues;

				 IFaustProcessor( const std::string& name, IObject * parent);
		virtual ~IFaustProcessor();

		void setPlay (bool status) 		{ fPlaying = status; }
		
		/// \brief print the set message
		virtual void	print (IMessage& out) const;

		/// \brief the set method handler
		virtual MsgHandler::msgStatus	set (const IMessage* msg);
		virtual MsgHandler::msgStatus	keyon (const IMessage* msg);
		virtual MsgHandler::msgStatus	keyoff (const IMessage* msg);

		// ------------------------
		// messages handling
		// ------------------------
		class SetFaustParamMsgHandler;
		typedef libmapping::SMARTP<SetFaustParamMsgHandler> SSetFaustParamMsgHandler;
		class SetFaustParamMsgHandler : public MsgHandler {
			public:
				typedef void (IFaustProcessor::*MsgHandlerMethod)(const std::string&, float);
				virtual float check(float val)	{ return std::min ( std::max(val,fMin), fMax ); }
				static SSetFaustParamMsgHandler create(IFaustProcessor* obj, MsgHandlerMethod method, const std::string& address, float min, float max)
						{ return new SetFaustParamMsgHandler(obj, method, address, min, max); }
				virtual msgStatus operator ()(const IMessage* msg) {
					float val;
					if ( msg->size() != 1 || !msg->cast_param(0, val)) return kBadParameters;
					(fObject->*fMethod)( fAddress, val );
					return kProcessed;
				}
			protected:
				IFaustProcessor*	fObject;
				MsgHandlerMethod	fMethod;
				std::string fAddress;
				float fMin, fMax;
				SetFaustParamMsgHandler(IFaustProcessor* obj, MsgHandlerMethod method, const std::string& address, float min, float max) :
					fObject(obj), fMethod(method), fAddress(address), fMin(min), fMax(max) {}
		};

		class GetFaustParamMethodHandler;
		typedef libmapping::SMARTP<GetFaustParamMethodHandler> SGetFaustParamMethodHandler;
		class GetFaustParamMethodHandler : public TGetParamMethodHandler<IFaustProcessor, float (IFaustProcessor::*)()> {
			std::string fAddress;
			typedef float (IFaustProcessor::* GetMethodHandler)(const std::string&) const;
			GetMethodHandler fFaustGetMethod;
			GetFaustParamMethodHandler(IFaustProcessor* o, GetMethodHandler m, std::string address) : TGetParamMethodHandler(o,0), fFaustGetMethod(m), fAddress(address) {}
		public:
			static SGetFaustParamMethodHandler create(IFaustProcessor* obj, GetMethodHandler method, std::string address) {
				return new GetFaustParamMethodHandler(obj, method, address); }
			virtual SIMessage&  print(SIMessage& out) const	{ *out << (fObject->*fFaustGetMethod)(fAddress); return out; }
	};
};

/*! @} */

} // end namespoace
