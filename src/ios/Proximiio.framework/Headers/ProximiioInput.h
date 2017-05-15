//
//  ProximiioInput.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team on 09/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import "ProximiioResource.h"
#import "ProximiioLocation.h"
#import "ProximiioBeacon.h"
#import "ProximiioInputType.h"
#import "ProximiioDepartment.h"
#import "ProximiioFloor.h"

@interface ProximiioInput : ProximiioResource

- (int)getMajor;
- (int)getMinor;

- (void)updateAccuracyWithBeacon:(ProximiioBeacon *)beacon;
+ (ProximiioInput *)inputFromJSON:(NSDictionary *)json;
- (ProximiioDepartment *)department;
- (ProximiioFloor *)floor;
- (int)level;


@property ProximiioInputType type;
@property BOOL triggerFloorChange;
@property BOOL triggerVenueChange;
@property (nonatomic, strong) NSUUID *beaconUUID;
@property (nonatomic, strong) NSString *namespaceId;
@property (nonatomic, strong) NSString *instanceId;
@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSDictionary *beaconData;
@property (nonatomic, strong) NSString *departmentId;
@property (nonatomic, strong) NSString *floorId;
@property (nonatomic, strong) NSString *placeId;
@property (nonatomic, strong) ProximiioLocation *location;
@property (nonatomic, strong) ProximiioDepartment *department;

@end
