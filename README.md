# Proximi.io Cordova Plugin #

# Introduction

Welcome to the Proximi.io Cordova plugin reference. Use our library to hook into the new Proximi.io platform.

Code samples can be found on the right side of the page.

# Quick Start

## Installing the plugin

Cordova plugin installation is really easy. Issue the following command in your Cordova project:

<code>cordova plugin add https://github.com/proximiio/proximiio-cordova.git</code>

In case you're using Ionic, the installation is similar:

<code>ionic plugin add https://github.com/proximiio/proximiio-cordova.git</code>

## Basic usage

proximiio object is globally accessible variable exposed by Cordova.
It's usually good idea to put proximiio initialization inside deviceReady or
platformReady callbacks

```javascript
var PROXIMIIO_TOKEN = 'PUT_YOUR_APPLCIATION_TOKEN_HERE';

function initProximiio() {
	proximiio.setToken(PROXIMIIO_TOKEN);
	proximiio.setDebugOutput(true, null, null);

	proximiio.setOutputTriggerCallback(function (output) {
	  // Your code here
	});

	proximiio.setInputTriggerCallback(function(enter, geofence) {
	  // Your code here
	});

	proximiio.setPositionChangeCallback(function(coords) {
	  // Your code here, for example:
	  //document.getElementById("position-latitude").innerHTML = coords.coordinates.lat;
	  //document.getElementById("position-longitude").innerHTML = coords.coordinates.lon;
	  //document.getElementById("position-accuracy").innerHTML = coords.accuracy;
	});
},
```

Happy coding! Not what you needed? Check out the rest below.

# Authentication

First thing to do before adding any callbacks is to authenticate with the Proximi.io Proximity Platform. You can find the required token from the Proximi.io Web Portal Manage Applications section.

```javascript
proximiio.setToken(PROXIMIIO_TOKEN);
```

# Callbacks

Cordova plugin offers multiple callbacks which are triggered by different events.

## proximiioReadyCallback

proximiioReadyCallback is triggered when Proximio.io SDK is fully initialized;

```javascript
  proximiio.setProximiioReadyCallback(function(visitorId) {
    document.getElementById("visitor_id").innerHTML = visitorId;
  })
```

## geofenceTriggerCallback

geofenceTriggerCallback is triggered when someone enters or exits a defined geofence.

```javascript
proximiio.setGeofenceTriggerCallback(function(enter, geofence) {
  document.getElementById("address").innerHTML = geofence.address                                
	document.getElementById("lat").innerHTML = geofence.area.lat                                
	document.getElementById("long").innerHTML = geofence.area.lng                                
	document.getElementById("createdAt").innerHTML = geofence.createdAt                              
	document.getElementById("department_id").innerHTML = geofence.department_id                          
	document.getElementById("id").innerHTML = geofence.id                                     
	document.getElementById("name").innerHTML = geofence.name                                   
	document.getElementById("organization_id").innerHTML = geofence.organization_id                        
	document.getElementById("place_id").innerHTML = geofence.place_id                               
	document.getElementById("radius").innerHTML = geofence.radius                                 
	document.getElementById("updatedAt").innerHTML = geofence.updatedAt                              
});
```

## outputTriggerCallback  

outputTriggerCallback is triggered when output payload is available. Output payload is defined in the web portal action flow editor using function node and event transmitter.

```javascript
proximiio.setOutputTriggerCallback(function (output) {
  // Your code here
  //
  // Contains the JSON object as it was defined in the
  // action flow editor before passing it to the event transmitter as a payload
});
```

## positionChangeCallback  

positionChangeCallback is triggered when Proximi.io SDK detects a major change in the users position.

```javascript
proximiio.setPositionChangeCallback(function(coords) {
	// Your code here, for example:
	document.getElementById("position-latitude").innerHTML = coords.coordinates.lat;
	document.getElementById("position-longitude").innerHTML = coords.coordinates.lon;
	document.getElementById("position-accuracy").innerHTML = coords.accuracy;
});
```

## floorChangedCallback  

floorChangedCallback is triggered when Proximi.io SDK detects a floor change.

```javascript
proximiio.setFloorChangedCallback(function(floor) {
	// Your code here, for example:
	document.getElementById("floor-anchors").innerHTML = JSON.stringify(floor.anchors, null, 4)
	document.getElementById("floor-createdAt").innerHTML = floor.createdAt;
	document.getElementById("floor-id").innerHTML = floor.id;
	document.getElementById("floor-name").innerHTML = floor.name;
	document.getElementById("floor-organization_id").innerHTML = floor.organization_id;
	document.getElementById("floor-.place_id").innerHTML = floor.place_id;
	document.getElementById("floor-updatedAt").innerHTML = floor.updatedAt;
});
```

## beaconFoundCallback

beaconFoundCallback is triggered when Proximio.io SDK detects a beacon;

```javascript
  proximiio.setBeaconFoundCallback(function(beacon) {
    document.getElementById("beacon-uuid").innerHTML = beacon.uuid;
  	document.getElementById("beacon-major").innerHTML = beacon.major;
  	document.getElementById("beacon-minor").innerHTML = beacon.minor;
  })
```

## beaconLostCallback

beaconLostCallback is triggered when Proximio.io SDK looses beacon from sight;

```javascript
  proximiio.setBeaconLostCallback(function(beacon) {
    document.getElementById("beacon-uuid").innerHTML = beacon.uuid;
  	document.getElementById("beacon-major").innerHTML = beacon.major;
  	document.getElementById("beacon-minor").innerHTML = beacon.minor;
  })
```

## floorChangedCallback

floorChangedCallback is triggered when Proximio.io SDK detects a floor change;

```javascript
  proximiio.setFloorChangedCallback(function(floor) {
    document.getElementById("floor").innerHTML = floor;
  })
```

## Push Handling

sets Proximi.io SDK to handle output Push messages automatically

```javascript
  proximiio.setHandlePush(true);
```

## errorCallback   

```javascript
// Example TBA
```

### Android Gotchas ###

depending on your project configuration you may encounter a build error:

```
	uses-sdk:minSdkVersion 14 cannot be smaller than version 18 declared in library ....
```

in that case edit platforms/android/AndroidManifest.xml and replace

```
    <uses-sdk android:minSdkVersion="14" android:targetSdkVersion="22" />
with
    <uses-sdk android:minSdkVersion="18" android:targetSdkVersion="22" />
```
