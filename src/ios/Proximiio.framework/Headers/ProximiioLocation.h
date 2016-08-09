//
//  ProximiioLocation.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team 06/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import <CoreLocation/CoreLocation.h>

@class ProximiioLocationSource;

@interface ProximiioLocation : CLLocation

+ (ProximiioLocation *)locationWithLatitude:(float)latitude
                                  longitude:(float)longitude;

+ (ProximiioLocation *)locationWithLatitude:(float)latitude
                                  longitude:(float)longitude
                         horizontalAccuracy:(float)accuracy;

+ (id)locationWithCLLocation:(CLLocation *)location
                      source:(id)source
                       cycle:(int)cycle;
    
+ (id)locationWithCLLocation:(CLLocation *)location
                      source:(id)source;

- (id)initWithCoordinate:(CLLocationCoordinate2D)coordinate
                altitude:(CLLocationDistance)altitude
      horizontalAccuracy:(CLLocationAccuracy)hAccuracy
        verticalAccuracy:(CLLocationAccuracy)vAccuracy
               timestamp:(NSDate *)timestamp
                  source:(ProximiioLocationSource *)source;

- (BOOL)hasSource;
- (BOOL)isWithinInterval:(float)interval;
- (BOOL)hasEqualCoordinatesTo:(ProximiioLocation *)otherLocation;

@property int cycle;
@property double originalAccuracy;
@property double kalmanFix;
@property (nonatomic, strong) ProximiioLocationSource *source;

@end
