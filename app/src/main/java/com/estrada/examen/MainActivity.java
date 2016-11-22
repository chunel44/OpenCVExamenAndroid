package com.estrada.examen;

import android.os.CountDownTimer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.AppCompatButton;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.imgproc.Imgproc;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2 {
    static final String TAG = MainActivity.class.getSimpleName();
    MyNdk myNdk;
    ArrayList<Integer> resE;
    int flag = 0;
    JavaCameraView javaCameraView;
    AppCompatButton boton;
    Mat mRgba,imgGray,imgCanny;
    String catchedValue = "No detected, try again";

    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status){
                case BaseLoaderCallback.SUCCESS:{
                    javaCameraView.enableView();
                    break;
                }
                default:{
                    super.onManagerConnected(status);
                    break;
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_main);
        myNdk = new MyNdk();
        boton = (AppCompatButton) findViewById(R.id.boton);
//        System.out.println(myNdk.message);
//        myNdk.modifyInstanceVariable("Hola Como estas");
//        System.out.println(myNdk.message);
        //Drawable drawable = getResources().getDrawable(R.drawable.example);
        javaCameraView = (JavaCameraView)findViewById(R.id.JavaCamera);
        javaCameraView.setVisibility(SurfaceView.VISIBLE);
        javaCameraView.setCvCameraViewListener(this);
        boton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(flag == 0){
                    flag++;
                }else{
                    flag--;
                }
                CountDownTimer timer = new CountDownTimer(6000, 6000)
                {
                    public void onTick(long millisUntilFinished)
                    {
                    }

                    public void onFinish()
                    {
                        Toast.makeText(getApplicationContext(),"Values: "+ catchedValue, Toast.LENGTH_LONG).show();
                    }
                };
                timer.start();
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();
        if(javaCameraView!=null){
            javaCameraView.disableView();
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(javaCameraView!=null)javaCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }

    @Override
    public void onCameraViewStarted(int width, int height) {
        mRgba = new Mat(height, width, CvType.CV_8UC4);


    }

    @Override
    public void onCameraViewStopped() {
        mRgba.release();
    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        mRgba = inputFrame.rgba();
      if(flag == 1){
          //System.out.println(myNdk.message);
        myNdk.detect(mRgba.getNativeObjAddr());
          flag--;
          try {
              Process process = Runtime.getRuntime().exec("logcat -d");
              BufferedReader bufferedReader = new BufferedReader(
                      new InputStreamReader(process.getInputStream()));

              String log = "";
              String line = "";
              while ((line = bufferedReader.readLine()) != null) {
                  log = line;
              }
              catchedValue = log.substring((log.indexOf(':') + 2), log.length());
              System.out.println("Values: "+log);
              System.out.println("CatchedValue: " + catchedValue);
          }
          catch (IOException e) {}
       }
        return mRgba;
    }
}
