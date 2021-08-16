package com.navtureapps.cordovaplugin;

import org.apache.cordova.*;
import org.json.JSONException;

import android.app.Activity;
import android.util.Log;
import android.content.Intent;
import java.lang.Runnable;
import org.json.JSONObject;


import androidx.annotation.Nullable;
import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat.OnRequestPermissionsResultCallback;

import io.proximi.proximiiolibrary.ProximiioAPI;
import io.proximi.proximiiolibrary.ProximiioBLEDevice;
import io.proximi.proximiiolibrary.ProximiioEddystone;
import io.proximi.proximiiolibrary.ProximiioGeofence;
import io.proximi.proximiiolibrary.ProximiioIBeacon;
import io.proximi.proximiiolibrary.ProximiioInput;
import io.proximi.proximiiolibrary.ProximiioListener;
import io.proximi.proximiiolibrary.ProximiioFloor;
import io.proximi.proximiiolibrary.ProximiioOptions;
import android.Manifest;
import android.os.Build;
import android.content.pm.PackageManager;

public class ProximiioCordova extends CordovaPlugin implements OnRequestPermissionsResultCallback {
    @Nullable private ProximiioAPI proximiio;
    private ProximiioListener listener;
    private String token;
    private boolean enableDebug;
    private Activity activity;
    private CallbackContext context;

    private static final String TAG = "ProximiioCordova";

    private static final String ACTION_SET_TOKEN = "setToken";
    private static final String ACTION_ENABLE_DEBUG = "enableDebug";
    private static final String ACTION_HANDLE_PUSH = "handlePush";
    private static final String ACTION_REQUEST_PERMISSIONS = "requestPermissions";
    private static final String ACTION_START_SCANNING = "startScanning";
    private static final String ACTION_STOP_SCANNING = "stopScanning";

    @Override
    public boolean execute(String action, CordovaArgs args, CallbackContext callbackContext) throws JSONException {
        activity = cordova.getActivity();
        context = callbackContext;
        cordova.setActivityResultCallback(this);
        switch (action) {
            case ACTION_SET_TOKEN:
                if (proximiio == null) {
                    token = args.getString(0);
                    initProximiio();
                }
                break;
            case ACTION_ENABLE_DEBUG:
                String value = args.getString(0);
                enableDebug = value.equals("true");
                log("execute", "Debug mode enabled");
                break;
            case ACTION_START_SCANNING:
                if (proximiio == null && token != null) {
                    initProximiio();
                }
                break;
            case ACTION_STOP_SCANNING:
                if (proximiio != null) {
                    proximiio.destroyService(false);
                    proximiio = null;
                }
                break;
            case ACTION_HANDLE_PUSH:
            case ACTION_REQUEST_PERMISSIONS:
                // REMOVED
                break;
            default:
                if (enableDebug) {
                    Log.d(TAG, "Unknown action: " + action);
                }
                break;
        }
        PluginResult r = new PluginResult(PluginResult.Status.OK);
        context.sendPluginResult(r);
        return true;
    }

