//
//  ProximiioBeacon.h
//  ProximiiBeaconLibrary
//
//  Created by Marian Frische on 09.09.14.
//  Copyright (c) 2014 Navture Apps. All rights reserved.
//
//  Class that mirrors the CLBeacon functionality with addtional smoothing
//

#import <CoreLocation/CoreLocation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import "ProximiioBeacon.h"

/*!
 @interface ProximiioBeacon
 @brief ProximiioBeacon interface.
 */
@interface ProximiioIBeacon : ProximiioBeacon

/*!
 @brief UUID of this beacon.
 */
@property (nonatomic, readonly) NSUUID      *uuid;
/*!
 @brief Major value of this beacon.
 */
@property (nonatomic, readonly) int         major;
/*!
 @brief Minor value of this beacon.
 */
@property (nonatomic, readonly) int         minor;
/*!
 @brief Proximty of the user to this beacon.
 */
@property (nonatomic, readonly) CLProximity proximity;
/*!
 @brief Approximate distance to this beacon.
 */
@property (nonatomic, readonly) double      distance;
/*!
 @brief Avarage signal strength of this beacon in decibels.
 */
@property (nonatomic, readonly) int         rssi;

@end