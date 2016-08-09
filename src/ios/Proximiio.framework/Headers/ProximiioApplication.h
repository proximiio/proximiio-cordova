//
//  ProximiioApplication.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team 06/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProximiioResource.h"

@interface ProximiioApplication : ProximiioResource

@property (nonatomic, strong) NSString *name;

@property BOOL remoteMode;
@property BOOL usesNativeLocation;
@property BOOL usesIndoorAtlas;
@property BOOL usesIBeacons;
@property BOOL usesEddyStones;

+ (ProximiioApplication *)applicationFromJSON:(NSDictionary *)json;

@end
    