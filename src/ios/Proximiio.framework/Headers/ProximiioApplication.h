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
@property BOOL usesGeofencePositioning;
@property BOOL usesTrilateration;
@property BOOL usesNetworkInterval;
@property int networkInterval;
@property int nativeActivationThreshold;
@property NSString *indoorAtlasApiKey;
@property NSString *indoorAtlasApiSecret;
    

+ (ProximiioApplication *)applicationFromJSON:(NSDictionary *)json;

@end
    
