package com.epicdecals.openslider;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Fragment;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.FragmentManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import android.view.MotionEvent;
import android.view.View.OnTouchListener;
import android.widget.RelativeLayout;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.MobileAds;

import java.util.UUID;

/**
 * Created by gdeeth on 11/13/2016.
 */

public class LiveControls extends android.support.v4.app.Fragment{
    AdView mAdView;
    View rootview;

    /////////////////////////////Bluetooth///////////////////////
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    //SPP UUID. This is the COM port of the BT device you are connecting to.
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    /////////////////////////////end Bluetooth///////////////////

    /////////////////////////////joystick////////////////////////
    RelativeLayout layout_joystick;
    ImageView image_joystick, image_border;

    JoyStickClass js;
    //////////////////////////////end joystick///////////////////



    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.live_control, container, false);

        //Creating listeners: Define objects
        SeekBar dampStart = (SeekBar) rootview.findViewById(R.id.sb_damp_start);
        SeekBar dampEnd = (SeekBar) rootview.findViewById(R.id.sb_damp_end);
        ImageView sliderWindow = (ImageView) rootview.findViewById(R.id.iv_slider);
        RelativeLayout joystickWindow = (RelativeLayout) rootview.findViewById(R.id.layout_joystick);

        SharedPreferences sharedPref = getContext().getSharedPreferences("Settings", Context.MODE_PRIVATE);
        String btAddress = sharedPref.getString("btDeviceAddress", "");

        //final FragmentManager fragmentManager = getFragmentManager();

        if(btAddress == "") {
            new AlertDialog.Builder(getContext())
                    .setTitle(R.string.ERROR_GENERIC)
                    .setMessage(R.string.ERROR_NO_DEVICE_SAVED)
                    .setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            /*fragmentManager.beginTransaction()
                                    .replace(R.id.content_main
                                            , new Settings())
                                    .addToBackStack(null)
                                    .commit();*/
                        }
                    })
                    .setIcon(android.R.drawable.ic_dialog_alert)
                    .show();
        }



        ////////////////////////////////joystick/////////////////////////
        layout_joystick = (RelativeLayout)rootview.findViewById(R.id.layout_joystick);

        js = new JoyStickClass(getContext(), layout_joystick, R.drawable.ball);
        js.setStickSize(150, 150);
        js.setLayoutSize(500, 500);
        js.setLayoutAlpha(255);
        js.setStickAlpha(255);
        js.setOffset(90);
        js.setMinimumDistance(50);

        layout_joystick.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View arg0, MotionEvent arg1) {
                js.drawStick(arg1);
                if(arg1.getAction() == MotionEvent.ACTION_DOWN
                        || arg1.getAction() == MotionEvent.ACTION_MOVE) {
                    /*textView1.setText("X : " + String.valueOf(js.getX()));
                    textView2.setText("Y : " + String.valueOf(js.getY()));
                    textView3.setText("Angle : " + String.valueOf(js.getAngle()));
                    textView4.setText("Distance : " + String.valueOf(js.getDistance()));*/


                } else if(arg1.getAction() == MotionEvent.ACTION_UP) {
                    /*textView1.setText("X :");
                    textView2.setText("Y :");
                    textView3.setText("Angle :");
                    textView4.setText("Distance :");
                    textView5.setText("Direction :");*/
                }
                return true;
            }
        });
        //////////////////////////////////end joystick/////////////////////////






        //Create listeners
        /*joystickWindow.setOnTouchListener(new View.OnTouchListener() {
            float dx=0,dy=0,x=0,y=0;

            @Override
            public boolean onTouch(View v, MotionEvent event){
                ImageView joystickPosition = (ImageView) rootview.findViewById(R.id.iv_joystick_position);

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN: {
                        x = event.getX();
                        y = event.getY();
                        dx = x - joystickPosition.getX();
                        dy = y - joystickPosition.getY();
                    }
                    break;
                    case MotionEvent.ACTION_MOVE: {
                        joystickPosition.setX(event.getX() - dx);
                        joystickPosition.setY(event.getY() - dy);
                    }
                    break;
                    case MotionEvent.ACTION_UP: {
                        centerJoystick();
                    }
                    case MotionEvent.ACTION_OUTSIDE: {
                        centerObjectToParent(rootview.findViewById(R.id.iv_joystick), rootview.findViewById(R.id.iv_joystick_position));
                    }
                    break;
                }
                return true;
            }
        });*/

        sliderWindow.setOnTouchListener(new View.OnTouchListener() {
            float dx=0,dy=0,x=0,y=0;

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                ImageView sliderPosition = (ImageView) rootview.findViewById(R.id.iv_slider_position);

                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN: {
                        x = event.getX();
                        dx = x - sliderPosition.getX();
                    }
                    break;
                    case MotionEvent.ACTION_MOVE: {
                        sliderPosition.setX(event.getX() - dx);
                    }
                    break;
                    case MotionEvent.ACTION_UP: {
                        centerObjectToParent(rootview.findViewById(R.id.iv_slider), rootview.findViewById(R.id.iv_slider_position));
                    }
                }
                return true;
            }
        });

        dampStart.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar dampStart, int progress, boolean fromUser) {
                TextView dampStartText = (TextView) rootview.findViewById(R.id.tv_damp_start);
                dampStartText.setText(getResources().getString(R.string.live_damp_start) + " " + String.valueOf(progress));
                ((MainActivity)getActivity()).sendBT("G1F" + String.valueOf(progress*100) + System.getProperty("line.separator"));
            }

            @Override
            public void onStartTrackingTouch(SeekBar dampStart) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar dampStart) {

            }
        });

        dampEnd.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar dampEnd, int progress, boolean fromUser) {
                TextView dampEndText = (TextView) rootview.findViewById(R.id.tv_damp_end);
                dampEndText.setText(getResources().getString(R.string.live_damp_end) + " " + String.valueOf(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar dampEtart) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar dampEtart) {

            }
        });

        /*//Admob initialization
        // Gets the ad view defined in layout/ad_fragment.xml with ad unit ID set in
        // values/strings.xml.
        mAdView = (AdView) rootview.findViewById(R.id.ad_view);
        // Create an ad request. Check your logcat output for the hashed device ID to
        // get test ads on a physical device. e.g.
        // "Use AdRequest.Builder.addTestDevice("ABCDEF012345") to get test ads on this device."
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .build();
        // Start loading the ad in the background.
        mAdView.loadAd(adRequest);*/

        return rootview;
    }

    /**
     * This method converts dp unit to equivalent pixels, depending on device density.
     *
     * @param dp A value in dp (density independent pixels) unit. Which we need to convert into pixels
     * @param context Context to get resources and device specific display metrics
     * @return A float value to represent px equivalent to dp depending on device density
     */
    public static float convertDpToPixel(float dp, Context context){
        Resources resources = context.getResources();
        DisplayMetrics metrics = resources.getDisplayMetrics();
        float px = dp * ((float)metrics.densityDpi / DisplayMetrics.DENSITY_DEFAULT);
        return px;
    }

    //Get system's status bar height
    public int getStatusBarHeight() {
        int result = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }

    //Centers the position marker on the slider graphic
    public void centerSlider(){
        ImageView sliderPosition = (ImageView) rootview.findViewById(R.id.iv_slider_position);
        int posXY[] = new int[2];
        rootview.findViewById(R.id.iv_slider).getLocationOnScreen(posXY);
        int centerX = posXY[0] + ((rootview.findViewById(R.id.iv_slider)).getWidth() / 2);
        int centerY = posXY[1] + ((rootview.findViewById(R.id.iv_slider)).getHeight() / 2);
        sliderPosition.setX(centerX - (rootview.findViewById(R.id.iv_slider_position).getWidth() / 2));
        sliderPosition.setY(centerY - getStatusBarHeight() - convertDpToPixel(60, rootview.getContext()) - (rootview.findViewById(R.id.iv_slider_position).getHeight() / 2));
    }

    //Centers the position marker on the joystick graphic
   /* public void centerJoystick(){
        ImageView joystickPosition = (ImageView) rootview.findViewById(R.id.iv_joystick_position);
        int posXY[] = new int[2];
        rootview.findViewById(R.id.iv_joystick).getLocationInWindow(posXY);
        int centerX = posXY[0] + ((rootview.findViewById(R.id.iv_joystick)).getWidth() / 2);
        int centerY = posXY[1] + ((rootview.findViewById(R.id.iv_joystick)).getHeight() / 2);
        joystickPosition.setX(centerX - (rootview.findViewById(R.id.iv_joystick_position).getWidth() / 2));
        joystickPosition.setY(centerY - getStatusBarHeight() - convertDpToPixel(60, rootview.getContext()) - (rootview.findViewById(R.id.iv_joystick_position).getHeight() / 2));
        //centerObjectToParent(rootview.findViewById(R.id.iv_joystick), 24);
    }*/

    public void centerObjectToParent(View parent, View child){
        //ImageView joystickPosition = (ImageView) rootview.findViewById(R.id.iv_joystick_position);
        int posXY[] = new int[2];
        parent.getLocationInWindow(posXY);
        int centerX = posXY[0] + (parent.getWidth() / 2);
        int centerY = posXY[1] + (parent.getHeight() / 2);
        child.setX(centerX - (child.getWidth() / 2));
        child.setY(centerY - getStatusBarHeight() - convertDpToPixel(60, rootview.getContext()) - (child.getHeight() / 2));
    }

    /** Called when leaving the activity */
    @Override
    public void onPause() {
        if (mAdView != null) {
            mAdView.pause();
        }
        super.onPause();
    }

    /** Called when returning to the activity */
    @Override
    public void onResume() {
        super.onResume();
        if (mAdView != null) {
            mAdView.resume();
        }
    }

    /** Called before the activity is destroyed */
    @Override
    public void onDestroy() {
        if (mAdView != null) {
            mAdView.destroy();
        }
        super.onDestroy();
    }

}
