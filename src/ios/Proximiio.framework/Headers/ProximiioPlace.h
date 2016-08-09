//
//  ProximiioPlace.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team on 08/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import "ProximiioResource.h"
#import "ProximiioLocation.h"

@interface ProximiioPlace : ProximiioResource

+ (ProximiioPlace *)placeFromJSON:(NSDictionary *)json;

@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *address;
@property (nonatomic, strong) NSArray *tags;
@property (nonatomic, strong) NSString *indoorAtlasVenueID;
@property (nonatomic, strong) ProximiioLocation *location;

@end
