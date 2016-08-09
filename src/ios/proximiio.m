/********* proximiio.m Cordova Plugin Implementation *******/

#import <Cordova/CDV.h>

#import <Proximiio/Proximiio.h>

@interface proximiio : CDVPlugin <ProximiioDelegate> {
  // Member variables go here.

}

@property (readwrite, assign) BOOL mEnableDebug;
@property (readwrite, assign) BOOL mHandlePush;

- (void)setToken:(CDVInvokedUrlCommand*)command;
- (void)enableDebug:(CDVInvokedUrlCommand*)command;
- (void)handlePush:(CDVInvokedUrlCommand*)command;

@end

@implementation proximiio

// ACTIONS

- (void)setToken:(CDVInvokedUrlCommand*)command {
    NSString* callbackId    = [command callbackId];
    NSString* authToken     = [[command arguments] objectAtIndex:0];

    [[self commandDelegate] runInBackground:^{
        [[Proximiio sharedInstance] authWithToken:authToken callback:^(ProximiioState resultState) {
          CDVPluginResult* result;
          if (resultState == kProximiioReady) {
            [self runJavascript:@"proximiio.proximiioReady(\"abcdefgh\");"];
            result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
          } else {
            [self runJavascript:@"proximiio.proximiioReady(null);"];
            result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"Proximi.io Authorization Failed"];
          }
          [[self commandDelegate] sendPluginResult:result callbackId:callbackId];
        }];
    }];
}

- (void)enableDebug:(CDVInvokedUrlCommand*)command {
    NSString* callbackId    = [command callbackId];
    NSString* enableString  = [[command arguments] objectAtIndex:0];
    _mEnableDebug = [enableString isEqualToString:@"true"];
    NSLog(@"enableDebug set to: %d", _mEnableDebug);
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [[self commandDelegate] sendPluginResult:result callbackId:callbackId];
}

- (void)handlePush:(CDVInvokedUrlCommand*)command {
    NSString* callbackId    = [command callbackId];
    NSString* handleString  = [[command arguments] objectAtIndex:0];
    _mHandlePush = [handleString isEqualToString:@"true"];
    NSLog(@"_mHandlePush set to: %d", _mHandlePush);
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [[self commandDelegate] sendPluginResult:result callbackId:callbackId];
}

// HELPERS

- (NSString*)beaconToString:(ProximiioBeacon*)beacon {
  if (beacon.type == kProximiioInputTypeIBeacon) {
    ProximiioIBeacon *iBeacon = (ProximiioIBeacon *)beacon;
    NSDictionary *data = @{
      @"type": @"iBeacon",
      @"uuid": [[iBeacon uuid] UUIDString],
      @"major": @([iBeacon major]),
      @"minor": @([iBeacon minor]),
      @"accuracy": @([iBeacon distance])
    };
    return [self toJSON:data];
  } else if (beacon.type == kProximiioInputTypeEddystone){
    ProximiioEddystoneBeacon *eddy = (ProximiioEddystoneBeacon *)beacon;
    NSDictionary *data = @{
      @"type": @"Eddystone",
      @"namespace": eddy.Namespace,
      @"instanceId": eddy.InstanceID,
      @"accuracy": eddy.accuracy
    };
    return [self toJSON:data];
  } else {
    NSDictionary *data = @{
      @"type": @"Custom"
    };
    return [self toJSON:data];
  }
}

// PROXIMI.IO DELEGATES

- (BOOL)proximiioHandlePushMessage:(NSString*)title {
    if (_mHandlePush) {
      return YES;
    } else {
      NSString *action = [NSString stringWithFormat:@"proximiio.pushMessage(\"%@\");", title];
      [self log:@"proximiioHandlePushMessage" action:action];
      [self runJavascript:action];
      return NO;
    }
}

- (void)proximiioFloorChanged:(ProximiioFloor *)floor {
    NSString *action = [NSString stringWithFormat:@"proximiio.changedFloor(%@);", [self toJSON:floor.data]];
    [self log:@"proximiioHandleOutput" action:action];
    [self runJavascript:action];
}

