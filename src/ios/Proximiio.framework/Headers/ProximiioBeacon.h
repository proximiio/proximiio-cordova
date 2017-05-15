//
//  ProximiioBeacon.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team on 17/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProximiioInputType.h"
#import "ProximiioLocationSource.h"
#import "ProximiioDepartment.h"

@interface ProximiioBeacon : ProximiioLocationSource;

@property ProximiioInputType type;

@end
