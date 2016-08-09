package com.navtureapps.cordovaplugin;

import org.apache.cordova.*;
import org.json.JSONException;
import android.widget.Toast;
import android.app.Activity;
import android.util.Log;
import android.content.Intent;
import java.lang.Runnable;
import java.util.ArrayList;
import org.json.JSONObject;
import android.support.annotation.Nullable;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat.OnRequestPermissionsResultCallback;
import javax.security.auth.callback.Callback;
import io.proximi.proximiiolibrary.Proximiio;
import io.proximi.proximiiolibrary.ProximiioBeacon;
import io.proximi.proximiiolibrary.ProximiioFactory;
import io.proximi.proximiiolibrary.ProximiioGeofence;
import io.proximi.proximiiolibrary.ProximiioListener;
import io.proximi.proximiiolibrary.ProximiioFloor;
import android.Manifest;
import android.os.Build;
import android.content.pm.PackageManager;

public class ProximiioCordova extends CordovaPlugin implements OnRequestPermissionsResultCallback {
  private Proximiio proximiio;
  private ProximiioListener listener;
  private String id;
  private String token;
  private boolean handlePush = true;
  private boolean enableDebug = false;

  private Activity activity;
  private static final String TAG = "ProximiioCordova";

  CallbackContext context;

  private static final String ACTION_SET_TOKEN = "setToken";
  private static final String ACTION_ENABLE_DEBUG = "enableDebug";
  private static final String ACTION_HANDLE_PUSH = "handlePush";

  String [] permissions = { Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission.ACCESS_FINE_LOCATION };

  @Override
  public boolean execute(String action, CordovaArgs args, CallbackContext callbackContext) throws JSONException {
    activity = cordova.getActivity();
    context = callbackContext;
    cordova.setActivityResultCallback(this);
    if (action.equals(ACTION_SET_TOKEN)) {
        if (proximiio == null) {
          token = args.getString(0);
          if(hasPermisssion()) {
              PluginResult r = new PluginResult(PluginResult.Status.OK);
              context.sendPluginResult(r);
              initProximiio();
              return true;
          }
          else {
              Log.d(TAG, "Permissions not found, requesting");
              PermissionHelper.requestPermissions(this, 0, permissions);
          }
          return true;
        }
    } else if (action.equals(ACTION_ENABLE_DEBUG)) {
      String value = args.getString(0);
      enableDebug = value.equals("true");
      log("execute", "Debug mode enabled");
    } else if (action.equals(ACTION_HANDLE_PUSH)) {
      String value = args.getString(0);
      handlePush = value.equals("true");
    }
    return true;
  }

