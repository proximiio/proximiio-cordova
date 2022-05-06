/********* proximiio.m Cordova Plugin Implementation *******/

#import <Cordova/CDV.h>

@import Proximiio;
@import ProximiioProcessor;

@import WebKit;

@interface proximiio : CDVPlugin <ProximiioDelegate> {
    // Member variables go here.
}

@property (readwrite, assign) BOOL mEnableDebug;
@property (readwrite, assign) BOOL mHandlePush;
// Add PDR Processor
@property (readwrite, strong) ProximiioPDRProcessor *pdr;
@property (nonatomic, strong) CLLocationManager *locationManager;

- (void)setToken:(CDVInvokedUrlCommand*)command;
- (void)enableDebug:(CDVInvokedUrlCommand*)command;
- (void)handlePush:(CDVInvokedUrlCommand*)command;
- (void)requestPermissions:(CDVInvokedUrlCommand*)command;
- (void)startScanning:(CDVInvokedUrlCommand*)command;
- (void)stopScanning:(CDVInvokedUrlCommand*)command;

@end

@implementation proximiio

// ACTIONS

- (void)setToken:(CDVInvokedUrlCommand*)command {
    
        NSString* callbackId    = [command callbackId];
        NSString* authToken     = [[command arguments] objectAtIndex:0];
        [[self commandDelegate] runInBackground:^{

            // Dispatch in background thread! Very important!
            dispatch_sync(dispatch_get_main_queue(),^ {
                Proximiio *proximiio = (Proximiio *)[Proximiio sharedInstance];
                proximiio.delegate = self;
                // Set buffer size explicitely to mini
                [proximiio setBufferSize:kProximiioBufferMini];
                // Allocate PDR
                self->_pdr = [[ProximiioPDRProcessor alloc] init];
                // Set PDR threshold
                self->_pdr.threshold = 4;
                // Add PDR to proximi
                [ProximiioLocationManager.sharedManager addProcessor:self->_pdr avoidDuplicates:YES];
                [proximiio authWithToken:authToken callback:^(ProximiioState resultState) {
                    __block CDVPluginResult* result;
                    if (resultState == kProximiioReady) {
                        
                        [proximiio sync:^(BOOL completed) {

                            // Start the positioning process
                            [proximiio enable];
                            [proximiio startUpdating];
                            
                            NSString *action = [NSString stringWithFormat:@"proximiio.proximiioReady(\"%@\");", proximiio.visitorId];
                            [self runJavascript:action];
                            result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
                        }];
                        
                    } else {
                        [self runJavascript:@"proximiio.proximiioReady(null);"];
                        result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"Proximi.io Authorization Failed"];
                    }
                    [[self commandDelegate] sendPluginResult:result callbackId:callbackId];
                }];
            });
        }];
}

- (void)enableDebug:(CDVInvokedUrlCommand*)command {
    NSString* callbackId    = [command callbackId];
    NSString* enableString  = [[command arguments] objectAtIndex:0];
    _mEnableDebug = [enableString isEqualToString:@"true"];
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [[self commandDelegate] sendPluginResult:result callbackId:callbackId];
}

- (void)startScanning:(CDVInvokedUrlCommand*)command {
    NSString* callbackId    = [command callbackId];
    [[Proximiio sharedInstance] startUpdating];
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [[self commandDelegate] sendPluginResult:result callbackId:callbackId];
}

- (void)stopScanning:(CDVInvokedUrlCommand*)command {
    NSString* callbackId    = [command callbackId];
    [[Proximiio sharedInstance] stopUpdating];
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [[self commandDelegate] sendPluginResult:result callbackId:callbackId];
}

- (void)requestPermissions:(CDVInvokedUrlCommand*)command {
    NSString* callbackId    = [command callbackId];
    UIUserNotificationType userNotificationTypes = (UIUserNotificationTypeAlert |
                                                    UIUserNotificationTypeBadge |
                                                    UIUserNotificationTypeSound);
    UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:userNotificationTypes
                                                                             categories:nil];
    
    [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    [[UIApplication sharedApplication] registerForRemoteNotifications];
    
    _locationManager = [[CLLocationManager alloc] init];
    [_locationManager requestAlwaysAuthorization];
    
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [[self commandDelegate] sendPluginResult:result callbackId:callbackId];
}

