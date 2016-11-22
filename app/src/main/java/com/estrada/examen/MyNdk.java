package com.estrada.examen;

import org.opencv.core.Mat;


public class MyNdk {
  //  ArrayList<Integer> res;
    //int x;
  public String message = "21408";

  /* ArrayList<Integer> getRes(int x){
     res.add(x);
       ArrayList<Integer> arr;
       arr = res;
       return arr;
   }*/


    static {
        System.loadLibrary("MyLibrary");
    }

    public native String getMyString();
    public native void modifyInstanceVariable(String text);
    public native int suma(int n);
    public native static int convertGray(long matAddrRgba, long matAddrGray);
    public static native void detect(long addrRgba);
    public static native void black(long addrRgba);
    public native int[] prueba(int size);
  public native int[] resultado(long addrRgba);
}