  private void initProximiio() {
    proximiio = ProximiioFactory.getProximiio(activity, activity);
    listener = new ProximiioListener() {
      @Override
      public void geofenceEnter(final ProximiioGeofence geofence) {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
              String geofenceJson = geofence.getJSON().toString();
              String action = "javascript:proximiio.triggeredGeofence(1, " + geofenceJson + ")";
              log("geofenceEnter", action);
              webView.loadUrl(action);
            }
        });
      }

      @Override
      public void geofenceExit(final ProximiioGeofence geofence) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            String geofenceJson = geofence.getJSON().toString();
            String action = "javascript:proximiio.triggeredGeofence(0, " + geofenceJson + ")";
            log("geofenceExit", action);
            webView.loadUrl(action);
          }
        });
      }

      @Override
      public void changedFloor(@Nullable ProximiioFloor floor) {
        if (floor != null) {
          final String floorJson = floor.getJSON().toString();
          activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                  String action = "javascript:proximiio.changedFloor(0, " + floorJson + ")";
                  log("changedFloor", action);
                  webView.loadUrl(action);
                }
          });
        }
      }

      @Override
      public void position(final double lat, final double lon, final double accuracy) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            String json = "{\"coordinates\":{\"lat\":" + lat + ", \"lon\":" + lon + "}, \"accuracy\":" + accuracy + "}";
            String action = "javascript:proximiio.updatedPosition(" + json + ")";
            if (enableDebug) {
              log("position", action);
            }
            webView.loadUrl(action);
          }
        });
      }

      @Override
      public void loginFailed(LoginError loginError) {
        Log.e(TAG, "LoginError! (" + loginError.toString() + ")");
      }


      /**
       * Push output from Proximiio
       * @param title Text received
       * @return Is this push handled? (If not, Proximiio generates a snackbar)
       */
      @Override
      public boolean push(String title) {
        return false;
      }

      /**
      * Receives JSON payloads from Proximiio events outputs.
      * @param json JSON received as specified in payload.
      */
      @Override
      public void output(final JSONObject json) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            String action = "javascript:proximiio.triggeredOutput(" + json.toString() + ")";
            log("output", action);
            webView.loadUrl(action);
          }
        });
      }

      @Override
      public void foundBeacon(final ProximiioBeacon beacon, final boolean registered) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            String action;
            if (registered) {
              action = "javascript:proximiio.foundBeacon(" + beacon.getInput().getJSON().toString() + ")";
            } else {
              String json = "{\"name\": \"Unknown Beacon\", \"accuracy\": "+ beacon.getAccuracy() + ",\"uuid\": \"" + beacon.getUUID() +"\", \"major\": " + beacon.getMajor() + ", \"minor\": " + beacon.getMinor() + ", \"namespace\": \"" + beacon.getNamespace() + "\", \"instance\": \"" + beacon.getInstanceID() + "\"}";
              action = "javascript:proximiio.foundBeacon(" + json + ")";
            }
            log("foundBeacon", action);
            webView.loadUrl(action);
          }
        });
      }

      @Override
      public void lostBeacon(final ProximiioBeacon beacon, final boolean registered) {
        activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
            String action;
            if (registered) {
              action = "javascript:proximiio.lostBeacon(" + beacon.getInput().getJSON().toString() + ")";
            } else {
              String json = "{\"name\": \"Unknown Beacon\", \"accuracy\": "+ beacon.getAccuracy() + ",\"uuid\": \"" + beacon.getUUID() +"\", \"major\": " + beacon.getMajor() + ", \"minor\": " + beacon.getMinor() + ", \"namespace\": \"" + beacon.getNamespace() + "\", \"instance\": \"" + beacon.getInstanceID() + "\"}";
              action = "javascript:proximiio.lostBeacon(" + json + ")";
            }
            log("lostBeacon", action);
            webView.loadUrl(action);
          }
        });
      }
    };

    proximiio.addListener(listener);
    proximiio.setAuth(token);

    activity.runOnUiThread(new Runnable() {
      @Override
      public void run() {
        String action = "javascript:proximiio.proximiioReady(\"" + proximiio.getVisitorID() + "\")";
        log("initProximiio", action);
        if (enableDebug) {
          Toast.makeText(activity, "Proximi.io Initialized!", Toast.LENGTH_SHORT).show();
        }
        webView.loadUrl(action);
      }
    });
  }

  private void log(String method, String action) {
    if (enableDebug) {
      Log.d(TAG, method + ": " + action);
    }
  }

  @Override
  public void onStart() {
    activity = cordova.getActivity();
    if (id != null && token != null) {
      initProximiio();
    }
  }

  @Override
  public void onStop() {
      super.onStop();
      if (proximiio != null) {
        proximiio.removeListener(listener);
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

  public void onRequestPermissionResult(int requestCode, String[] permissions,
                                        int[] grantResults) throws JSONException {
      PluginResult result;
      for(int r:grantResults) {
          if(r == PackageManager.PERMISSION_DENIED) {
              Log.d(TAG, "Permission Denied!");
              result = new PluginResult(PluginResult.Status.ILLEGAL_ACCESS_EXCEPTION);
              context.sendPluginResult(result);
              return;
          }
      }
      result = new PluginResult(PluginResult.Status.OK);
      initProximiio();
      context.sendPluginResult(result);
  }

  public boolean hasPermisssion() {
      for(String p : permissions) {
          if(!PermissionHelper.hasPermission(this, p)) {
              return false;
          }
      }
      return true;
  }

  /*
   * We override this so that we can access the permissions variable, which no longer exists in
   * the parent class, since we can't initialize it reliably in the constructor!
   */

  public void requestPermissions(int requestCode) {
      PermissionHelper.requestPermissions(this, requestCode, permissions);
  }
}
