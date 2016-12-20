package com.epicdecals.openslider;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

/**
 * Created by gdeeth on 11/13/2016.
 */

public class Settings extends android.support.v4.app.Fragment {
    View rootview;


    //Widgets
    ListView devicelist;
    //Bluetooth
    private Set<BluetoothDevice> pairedDevices;
    private BluetoothAdapter myBluetooth = BluetoothAdapter.getDefaultAdapter(); // we already checked for a valid adapter on app load


    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        rootview = inflater.inflate(R.layout.settings, container, false);

        //Init shared preferences for reading settings
        SharedPreferences sharedPref = getContext().getSharedPreferences("Settings", Context.MODE_PRIVATE);

        ////////////Bluetooth//////////////
        devicelist = (ListView)rootview.findViewById(R.id.list_pairedDevices);
        //Create Listeners
        Button btn_pairedDevices = (Button) rootview.findViewById(R.id.btn_pairedDevices);

        //update screen with saved BT Address
        String btAddress = sharedPref.getString("btDeviceAddress", "");
        TextView tv = (TextView)rootview.findViewById(R.id.tv_savedBTAddress);

        if(btAddress == ""){
            tv.setText(getContext().getString(R.string.SETTINGS_BT_ADDRESS_TO_USE) + " " + getContext().getString(R.string.NONE));
        }else {
            tv.setText(getContext().getString(R.string.SETTINGS_BT_ADDRESS_TO_USE) + " " + btAddress);
        }

        //start listener for getting BT device address from list
        btn_pairedDevices.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                pairedDevicesList();
            }
        });
        /////////////end Bluetooth///////////

        return rootview;
    }

    private void pairedDevicesList() {
        pairedDevices = myBluetooth.getBondedDevices();
        ArrayList list = new ArrayList();

        if (pairedDevices.size() > 0) {
            for (BluetoothDevice bt : pairedDevices) {
                list.add(bt.getName() + "\n" + bt.getAddress()); //Get the device's name and the address
            }
        } else {
            Toast.makeText(getActivity().getApplicationContext(), getResources().getString(R.string.error_no_paired_devices), Toast.LENGTH_LONG).show();
        }

        final ArrayAdapter adapter = new ArrayAdapter(getActivity(), android.R.layout.simple_list_item_1, list);
        devicelist.setAdapter(adapter);
        devicelist.setOnItemClickListener(myListClickListener); //Method called when the device from the list is clicked
    }

    private AdapterView.OnItemClickListener myListClickListener = new AdapterView.OnItemClickListener() {
        public void onItemClick(AdapterView<?> av, View v, int arg2, long arg3) {
            //Init for saving and reading settings
            SharedPreferences sharedPref = getContext().getSharedPreferences("Settings", Context.MODE_PRIVATE);
            SharedPreferences.Editor editSetting = sharedPref.edit();

            // Get the device MAC address, the last 17 chars in the View
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);
            editSetting.putString("btDeviceAddress", address);  //Save address
            editSetting.commit();
            Log.d("STATE", "Captured New BT Address: " + address);
            TextView tv = (TextView)rootview.findViewById(R.id.tv_savedBTAddress);
            tv.setText(getContext().getString(R.string.SETTINGS_BT_ADDRESS_TO_USE) + " " + address);  //Update text view with saved address
            Toast.makeText(getContext(), getResources().getString(R.string.SETTINGS_BT_ADDRESS_SAVED), Toast.LENGTH_LONG).show();
        }
    };
}

