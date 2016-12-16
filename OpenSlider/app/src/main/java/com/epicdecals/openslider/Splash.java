package com.epicdecals.openslider;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.ImageView;


/**
 * Created by gdeeth on 12/15/2016.
 */

public class Splash extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //setTheme(R.style.SplashTheme);
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.splash);

        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
        finish();
    }
}
