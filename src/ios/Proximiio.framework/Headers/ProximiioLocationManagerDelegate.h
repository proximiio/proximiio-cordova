//
//  ProximiioLocationManagerDelegate.h
//  Proximiio
//
//  Created by Matej Držík on 02/11/16.
//  Copyright © 2016 proximi.io. All rights reserved.
//

#ifndef ProximiioLocationManagerDelegate_h
#define ProximiioLocationManagerDelegate_h

#import "ProximiioLocation.h"

@protocol ProximiioLocationManagerDelegate

-(void)didUpdateLocation:(ProximiioLocation *)location;

@end

#endif /* ProximiioLocationManagerDelegate_h */
