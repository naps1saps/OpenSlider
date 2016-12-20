package com.epicdecals.openslider;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.FragmentManager;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.MobileAds;
import android.app.Activity;
import android.os.Bundle;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private AdView mAdView;

    private BluetoothAdapter myBluetooth = null;
    private ProgressDialog progress;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    String btAddress = null;
    //SPP UUID. This is the COM port of the BT device you are connecting to.
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        ///////////////////////////START AD MOB INIT
        MobileAds.initialize(this, "ca-app-pub-8646202503635771~7287653003");

        // Gets the ad view defined in layout/ad_fragment.xml with ad unit ID set in
        // values/strings.xml.
        mAdView = (AdView) findViewById(R.id.ad_view);

        // Create an ad request. Check your logcat output for the hashed device ID to
        // get test ads on a physical device. e.g.
        // "Use AdRequest.Builder.addTestDevice("ABCDEF012345") to get test ads on this device."
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .build();

        // Start loading the ad in the background.
        mAdView.loadAd(adRequest);
        ///////////////////////////END AD MOB INIT

        ///////////////////////////Check Device has BT
        //if the device has bluetooth
        myBluetooth = BluetoothAdapter.getDefaultAdapter();

        if(myBluetooth == null)
        {
            //Show a message that the device has no bluetooth adapter
            Toast.makeText(getApplicationContext(), getResources().getString(R.string.error_no_bluetooth_adapter), Toast.LENGTH_LONG).show();

            //Close APP
            finish();
        }
        else if(!myBluetooth.isEnabled())
        {
            //Ask to the user turn the bluetooth on
            Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnBTon,1);
        }
        ///////////////////////////END Check Device has BT
        //Get address from storage
        SharedPreferences sharedPref = getApplicationContext().getSharedPreferences("Settings", Context.MODE_PRIVATE);
        btAddress = sharedPref.getString("btDeviceAddress", "");
    }



    @Override
    public void onBackPressed() {
        //Goes to previous fragment
        if (getFragmentManager().getBackStackEntryCount() > 0) {
            getFragmentManager().popBackStackImmediate();
        } else {
        //if no more fragments, go to android home
            //super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_connect) {
            SharedPreferences sharedPref = getApplicationContext().getSharedPreferences("Settings", Context.MODE_PRIVATE);
            btAddress = sharedPref.getString("btDeviceAddress", "");
            if(btAddress == "") {
                new AlertDialog.Builder(this)
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
            }else {
                new ConnectBT().execute(); //Call the class to connect
            }
            return true;
        } else if(id == R.id.action_disconnect) {
            Disconnect();
        }

        return super.onOptionsItemSelected(item);
    }


    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();
        String title = null;
        FragmentManager fragmentManager = getSupportFragmentManager();


        if (id == R.id.nav_live_control) {
            fragmentManager.beginTransaction()
                    .replace(R.id.content_main
                        , new LiveControls())
                    .addToBackStack(null)
                    .commit();
            //set title
            title = getResources().getString(R.string.live_control_title);

        } else if (id == R.id.nav_easy_motion) {
            fragmentManager.beginTransaction()
                    .replace(R.id.content_main
                            , new EasyMotion())
                    .addToBackStack(null)
                    .commit();
            //set title
            title = getResources().getString(R.string.easy_motion_title);

        } else if (id == R.id.nav_settings) {
            fragmentManager.beginTransaction()
                    .replace(R.id.content_main
                            , new Settings())
                    .addToBackStack(null)
                    .commit();
            //set title
            title = getResources().getString(R.string.settings_title);

        } else if (id == R.id.nav_content_main) {
            fragmentManager.popBackStack(null, FragmentManager.POP_BACK_STACK_INCLUSIVE);
            //set title
            title = getResources().getString(R.string.about_title);

        } else if (id == R.id.nav_share) {
            //Opens share options
            Intent sharingIntent = new Intent(android.content.Intent.ACTION_SEND);
            sharingIntent.setType("text/plain");
            sharingIntent.putExtra(android.content.Intent.EXTRA_SUBJECT, R.string.share_subject);
            sharingIntent.putExtra(android.content.Intent.EXTRA_TEXT, R.string.share_body);
            startActivity(Intent.createChooser(sharingIntent, getResources().getString(R.string.share_via)));

        } else if (id == R.id.nav_donate) {
            fragmentManager.beginTransaction()
                    .replace(R.id.content_main
                            , new Donate())
                    .addToBackStack(null)
                    .commit();
            //set title
            title = getResources().getString(R.string.donate_title);
        }

        //setting the title
        if (title != null){
            getSupportActionBar().setTitle(title);
        } else{
            getSupportActionBar().setTitle(R.string.app_name);
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    private void Disconnect()
    {
        if (btSocket!=null) //If the btSocket is busy
        {
            try
            {
                btSocket.close(); //close connection
            }
            catch (IOException e)
            { msg(getResources().getString(R.string.ERROR_DISCONNECT));}
        }
        msg(getResources().getString(R.string.BT_DISCONNECT_CONFIRMATION));
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute() {
            progress = ProgressDialog.show(MainActivity.this, (getResources().getString(R.string.CONNECTING) + "..."), (getResources().getString(R.string.PLEASE_WAIT) + "!"));  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try {
                if (btSocket == null || !isBtConnected) {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(btAddress);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            } catch (IOException e) {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                msg(getResources().getString(R.string.ERROR_CONNECT));
                finish();
            }
            else
            {
                msg(getResources().getString(R.string.CONNECTED));
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }

    //Write to BT
    public void sendBT(String s){
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write(s.toString().getBytes());
            }
            catch (IOException e)
            {
                msg(getResources().getString(R.string.ERROR_SEND));
            }
        }
    }

    // fast way to call Toast
    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }

//Button actions
    public void go_paypal(View v){
        //Opens PayPal donation site
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=9RWHJQLKB3T9L"));
        startActivity(browserIntent);
    }

    public void go_epicdecals(View v){
        //Opens Epic Decals site
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("http://epicdecals.com/Store/index.php/cPath/34"));
        startActivity(browserIntent);
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
