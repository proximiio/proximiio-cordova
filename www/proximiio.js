/*global cordova, module*/

var PLUGIN = 'ProximiioCordova';
var ACTION_SET_TOKEN = 'setToken';
var ACTION_ENABLE_DEBUG = 'enableDebug';
var ACTION_HANDLE_PUSH = 'handlePush';
var ACTION_REQUEST_PERMISSIONS = 'requestPermissions';

var dummy = function (obj) {
  //console.log('proximi.io default callback:' + JSON.stringify(obj));
};

var storage = window.localStorage;
var initialized = false;
var token = '';

var geofenceTriggerCallback = dummy;
var outputTriggerCallback   = dummy;
var positionChangeCallback  = dummy;
var floorChangedCallback    = dummy;
var errorCallback           = dummy;
var proximiioReadyCallback  = dummy;
var beaconFoundCallback     = dummy;
var beaconLostCallback      = dummy;

var needsPersistance = function() {
  var dummyFn = dummy.toString();
  return  !(
          initialized === false &&
          token === '' &&
          geofenceTriggerCallback.toString() === dummyFn &&
          outputTriggerCallback.toString() === dummyFn &&
          positionChangeCallback.toString() === dummyFn &&
          floorChangedCallback.toString() === dummyFn &&
          errorCallback.toString() === dummyFn &&
          proximiioReadyCallback.toString() === dummyFn &&
          beaconFoundCallback.toString() === dummyFn &&
          beaconLostCallback.toString() === dummyFn);
};

var onPause = function() {
  // serialize & persist state  
  if (needsPersistance()) {
    var bundle = {
      initialized: initialized,
      token: token,
      geofenceTriggerCallback: geofenceTriggerCallback.toString(),
      outputTriggerCallback: outputTriggerCallback.toString(),
      positionChangeCallback: positionChangeCallback.toString(),
      floorChangedCallback: floorChangedCallback.toString(),
      errorCallback: errorCallback.toString(),
      proximiioReadyCallback: proximiioReadyCallback.toString(),
      beaconFoundCallback: beaconFoundCallback.toString(),
      beaconLostCallback: beaconLostCallback.toString()
    };
    var serialized = JSON.stringify(bundle);
    storage.setItem(PLUGIN, serialized);    
  }
};

var onResume = function() {
  // load & deserialize state  
  if (!needsPersistance()) {    
    // all values are empty, load from localStorage
    var serialized = storage.getItem(PLUGIN);    
    if (serialized !== undefined && serialized.length > 0) {
      var data = JSON.parse(serialized);
      initialized = data.initialized;
      token = data.token;
      geofenceTriggerCallback = eval('geofenceTriggerCallback = ' + data.geofenceTriggerCallback);
      outputTriggerCallback = eval('outputTriggerCallback = ' + data.outputTriggerCallback);
      positionChangeCallback = eval('positionChangeCallback = ' + data.positionChangeCallback);
      floorChangedCallback = eval('floorChangedCallback = ' + data.floorChangedCallback);
      errorCallback = eval('errorCallback = ' + data.errorCallback);
      proximiioReadyCallback = eval('proximiioReadyCallback = ' + data.proximiioReadyCallback);
      beaconFoundCallback = eval('beaconFoundCallback = ' + data.beaconFoundCallback);
      beaconLostCallback = eval('beaconLostCallback = ' + data.beaconLostCallback);      
    }
  }
};

var onDeviceReady = function() {  
  document.addEventListener("pause", onPause, false);
  document.addEventListener("resume", onResume, false);
};

document.addEventListener("deviceready", onDeviceReady);

