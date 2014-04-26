//
//  Resource.h
//  Space Invaders
//
//  Created by Jed Tiotuico on 4/23/14.
//  Copyright (c) 2014 Jed Tiotuico. All rights reserved.
//

#ifndef Space_Invaders_Resource_h
#define Space_Invaders_Resource_h

#include "ResourcePath.hpp"

class Resource {
public:
	static std::string resourcePath(void) {
		return resourcePathForMac();
	}
};

#endif