- (void)handlePush:(CDVInvokedUrlCommand*)command {
    NSString* callbackId    = [command callbackId];
    NSString* handleString  = [[command arguments] objectAtIndex:0];
    _mHandlePush = [handleString isEqualToString:@"true"];
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

- (void)proximiioFloorChanged:(ProximiioFloor *)floor {
    NSString *action = [NSString stringWithFormat:@"proximiio.changedFloor(%@);", [self toJSON:floor.data]];
    [self log:@"proximiioChangedFloor" action:action];
    [self runJavascript:action];
}


- (BOOL)proximiioHandlePushMessage:(NSString*)title {
    if (_mHandlePush) {
        NSString *action = [NSString stringWithFormat:@"proximiio.pushMessage(\"%@\");", title];
        [self log:@"proximiioHandlePushMessage" action:action];
        [self runJavascript:action];
    }
    return _mHandlePush;
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
    NSString *action = [NSString stringWithFormat:@"proximiio.triggeredGeofence(1, %@);", [self toJSON:geofence.data]];
    [self log:@"proximiioExitedGeofence" action:action];
    [self runJavascript:action];
}

- (void)proximiioExitedGeofence:(ProximiioGeofence*)geofence {
    NSString *action = [NSString stringWithFormat:@"proximiio.triggeredGeofence(0, %@);", [self toJSON:geofence.data]];
    [self log:@"proximiioExitedGeofence" action:action];
    [self runJavascript:action];
}

- (void)proximiioPositionUpdated:(ProximiioLocation *)location {
    // Only listen to location updates with a location
    if(location != nil) {
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
}

- (void)proximiio:(Proximiio *)proximiio encounteredError:(NSError*)error {
    NSString *action = [NSString stringWithFormat:@"proximiio.encounteredError(%ld, \"%@\", \"%@\");", (long)error.code, error.domain, error.localizedDescription];
    [self log:@"proximiio:encounteredError" action:action];
    [self runJavascript:action];
}

- (NSDictionary *)iBeaconToDictionary:(ProximiioIBeacon *)beacon {
    return @{
             @"type": @"iBeacon",
             @"uuid": beacon.uuid.UUIDString,
             @"major": @(beacon.major),
             @"minor": @(beacon.minor),
             @"accuracy": @(beacon.proximity)
             };
}

- (void)proximiioFoundiBeacon:(ProximiioIBeacon *)beacon {
    NSString *action = [NSString stringWithFormat:@"proximiio.foundBeacon(%@);", [self toJSON:[self iBeaconToDictionary:beacon]]];
    [self log:@"proximiioFoundiBeacon" action:action];
    [self runJavascript:action];
}

- (void)proximiioLostiBeacon:(ProximiioIBeacon *)beacon {
    NSString *action = [NSString stringWithFormat:@"proximiio.lostBeacon(%@);", [self toJSON:[self iBeaconToDictionary:beacon]]];
    [self log:@"proximiioLostiBeacon" action:action];
    [self runJavascript:action];
}

- (void)proximiioUpdatediBeacon:(ProximiioIBeacon *)beacon {}

- (NSDictionary *)eddystoneToDictionary:(ProximiioEddystoneBeacon *)beacon {
    return @{
             @"type": @"Eddystone",
             @"namespace": beacon.Namespace,
             @"instanceId": beacon.InstanceID,
             @"accuracy": beacon.accuracy
             };
}

- (void)proximiioFoundEddystoneBeacon:(ProximiioEddystoneBeacon *)beacon isRegistered:(BOOL)isRegistered {
    NSString *action = [NSString stringWithFormat:@"proximiio.foundBeacon(%@);", [self toJSON:[self eddystoneToDictionary:beacon]]];
    [self log:@"proximiioFoundEddystone" action:action];
    [self runJavascript:action];
}

- (void)proximiioLostEddystoneBeacon:(ProximiioEddystoneBeacon *)beacon isRegistered:(BOOL)isRegistered {
    NSString *action = [NSString stringWithFormat:@"proximiio.lostBeacon(%@);", [self toJSON:[self eddystoneToDictionary:beacon]]];
    [self log:@"proximiioLostEddystone" action:action];
    [self runJavascript:action];
}

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
    dispatch_async(dispatch_get_main_queue(), ^{
        WKWebView *webView = (WKWebView *)self.webView;
        [webView evaluateJavaScript:command completionHandler:^(id _Nullable result, NSError * _Nullable error) {
            if (error) { NSLog(@"[ProximiioSDK] Javascript error: %@", error); }
        }];
    });
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
