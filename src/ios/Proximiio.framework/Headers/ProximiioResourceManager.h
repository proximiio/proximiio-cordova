//
//  ProximiioResourceManager.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team on 08/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ProximiioAPIResult.h"
#import "ProximiioInput.h"
#import "ProximiioDepartment.h"
#import "ProximiioPlace.h"
#import "ProximiioFloor.h"
#import "ProximiioApplication.h"

@interface ProximiioResourceManager : NSObject

- (NSArray *)allApplications;
- (NSArray *)allPlaces;
- (NSArray *)allFloors;
- (NSArray *)allDepartments;
- (NSArray *)allGeofences;
- (NSArray *)allInputs;

+ (id)sharedManager;

- (void)fetchWithCallback:(void (^)(ProximiioAPIResult result))callback;

- (ProximiioApplication *)applicationWithUUID:(NSString *)uuid;
- (ProximiioInput *)inputWithUUID:(NSUUID *)uuid major:(int)major minor:(int)minor;
- (ProximiioInput *)inputWithNamespace:(NSString *)ns instance:(NSString *)instance;
- (ProximiioFloor *)floorWithUUID:(NSString *)uuid;
- (ProximiioPlace *)placeWithUUID:(NSString *)uuid;
- (ProximiioDepartment *)departmentWithUUID:(NSString *)uuid;

- (void)update:(NSString *)resource data:(NSDictionary *)data;

- (void)upsert:(NSString *)resource
    parameters:(NSDictionary *)parameters
  withCallback:(void (^)(ProximiioAPIResult result))callback;

- (void)upsertPlace:(NSDictionary *)parameters withCallback:(void (^)(ProximiioAPIResult result))callback;
- (void)upsertFloor:(NSDictionary *)parameters withCallback:(void (^)(ProximiioAPIResult result))callback;
- (void)upsertDepartment:(NSDictionary *)parameters withCallback:(void (^)(ProximiioAPIResult result))callback;
- (void)upsertGeofence:(NSDictionary *)parameters withCallback:(void (^)(ProximiioAPIResult result))callback;
- (void)upsertInput:(NSDictionary *)parameters withCallback:(void (^)(ProximiioAPIResult result))callback;
- (void)upsertApplication:(NSDictionary *)parameters withCallback:(void (^)(ProximiioAPIResult result))callback;

@property (readonly, nonatomic, strong) NSMutableDictionary *applications;
@property (readonly, nonatomic, strong) NSMutableDictionary *places;
@property (readonly, nonatomic, strong) NSMutableDictionary *floors;
@property (readonly, nonatomic, strong) NSMutableDictionary *departments;
@property (readonly, nonatomic, strong) NSMutableDictionary *geofences;
@property (readonly, nonatomic, strong) NSMutableDictionary *inputs;

@end

@protocol ProximiioResourceManagerDelegate <NSObject>

- (void)didUpdateApplications:(NSDictionary *)applications;
- (void)didUpdatePlaces:(NSDictionary *)places;
- (void)didUpdateFloors:(NSDictionary *)floors;
- (void)didUpdateDepartments:(NSDictionary *)departments;
- (void)didUpdateGeofences:(NSDictionary *)geofences;
- (void)didUpdateInputs:(NSDictionary *)inputs;

@end
