//
//  Proximiio.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team on 06/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProximiioApplication.h"
#import "ProximiioState.h"
#import "ProximiioMode.h"
#import "ProximiioResourceManager.h"
#import "ProximiioBufferSize.h"

//! Project version number for Proximiio.
FOUNDATION_EXPORT double ProximiioVersionNumber;

//! Project version string for Proximiio.
FOUNDATION_EXPORT const unsigned char ProximiioVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <Proximiio/PublicHeader.h>

@interface ProximiioManager : NSObject

+ (ProximiioManager *)sharedManager;
- (void)requestPermissions;

- (void)enable;
- (void)disable;
- (void)startUpdating;
- (void)stopUpdating;
- (void)extendBackgroundTime;

- (id)initWithDelegate:(id)delegate token:(NSString *)token;
- (id)initWithDelegate:(id)delegate email:(NSString *)email password:(NSString *)password;

- (void)authWithToken:(NSString *)token callback:(void (^)(ProximiioState result))callback;
- (void)authWithEmail:(NSString *)email password:(NSString *)password callback:(void (^)(ProximiioState result))callback;

- (void)setMode:(ProximiioMode)mode;
- (void)setBufferSize:(ProximiioBufferSize)bufferSize;
- (void)setUpdateInterval:(double)updateInterval;

- (void)handlePush:(NSString *)title;
- (void)handleOutput:(NSObject *)payload;
- (void)selectApplication:(NSString *)uuid;

-(NSArray *)places;
-(NSArray *)floors;
-(NSArray *)departments;
-(NSArray *)geofences;
-(NSArray *)applications;

- (NSArray *)geofencesForLocation:(ProximiioLocation *)location;

- (void)addCustomiBeaconUUID:(NSString *)uuid;

- (void)proximiioFloorChanged:(ProximiioFloor *)floor;

- (BOOL)addPlace:(NSString *)name
        location:(CLLocationCoordinate2D)location
         address:(NSString *)address
indoorAtlasVenueID:(NSString *)indoorAtlasVenueId
            tags:(NSArray *)tags
    withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)addFloor:(NSString*)name
     floorPlanID:(NSString*)floorPlanID
           place:(ProximiioPlace*)place
     floorNumber:(NSNumber*)floorNumber
    withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)addDepartment:(NSString*)name
                floor:(ProximiioFloor*)floor
                 tags:(NSArray*)tags
         withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)addGeofence:(NSString*)name
         department:(ProximiioDepartment*)department
           location:(CLLocationCoordinate2D)location
             radius:(double)radius
            address:(NSString*)address
       withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)addiBeaconInput:(NSString*)name
             department:(ProximiioDepartment*)department
               location:(CLLocationCoordinate2D)location
                   uuid:(NSString*)uuid
                  major:(int)major
                  minor:(int)minor
           withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)addEddystoneInput:(NSString*)name
               department:(ProximiioDepartment*)department
                 location:(CLLocationCoordinate2D)location
              namespaceID:(NSString*)namespaceID
               instanceID:(NSString*)instanceID
             withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)addCustomInput:(NSString*)name
            department:(ProximiioDepartment*)department
              location:(CLLocationCoordinate2D)location
          withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)addApplication:(NSString*)name
            eddystones:(BOOL)eddystones
              iBeacons:(BOOL)iBeacons
           indoorAtlas:(BOOL)indoorAtlas
     indoorAtlasApiKey:(NSString*)iaApiKey
indoorAtlasApiKeySecret:(NSString*)iaApiKeySecret
       nativeGeofences:(BOOL)nativeGeofences
             steerPath:(BOOL)steerPath
          steerPathNDD:(NSString*)steerPathNDD
          withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)updatePlace:(NSString *)uuid
               name:(NSString *)name
           location:(CLLocationCoordinate2D)location
            address:(NSString *)address
 indoorAtlasVenueID:(NSString *)indoorAtlasVenueId
               tags:(NSArray *)tags
       withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)updateFloor:(NSString*)ID
               name:(NSString*)name
        floorPlanID:(NSString*)floorPlanID
              place:(ProximiioPlace*)place
        floorNumber:(NSNumber*)floorNumber
       withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)updateDepartment:(NSString*)ID
                    name:(NSString*)name
                   floor:(ProximiioFloor*)floor
                    tags:(NSArray*)tags
            withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)updateGeofence:(NSString*)ID
                  name:(NSString*)name
            department:(ProximiioDepartment*)department
              location:(CLLocationCoordinate2D)location
                radius:(double)radius
               address:(NSString*)address
          withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)updateiBeaconInput:(NSString*)ID
                      name:(NSString*)name
                department:(ProximiioDepartment*)department
                  location:(CLLocationCoordinate2D)location
                      uuid:(NSString*)uuid
                     major:(int)major
                     minor:(int)minor
       triggersFloorChange:(BOOL)triggersFloorChange
       triggersPlaceChange:(BOOL)triggersPlaceChange
                   floorID:(NSString*)floorID
                   placeID:(NSString*)placeID
              withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)updateEddystoneInput:(NSString*)ID
                        name:(NSString*)name
                  department:(ProximiioDepartment*)department
                    location:(CLLocationCoordinate2D)location
                 namespaceID:(NSString*)namespaceID
                  instanceID:(NSString*)instanceID
         triggersFloorChange:(BOOL)triggersFloorChange
         triggersPlaceChange:(BOOL)triggersPlaceChange
                     floorID:(NSString*)floorID
                     placeID:(NSString*)placeID
                withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)updateCustomInput:(NSString*)ID
                     name:(NSString*)name
               department:(ProximiioDepartment*)department
                 location:(CLLocationCoordinate2D)location
      triggersFloorChange:(BOOL)triggersFloorChange
      triggersPlaceChange:(BOOL)triggersPlaceChange
                  floorID:(NSString*)floorID
                  placeID:(NSString*)placeID
             withCallback:(void (^)(BOOL success, NSError* error))callback;

- (BOOL)updateApplication:(NSString*)ID
                     name:(NSString*)name
               eddystones:(BOOL)eddystones
                 iBeacons:(BOOL)iBeacons
              indoorAtlas:(BOOL)indoorAtlas
        indoorAtlasApiKey:(NSString*)iaApiKey
  indoorAtlasApiKeySecret:(NSString*)iaApiKeySecret
          nativeGeofences:(BOOL)nativeGeofences
                steerPath:(BOOL)steerPath
             steerPathNDD:(NSString*)steerPathNDD
             withCallback:(void (^)(BOOL success, NSError* error))callback;

- (void)deletePlace:(NSString *)uuid withCallback:(void (^)(BOOL success, NSError* error))callback;
- (void)deleteFloor:(NSString *)uuid withCallback:(void (^)(BOOL success, NSError* error))callback;
- (void)deleteDepartment:(NSString *)uuid withCallback:(void (^)(BOOL success, NSError* error))callback;
- (void)deleteGeofence:(NSString *)uuid withCallback:(void (^)(BOOL success, NSError* error))callback;
- (void)deleteInput:(NSString *)uuid withCallback:(void (^)(BOOL success, NSError* error))callback;

- (ProximiioApplication *)application;

@property (readonly) BOOL authenticated;
@property (readonly) BOOL remoteMode;
@property (readonly) ProximiioMode mode;
@property (readonly) ProximiioState state;
@property (readonly, strong) ProximiioLocation *lastLocation;
@property (readonly, strong) NSString *visitorId;
@property (weak) id delegate;



@end
