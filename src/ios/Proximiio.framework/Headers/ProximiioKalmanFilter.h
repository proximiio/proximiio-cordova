//
//  ProximiioKalmanFilter.h
//  ProximiioApp
//
//  Created by Proximi.io Developer Team on 10/06/16.
//  Copyright © 2016 Proximi.io. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ProximiioKalmanFilter : NSObject

- (void)updateWithSignal:(double)signal;
- (double)stateEstimate;

@end
