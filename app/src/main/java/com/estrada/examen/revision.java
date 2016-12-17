package com.estrada.examen;

import android.app.Dialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ListView;
import android.widget.Toast;

public class revision extends AppCompatActivity {
    String score;
    int[] answers;
    ListView revisionListView;
    RevisionAdapter revisionAdapter;
    Dialog spinner;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_revision);

        revisionListView = (ListView) findViewById(R.id.results_listview);

        if (getIntent().getExtras() != null) {
            score = getIntent().getStringExtra("Score");
            Toast.makeText(getApplicationContext(), "Values: " + score, Toast.LENGTH_LONG).show();
        }

        spinner = Spinner.spinnerDialog(this, "Loading results");
        spinner.show();

        answers = new int[score.length()];

        for (int i=0; i<score.length(); i++){
            answers[i] = Integer.parseInt(score.substring(i, i+1));
        }

        revisionAdapter = new RevisionAdapter(this, answers);

        revisionListView.setAdapter(revisionAdapter);
        revisionListView.setDivider(null);
        spinner.dismiss();
    }
}