module.exports = {

  // ACTIONS

  requestPermissions: function(onSuccess, onError) {
    cordova.exec(onSuccess, onError, PLUGIN, ACTION_REQUEST_PERMISSIONS, []);
  },

  /**
   * Sets authorization token and initializes Proximi.io native plugin
   *
   * @param authToken {String}
   * @param successCallback {Function}
   * @param failureCallback {Function}
   * @returns
   */
  setToken: function (authToken, onSuccess, onError) {    
    initialized = true;
    token = authToken;
    cordova.exec(onSuccess, onError, PLUGIN, ACTION_SET_TOKEN, [authToken]);
  },

  /**
   * Sets debug output from native plugin (enables logs)
   *
   * @param enableDebug {Boolean}
   * @param successCallback {Function}
   * @param failureCallback {Function}
   * @returns
   */
  setDebugOutput: function (enableDebug, onSuccess, onError) {
    enableDebug = [(enableDebug || enableDebug > 0) ? 'true' : 'false'];
    cordova.exec(onSuccess, onError, PLUGIN, ACTION_ENABLE_DEBUG, enableDebug);
  },

  /**
   * Sets automatic output push message handling by Proximi.io Native SDK
   *
   * @param value {Boolean}
   * @param successCallback {Function}
   * @param failureCallback {Function}
   * @returns
   */
  handlePush: function (value, onSuccess, onError) {
    value = [(value || value > 0) ? 'true' : 'false'];
    cordova.exec(onSuccess, onError, PLUGIN, ACTION_HANDLE_PUSH, value);
  },

  // CALLBACKS

  /**
   * Sets input trigger callback
   * this function is called when visitor enters or leaves Geofence
   * DEPRECATED, replaced by setGeofenceTriggerCallback
   *
   * @param callback(enter, geofence) {Function}
   * @returns
   */
  setInputTriggerCallback: function (callback) { geofenceTriggerCallback = callback; },

  /**
   * Sets geofence trigger callback
   * this function is called when visitor enters or leaves Geofence
   *
   * @param callback(enter, geofence) {Function}
   * @returns
   */
  setGeofenceTriggerCallback: function (callback) { geofenceTriggerCallback = callback; },

  /**
   * Sets output trigger callback
   * this function is called when Proximi.io cloud produces output payload
   *
   * @param callback(output) {Function}
   * @returns
   */
  setOutputTriggerCallback: function (callback) { outputTriggerCallback = callback; },

  /**
   * Sets position change callback
   * this function is called when Proximi.io detects position change
   *
   * @param callback(position) {Function}
   * @returns
   */
  setPositionChangeCallback: function (callback) { positionChangeCallback = callback; },

  /**
   * Floor change callback
   * this function is called when Proximi.io detects floor change
   *
   * @param callback(floor) {Function}
   * @returns
   */
  setFloorChangedCallback: function (callback) { floorChangedCallback = callback; },

  /**
   * Fires after proximi.io is completely initialized
   * this function is called when Proximi.io detects floor change
   *
   * Provides Visitor ID in callback parameter
   *
   * @param callback(visitorId) {Function}
   * @returns
   */
  setProximiioReadyCallback: function (callback) { proximiioReadyCallback = callback; },

  /**
   * Called when Beacon is found
   *
   * @param callback(beacon) {Function}
   * @returns
   */
  setBeaconFoundCallback: function (callback) { beaconFoundCallback = callback; },

  /**
   * Called after Beacon is lost
   *
   * @param callback(beacon) {Function}
   * @returns
   */
  setBeaconLostCallback: function (callback) { beaconLostCallback = callback; },

  // NATIVE PLUGIN calls

  triggeredInput: function (enter, geofence) { geofenceTriggerCallback(enter, geofence); },

  triggeredGeofence: function (enter, geofence) { geofenceTriggerCallback(enter, geofence); },

  triggeredOutput: function (output) { outputTriggerCallback(output); },

  updatedPosition: function (coords) { positionChangeCallback(coords); },

  changedFloor: function (floor) { floorChangedCallback(floor); },

  proximiioReady: function (visitorId) { proximiioReadyCallback(visitorId); },

  foundBeacon: function (beacon) { beaconFoundCallback(beacon); },

  lostBeacon: function (beacon) { beaconLostCallback(beacon); },

  encounteredError: function (code, id, str) {
    var errorObj = {};
    errorObj.domain = id;
    errorObj.code = code;
    errorObj.text = str;
    errorCallback(errorObj);
  },

  onDeviceReady: onDeviceReady
};
