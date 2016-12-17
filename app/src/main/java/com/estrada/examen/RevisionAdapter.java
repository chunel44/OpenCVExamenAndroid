package com.estrada.examen;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class RevisionAdapter extends BaseAdapter{

    private Context mContext;
    private int[] answers;
    private Drawable correctOption;

    public RevisionAdapter(Context mContext, int[] answers){
        this.mContext = mContext;
        this.answers = answers;
    }

    @Override
    public int getCount() {
        return answers.length;
    }

    @Override
    public Object getItem(int position) {
        return answers[position];
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        final ViewHolder holder;
        LayoutInflater mLayoutInflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        convertView = mLayoutInflater.inflate(R.layout.listitem_questions, null);
        holder = new ViewHolder();
        correctOption = mContext.getResources().getDrawable(R.drawable.correcta);

        holder.numQuestion = (TextView) convertView.findViewById(R.id.question_listview);
        holder.option1 = (ImageView) convertView.findViewById(R.id.opcion_1);
        holder.option2 = (ImageView) convertView.findViewById(R.id.opcion_2);
        holder.option3 = (ImageView) convertView.findViewById(R.id.opcion_3);
        holder.option4 = (ImageView) convertView.findViewById(R.id.opcion_4);

        holder.numQuestion.setText("Pregunta "+(position + 1));
        if (answers[position] == 0){
            holder.option1.setImageDrawable(correctOption);
        } else if (answers[position] == 1){
            holder.option2.setImageDrawable(correctOption);
        } else if (answers[position] == 2){
            holder.option3.setImageDrawable(correctOption);
        } else if (answers[position] == 3){
            holder.option4.setImageDrawable(correctOption);
        }

        return convertView;
    }

    private static class ViewHolder {

        TextView numQuestion;
        ImageView option1;
        ImageView option2;
        ImageView option3;
        ImageView option4;
    }
}
