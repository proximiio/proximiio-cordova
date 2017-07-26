package com.navtureapps.cordovaplugin;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import io.proximi.proximiiolibrary.ProximiioAPI;

public class ProximiioCordovaOutputReceiver extends BroadcastReceiver {
    private static final String TAG = "ProximiioCordovaOutput";

    @Override
    public void onReceive(Context context, Intent intent) {
        switch (intent.getAction()) {
            case ProximiioAPI.ACTION_OUTPUT:
                JSONObject json = null;
                try {
                    json = new JSONObject(intent.getStringExtra(ProximiioAPI.EXTRA_JSON));
                }
                catch (JSONException e) {
                    // Not a push
                }

                if (json != null) {
                    String title = null;
                    try {
                        if (!json.isNull("type") && !json.isNull("title")) {
                            if (json.getString("type").equals("push")) {
                                title = json.getString("title");
                            }
                        }
                    }
                    catch (JSONException e) {
                        // Not a push
                    }

                    if (title != null) {
						Intent intent2 = context.getPackageManager().getLaunchIntentForPackage(context.getPackageName());
						if (intent2 != null) {
							intent2.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
						}
						
                        PendingIntent contentIntent = intent2 != null ? PendingIntent.getActivity(context, 0, intent2, PendingIntent.FLAG_CANCEL_CURRENT) : null;
                        NotificationManager notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);

                        NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(context)
                                .setContentIntent(contentIntent)
                                .setSmallIcon(context.getResources().getIdentifier("notification_icon", "drawable", context.getPackageName()))
                                .setContentTitle(title);

                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
                            notificationBuilder.setPriority(Notification.PRIORITY_HIGH);
                        }

                        Notification notification = notificationBuilder.build();

                        notification.flags |= Notification.FLAG_AUTO_CANCEL;
                        notificationManager.notify(1, notification);
                    }
                }
                break;
        }
    }
}