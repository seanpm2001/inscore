#include "OSCFilter.h"
#include <iostream>

namespace inscorelistener{

OSCFilter::OSCFilter(OSCFilterNode *root)
	:_root(root)
{

}

OSCFilter::~OSCFilter(){delete _root;}

bool OSCFilter::match(const osc::ReceivedMessage &m, bool verbose) const
{
	OSCFilterContext c(m, verbose);
	return _root->match(c);
}

/********************************************************
 *		    OSCFilterContext			*
 * *****************************************************/

OSCFilterContext::OSCFilterContext(const osc::ReceivedMessage &m, bool verbose)
	:_message(m), _verbose(verbose)
{
	osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
	while(arg!=m.ArgumentsEnd())
	_args.push_back(*(arg++));
}

const osc::ReceivedMessageArgument *OSCFilterContext::arg(size_t i) const
{
	if(_args.size()<=i)
		return 0;
	return &_args.at(i);
}

/********************************************************
 *		    OSCFilterExpr			*
 * *****************************************************/

OSCFilterNode *OSCFilterExpr::filterArg(OSCFilterExpr::Operator op, unsigned int argIndex, float floatValue)
{
	return new OSCFilterExpr(op, argIndex, floatValue, "");
}

OSCFilterNode *OSCFilterExpr::filterArg(OSCFilterExpr::Operator op, unsigned int argIndex, std::string stringValue)
{
	if(op==kEQUAL || op==kNOTEQUAL)
		return new OSCFilterExpr(op, argIndex, 0, stringValue);

	return 0;
}

OSCFilterExpr::OSCFilterExpr(OSCFilterExpr::Operator op, unsigned int argIndex, float floatValue, std::string stringValue)
	:_operator(op), _argIndex(argIndex), _float(floatValue), _string(stringValue)
{
}

bool OSCFilterExpr::eval(OSCFilterContext &filter)
{

	const osc::ReceivedMessageArgument* arg = filter.arg(_argIndex);
	if(!arg)
	return false;

	switch(_operator){
	case OSCFilterExpr::kEQUAL:
	if(_string.empty()){
		if(arg->IsFloat())		//float
		return arg->AsFloatUnchecked()==_float;
		else if(arg->IsInt32()){	//int32
		if(_float!=(float)(int)_float)
			return false;	//Wrong Type
		return arg->AsInt32Unchecked()==_float;
		}else if(arg->IsString())	//Empty string
		return std::string(arg->AsStringUnchecked()).empty();
		else			//Wrong Type
		return false;
	}else if(arg->IsString())
		return !_string.compare(arg->AsStringUnchecked());
	return false;			//Arg is not a string

	case OSCFilterExpr::kNOTEQUAL:
	if(_string.empty()){
		if(arg->IsFloat())		//float
		return arg->AsFloatUnchecked()!=_float;
		else if(arg->IsInt32()){	//int32
		if(_float!=(float)(int)_float)
			return true;	//Wrong Type
		return arg->AsInt32Unchecked()!=_float;
		}else if(arg->IsString())	//Empty string
		return !std::string(arg->AsStringUnchecked()).empty();
		else			//Wrong Type
		return true;
	}else if(arg->IsString())
		return _string.compare(arg->AsStringUnchecked());
	return true;			//Arg is not a string

	case OSCFilterExpr::kGREATER:
	if(arg->IsFloat())		//float
		return arg->AsFloatUnchecked() >_float;
	else if(arg->IsInt32())	//int32
		return arg->AsInt32Unchecked()>_float;
	inequalStringError(filter);	//Wrong Type
	return false;

	case OSCFilterExpr::kGREATEREQUAL:
	if(arg->IsFloat())		//float
		return arg->AsFloatUnchecked()>=_float;
	else if(arg->IsInt32())	//int32
		return arg->AsInt32Unchecked()>=_float;
	inequalStringError(filter);	//Wrong Type
	return false;

	case OSCFilterExpr::kLOWER:
	if(arg->IsFloat())		//float
		return arg->AsFloatUnchecked()<_float;
	else if(arg->IsInt32())
		return arg->AsInt32Unchecked()<_float;
	inequalStringError(filter);	//Wrong Type
	return false;

	case OSCFilterExpr::kLOWEREQUAL:
	if(arg->IsFloat())		//float
		return arg->AsFloatUnchecked()<=_float;
	else if(arg->IsInt32())
		return arg->AsInt32Unchecked()<=_float;
	inequalStringError(filter); 	//Wrong Type
	return false;
	}

	return false;
}

void OSCFilterExpr::inequalStringError(OSCFilterContext &filter)
{
    if(!filter.verbose())
	return;

    std::cerr<<"Inequality only support numbers as arguments, %"<<_argIndex<<" is a string...";
}

/********************************************************
 *		    OSCFilterAddress			*
 * *****************************************************/

bool OSCFilterAddress::eval(OSCFilterContext &filter)
{
	bool r =_addressRegExp.match(filter.address());
	if(!r)	//Si l'addresse du message ne match pas on vérifie que ce n'est pas une regex
		r = inscore::OSCRegexp(filter.address()).match(_address);

	return r;
}

/********************************************************
 *		    OSCFilterLogical			*
 * *****************************************************/
bool OSCFilterLogical::eval(OSCFilterContext &filter)
{
	bool lNodeValue = _lNode->match(filter);
	if(!lNodeValue && _and)
	return false;
	if(lNodeValue && !_and)
	return true;

	return _rNode->match(filter);
}

}//End namespace