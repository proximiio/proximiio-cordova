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

+ (ProximiioLocation * _Nonnull)locationWithLatitude:(float)latitude
                                  longitude:(float)longitude;

+ (ProximiioLocation * _Nonnull)locationWithLatitude:(float)latitude
                                  longitude:(float)longitude
                         horizontalAccuracy:(float)accuracy;

+ (id _Nonnull)locationWithCLLocation:(CLLocation *_Nonnull)location
                      source:(id _Nullable)source
                       cycle:(int)cycle;

+ (id _Nonnull)locationWithCLLocation:(CLLocation *_Nonnull)location
                      source:(id _Nullable)source;

- (id _Nonnull)initWithCoordinate:(CLLocationCoordinate2D)coordinate
                altitude:(CLLocationDistance)altitude
      horizontalAccuracy:(CLLocationAccuracy)hAccuracy
        verticalAccuracy:(CLLocationAccuracy)vAccuracy
               timestamp:(NSDate *_Nonnull)timestamp
                  source:(ProximiioLocationSource *_Nonnull)source;

- (BOOL)hasSource;
- (BOOL)isWithinInterval:(float)interval;
- (BOOL)isInsidePolygon:(NSArray *_Nonnull)polygon;
- (BOOL)hasEqualCoordinatesTo:(ProximiioLocation *_Nonnull)otherLocation;

-(double) bearingToLocation:(CLLocation *_Nonnull) destinationLocation;

@property int cycle;
@property double originalAccuracy;
@property double kalmanFix;
@property (nonatomic, strong) ProximiioLocationSource *_Nonnull source;

// CLLocation+GreatCircle extensions

/*!
 *  @brief
 *  Returns a location representing the point of intersection of two paths, each specified by a location and bearing.
 *
 *  @param location1 The first location.
 *  @param bearing1 The first bearing.
 *  @param location2 The second location.
 *  @param bearing2 The second bearing.
 *
 *  @return A location representing the point of intersection of two paths, if there is an intersection; otherwise, nil.
 */
+ (nullable CLLocation *)intersectionOfLocation:(nonnull CLLocation *)location1
                                     andBearing:(CLLocationDirection)bearing1
                                   withLocation:(nonnull CLLocation *)location2
                                     andBearing:(CLLocationDirection)bearing2;

/*!
 *  @brief
 *  Compares this location to the other location for equality.
 *
 *  @param otherLocation The other location to compare to this location.
 *
 *  @return YES if this location and the other location are equal; otherwise, NO.
 */
- (BOOL)isEqualToOtherLocation:(nonnull CLLocation *)otherLocation;

/*!
 *  @brief
 *  Returns the distance (in meters) between this location and the other location.
 *
 *  @param otherLocation The other location.
 *
 *  @return The distance (in meters) between this location and the other location.
 */
- (CLLocationDistance)distanceToOtherLocation:(nonnull CLLocation *)otherLocation;

/*!
 *  @brief
 *  Returns the initial bearing (in degrees) between this location and the other location.
 *
 *  @param otherLocation The other location.
 *
 *  @return The initial bearing (in degrees) between this location and the other location.
 */
- (CLLocationDirection)initialBearingToOtherLocation:(nonnull CLLocation *)otherLocation;

/*!
 *  @brief
 *  Returns the final bearing (in degrees) between this location and the other location.
 *
 *  @discussion
 *  The final bearing will differ from the initial bearing by varying degrees according to distance and latitude.
 *
 *  @param otherLocation The other location.
 *
 *  @return The final bearing (in degrees) between this location and the other location.
 */
- (CLLocationDirection)finalBearingToOtherLocation:(nonnull CLLocation *)otherLocation;

/*!
 *  @brief
 *  Returns a location representing the midpoint between this location and the other location.
 *
 *  @param otherLocation The other location.
 *
 *  @return A location representing the midpoint between this location and the other location.
 */
- (nonnull CLLocation *)midpointToOtherLocation:(nonnull CLLocation *)otherLocation;

/*!
 *  @brief
 *  Returns a location representing the point that lies at the specified bearing and distance from this location.
 *
 *  @param bearing The bearing, in degrees.
 *  @param distance The distance, in meters.
 *
 *  @return A location representing the point that lies at the specified bearing and distance from this location.
 */
- (nonnull CLLocation *)locationWithBearing:(CLLocationDirection)bearing
                                   distance:(CLLocationDistance)distance;

/*!
 *  @brief
 *  Returns the cross track distance of this location relative to the specified start location and end location.
 *
 *  @param startLocation The start location.
 *  @param endLocation The end location.
 *
 *  @return The cross track distance of this location relative to the specified start location and end location.
 */
- (CLLocationDistance)crossTrackDistanceToStartLocation:(nonnull CLLocation *)startLocation
                                            endLocation:(nonnull CLLocation *)endLocation;

// Max latitude.

/*!
 *  @brief
 *  Returns a location representing the cross track point of this location relative to the specified start location and end location.
 *
 *  @param startLocation The start location.
 *  @param endLocation The end location.
 *
 *  @return A location representing the cross track point of this location relative to the specified start location and end location.
 */
- (nonnull CLLocation *)crossTrackLocationToStartLocation:(nonnull CLLocation *)startLocation
                                              endLocation:(nonnull CLLocation *)endLocation;

@end
