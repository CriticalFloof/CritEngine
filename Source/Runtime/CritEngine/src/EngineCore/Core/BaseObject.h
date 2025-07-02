#pragma once

namespace Engine {

	// This is the base object for anything that can be modelled through the engine's various systems.
	// Its responsibility is to be a common class for most engine subsystem targets.
	// As such, it must be lightweight holding only data and methods that all engine objects must absolutely have.

	// Functionality can be appended to this base object through the use of inheritance of an interface implementation.
	// The aim with this approach is to allow for flexibility through composition. For example, if a developer doesn't 
	// wish to have his Object be serializable, then he can simply not inherit from one which implements such a feature.
	
	// Of course common guarantees aren't lost because of this, It's just shifting the guarantee from object types to feature interfaces.
	// Additionally, for convenience the engine will implement an inheritance heirarchy of common feature-sets that developers use in objects.
	// with that guarantee shown in a more explicit manner.

	class BaseObject
	{
		// No universal guarantees yet.
	};

}