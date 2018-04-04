#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

//#include "archive.h"
class Archive;
class Serializable
{
	public:
		/* Convert a serializble object into bytes */
		virtual void serialize(Archive& in) const = 0;

		/* Populate the serializable object from bytes */
		virtual void deserialize(Archive& out) = 0;
};


#endif
