//
//  ProximiioEddystoneBeacon.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team on 17/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProximiioBeacon.h"

@interface ProximiioEddystoneBeacon : ProximiioBeacon

@property (nonatomic, readonly) NSData *IDData;
@property (nonatomic, readonly) NSString *Namespace;
@property (nonatomic, readonly) NSString *InstanceID;
@property (nonatomic, readonly) NSNumber *RSSI;
@property (nonatomic, readonly) NSNumber *txPower;
@property (nonatomic, readonly) NSNumber *accuracy;

@property (nonatomic, readonly) NSNumber *batteryVoltage;
@property (nonatomic, readonly) NSNumber *temperature;
@property (nonatomic, readonly) NSNumber *uptime;
@property (nonatomic, readonly) NSNumber *PDU;

@property (nonatomic, readonly) NSString *URL;

- (BOOL)hasAllValuesSet;

@end
