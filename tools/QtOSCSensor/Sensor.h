#ifndef __Sensor__
#define __Sensor__

#include <map>
#include <vector>

class QSensor;
class QSensorReading;

//------------------------------------------------------------------------
class Sensor
{
	private:
		QSensor*		fSensor;
		int				fType;
		QSensorReading*	fReader;
		bool			fSkipDuplicates;
		std::vector<float> fLastRead;

		void			init();
		QSensor*		create(int type) const;
	
		static std::map<int, const char*>	fOSC;
		static std::map<int, const char*>	fNames;
	
	public:
		enum  { kSensorStart=1, kAccelerometer=1, kAmbientLight, kCompass, kGyroscope, kLight, kMagnetometer, kOrientation, kRotation, kTilt, kSensorMax };

				 Sensor(int type);
		virtual ~Sensor();

		const char*		name() const		{ return fNames[fType]; }
		const char*		address() const		{ return fOSC[fType]; }
		QSensor*		sensor()			{ return fSensor; }
		bool			available() const	{ return fSensor->isConnectedToBackend(); }
		bool			active() const		{ return fSensor->isActive(); }
		void			activate(bool state);
		int				count();
		float			value(int i) const	{ return fReader->value(i).value<float>(); }
		void			skipDuplicates(bool state);
};


#endif