- (void)proximiioHandleOutput:(NSObject*)payload {
    NSString *action;
    if ([payload isKindOfClass:NSDictionary.class]) {
      action = [NSString stringWithFormat:@"proximiio.triggeredOutput(%@);", [self toJSON:(NSDictionary *)payload]];
    } else {
      action = [NSString stringWithFormat:@"proximiio.triggeredOutput(%@);", payload];
    }
    [self log:@"proximiioHandleOutput" action:action];
    [self runJavascript:action];
}

- (void)proximiioEnteredGeofence:(ProximiioGeofence*)geofence {
    NSString *action = [NSString stringWithFormat:@"proximiio.triggeredInput(1, %@);", [self toJSON:geofence.data]];
    [self log:@"proximiioExitedGeofence" action:action];
    [self runJavascript:action];
}

- (void)proximiioExitedGeofence:(ProximiioGeofence*)geofence {
    NSString *action = [NSString stringWithFormat:@"proximiio.triggeredInput(0, %@);", [self toJSON:geofence.data]];
    [self log:@"proximiioExitedGeofence" action:action];
    [self runJavascript:action];
}

- (void)proximiioPositionUpdated:(CLLocation *)location {
    NSDictionary *position = @{
      @"coordinates": @{
        @"lat": @(location.coordinate.latitude),
        @"lon": @(location.coordinate.longitude)
      },
      @"accuracy": @(location.horizontalAccuracy)
    };
    NSString *action = [NSString stringWithFormat:@"proximiio.updatedPosition(%@);", [self toJSON:position]];
    [self log:@"proximiioPositionUpdated" action:action];
    [self runJavascript:action];
}

- (void)proximiio:(Proximiio *)proximiio encounteredError:(NSError*)error {
    NSString *action = [NSString stringWithFormat:@"proximiio.encounteredError(%ld, \"%@\", \"%@\");", (long)error.code, error.domain, error.localizedDescription];
    [self log:@"proximiio:encounteredError" action:action];
    [self runJavascript:action];
}

- (void)proximiioFoundiBeacon:(ProximiioIBeacon *)beacon isRegistered:(BOOL)isRegistered {}
- (void)proximiioLostiBeacon:(ProximiioIBeacon *)beacon isRegistered:(BOOL)isRegistered {}
- (void)proximiioUpdatediBeacon:(ProximiioIBeacon *)beacon isRegistered:(BOOL)isRegistered {}
- (void)proximiioFoundEddystoneBeacon:(ProximiioEddystoneBeacon *)beacon isRegistered:(BOOL)isRegistered {}
- (void)proximiioLostEddystoneBeacon:(ProximiioEddystoneBeacon *)beacon isRegistered:(BOOL)isRegistered {}
- (void)proximiioUpdatedEddystoneBeacon:(ProximiioEddystoneBeacon *)beacon isRegistered:(BOOL)isRegistered {}
- (void)proximiioUpdatedApplications {}
- (void)proximiioUpdatedPlaces {}
- (void)proximiioUpdatedDepartments {}
- (void)proximiioUpdatedFloors {}
- (void)proximiioUpdatedGeofences {}
- (void)proximiioUpdatedInputs {}
- (void)onProximiioAuthorizationInvalid {}
- (void)onProximiioAuthorizationFailure {}

- (void)log:(NSString *)method action:(NSString *)action {
    _mEnableDebug = YES;
    if (_mEnableDebug) {
      NSLog(@"[ProximiioSDK] %@: %@", method, action);
    }
}

- (void)runJavascript:(NSString *)command {
    [[self webView] performSelectorOnMainThread:@selector(stringByEvaluatingJavaScriptFromString:) withObject:command waitUntilDone:NO];
}

-(NSString*)toJSON:(NSDictionary *)data {
   NSError *error;
   NSData *jsonData = [NSJSONSerialization dataWithJSONObject:data
                                                 options:(NSJSONWritingOptions)0
                                                   error:&error];
   if (!jsonData) {
      NSLog(@"json error: %@", error.localizedDescription);
      return @"{}";
   } else {
      return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
   }
}

@end