    private void initProximiio() {
        ProximiioOptions options = new ProximiioOptions()
            .setNotificationMode(ProximiioOptions.NotificationMode.ENABLED);

        proximiio = new ProximiioAPI("ProximiioCordovaAPI", activity, options);
        proximiio.setActivity(activity);
        listener = new ProximiioListener() {
            @Override
            public void geofenceEnter(ProximiioGeofence geofence) {
                String action = "javascript:proximiio.triggeredGeofence(1, " + geofence.getJSON() + ")";
                log("geofenceEnter", action);
                loadUrl(action);
            }

            @Override
            public void geofenceExit(ProximiioGeofence geofence, @Nullable Long dwellTime) {
                String action = "javascript:proximiio.triggeredGeofence(0, " + geofence.getJSON() + ")";
                log("geofenceExit", action);
                loadUrl(action);
            }

            @Override
            public void changedFloor(@Nullable ProximiioFloor floor) {
                String floorJson = floor != null ? floor.getJSON() : "{}";
                String action = "javascript:proximiio.changedFloor(0, " + floorJson + ")";
                log("changedFloor", action);
                loadUrl(action);
            }

            @Override
            public void position(double lat, double lon, double accuracy) {
                String json = "{\"coordinates\":{\"lat\":" + lat + ", \"lon\":" + lon + "}, \"accuracy\":" + accuracy + "}";
                String action = "javascript:proximiio.updatedPosition(" + json + ")";
                log("position", action);
                loadUrl(action);
            }

            @Override
            public void loggedIn(boolean online, String authKey) {
                String action = "javascript:proximiio.proximiioReady(\"" + proximiio.getVisitorID() + "\")";
                log("initProximiio", action);
                loadUrl(action);
            }

            @Override
            public void loginFailed(LoginError loginError) {
                if (enableDebug) {
                    Log.e(TAG, "LoginError! (" + loginError.toString() + ")");
                }
                String action = "javascript:proximiio.proximiioReady(null)";
                log("initProximiio", action);
                loadUrl(action);
            }

            /**
            * Receives JSON payloads from Proximiio events outputs.
            * @param json JSON received as specified in payload.
            */
            @Override
            public void output(String json) {
                String action = "javascript:proximiio.triggeredOutput(" + json + ")";
                log("output", action);
                loadUrl(action);
            }

            @Override
            public void foundDevice(ProximiioBLEDevice beacon, boolean registered) {
                String action = null;
                if (registered) {
                    try {
                        JSONObject json = new JSONObject(beacon.getInput().getJSON());
                        json.put("accuracy", beacon.getDistance());
                        action = "javascript:proximiio.foundBeacon(" + json.toString() + ")";
                    } catch (JSONException e) {
                        if (enableDebug) {
                            Log.e(TAG, "foundDevice JSONException!", e);
                        }
                    }
                } else {
                    String bleDeviceJSON = getJSONForBLEDevice(beacon);
                    if (bleDeviceJSON != null) {
                        action = "javascript:proximiio.foundBeacon(" + bleDeviceJSON + ")";
                    }
                }
                if (action != null) {
                    log("foundBeacon", action);
                    loadUrl(action);
                }
            }

            @Override
            public void lostDevice(ProximiioBLEDevice beacon, boolean registered) {
                String action = null;
                if (registered) {
                    try {
                        JSONObject json = new JSONObject(beacon.getInput().getJSON());
                        json.put("accuracy", beacon.getDistance());
                        action = "javascript:proximiio.lostBeacon(" + json.toString() + ")";
                    } catch (JSONException e) {
                        if (enableDebug) {
                            Log.e(TAG, "lostDevice JSONException!", e);
                        }
                    }
                } else {
                    String bleDeviceJSON = getJSONForBLEDevice(beacon);
                    if (bleDeviceJSON != null) {
                        action = "javascript:proximiio.lostBeacon(" + bleDeviceJSON + ")";
                    }
                }
                if (action != null) {
                    log("lostBeacon", action);
                    loadUrl(action);
                }
            }
        };

        proximiio.setListener(listener);
        proximiio.setAuth(token, false);
        proximiio.onStart();
    }

    @Nullable
    private String getJSONForBLEDevice(ProximiioBLEDevice beacon) {
        String json = null;
        switch (beacon.getType()) {
            case IBEACON:
                ProximiioIBeacon iBeacon = (ProximiioIBeacon)beacon;
                json = "{\"name\": \"Unknown iBeacon\", \"accuracy\": " + beacon.getDistance() + ",\"uuid\": \"" + iBeacon.getUUID() +"\", \"major\": " + iBeacon.getMajor() + ", \"minor\": " + iBeacon.getMinor() + "}";
                break;
            case EDDYSTONE:
                ProximiioEddystone eddystone = (ProximiioEddystone)beacon;
                json = "{\"name\": \"Unknown Eddystone\", \"accuracy\": " + beacon.getDistance() + ", \"namespace\": \"" + eddystone.getNamespace() + "\", \"instance\": \"" + eddystone.getInstanceID() + "\"}";
                break;
            case GENERIC_BLE:
                json = "{\"name\": \"Unknown Device\", \"accuracy\": " + beacon.getDistance() + ", \"mac\": \"" + beacon.getRSSI() + "\"}";
                break;
        }
        return json;
    }

    private void log(String method, String action) {
        if (enableDebug) {
            Log.d(TAG, method + ": " + action);
        }
    }

    @Override
    public void onStart() {
        super.onStart();
        activity = cordova.getActivity();
        if (token != null && proximiio == null) {
            initProximiio();
        }
    }

    @Override
    public void onStop() {
        super.onStop();
        if (proximiio != null) {
            proximiio.onStop();
            proximiio.destroy();
            proximiio = null;
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        activity.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (proximiio != null) {
            proximiio.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (proximiio != null) {
            proximiio.onActivityResult(requestCode, resultCode, data);
        }
    }

    private void loadUrl(final String url) {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (webView != null) {
                    webView.loadUrl(url);
                }
            }
        });
    }
}
