//
//  ProximiioGeofenceManager.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team on 09/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProximiioGeofence.h"
#import "ProximiioLocation.h"

@interface ProximiioGeofenceManager : NSObject

-(NSArray *)geofencesForLocation:(ProximiioLocation *)location;
-(void)updateWithLocation:(ProximiioLocation *)location;

+(id)sharedManager;

@property (weak) id delegate;
@property (nonatomic, strong, readonly) NSArray *lastGeofences;

@end

@protocol ProximiioGeofenceManagerDelegate

- (void)didEnterGeofence:(ProximiioGeofence *)geofence;
- (void)didLeaveGeofence:(ProximiioGeofence *)geofence;


@end
