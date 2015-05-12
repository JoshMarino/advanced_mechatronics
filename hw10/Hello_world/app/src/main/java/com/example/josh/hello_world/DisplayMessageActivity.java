package com.example.josh.hello_world;

import android.content.Intent;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;


public class DisplayMessageActivity extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Intent intent = getIntent(); //Get the intent and assign it to a local variable.
        String message = intent.getStringExtra(MainActivity.EXTRA_MESSAGE); //Extract the message delivered by MyActivity with the getStringExtra() method

        TextView textView = new TextView(this); //create a TextView object
        textView.setTextSize(40); //Set the text size and message with setText()
        textView.setText(message);

        setContentView(textView); //Add the TextView as the root view of the activity’s layout by passing it to setContentView()
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
