/*
 *  CocoaDefines.h
 *  Cocoa Test
 *
 *  Created by Mehmet Akten on 17/08/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

#if (TARGET_OF_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
	#import <UIKit/UIKit.h>
	#define OFViewRect	CGRect
#else
	#import <GLee.h>
	#import <Cocoa/Cocoa.h>
	#define OFViewRect	NSRect
#endif