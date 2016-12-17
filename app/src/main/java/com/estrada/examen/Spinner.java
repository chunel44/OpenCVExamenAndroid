package com.estrada.examen;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

public class Spinner{

    public static Dialog spinnerDialog(Context context, String title) {

        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setCancelable(false);

        LayoutInflater layoutInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View spinnerView = layoutInflater.inflate(R.layout.dialog_spinner, null);

        TextView titleView = (TextView) spinnerView.findViewById(R.id.dialog_title);
        titleView.setText(title);

        builder.setView(spinnerView);

        return builder.create();
    }
}
