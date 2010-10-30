//
//  MyUIDelegate.m
//  ofxCocoa demo
//
//  Created by Memo Akten on 30/10/2010.
//  Copyright 2010 MSA Visuals Ltd. All rights reserved.
//

#include "testApp.h"

#import "MyUIDelegate.h"


@implementation MyUIDelegate


-(void)awakeFromNib {
	NSLog(@"MyUIDelegate::awakeFromNib");
}

-(IBAction) theSliderChanged:(id)sender {
	NSLog(@"MyUIDelegate::theSliderChanged %@", sender);
	testApp *app = (testApp*)ofGetAppPtr();
	app->lineWidth = [sender floatValue];
}

-(IBAction) cubeSliderChanged:(id)sender {
	testApp *app = (testApp*)ofGetAppPtr();

	string key;
	switch([sender tag]) {
		case 0: key = "width"; break;
		case 1: key = "height"; break;
		case 2: key = "depth"; break;
		case 3: key = "XRotSpeed"; break;
		case 4: key = "YRotSpeed"; break;
		case 5: key = "ZRotSpeed"; break;
	}
	
	app->qcBox.setValueForKey([sender objectValue], key);
}





@end
