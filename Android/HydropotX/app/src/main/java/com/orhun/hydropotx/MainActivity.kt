package com.orhun.hydropotx

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.Toast
import android.R
import android.app.Dialog
import android.content.Context
import android.view.Window.FEATURE_NO_TITLE
import android.content.Context.LAYOUT_INFLATER_SERVICE
import androidx.core.content.ContextCompat.getSystemService
import android.view.LayoutInflater
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import android.view.Window


class MainActivity : Activity() {

    private lateinit var btnBt: Button
    private var bluetoothAdapter: BluetoothAdapter? = null
    companion object {
        private const val TAG = "HydropotX"
        private const val REQUEST_ENABLE_BT = 1
        private const val TARGET_DEVICE_NAME = "hydroponics"
        private const val TARGET_DEVICE_ADDR = "00:13:EF:00:B2:38"
    }

    private fun initViews() {
        btnBt = findViewById(R.id.btnBt)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        initViews()
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        if (bluetoothAdapter == null) {
            Toast.makeText(this,
                getString(R.string.bt_device_error), Toast.LENGTH_LONG).show()
            finish()
        } else if (!bluetoothAdapter!!.isEnabled) {
            val btEnableIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(btEnableIntent, REQUEST_ENABLE_BT)
        } else { /* */ }
        btnBt.setOnClickListener {

            val pairedDevices = bluetoothAdapter!!.bondedDevices
            if (pairedDevices.size > 0) {
                for (device in pairedDevices) {
                    Log.d(TAG, String.format("%s %s", device.name, device.address))
                }
            }

            val inflater = applicationContext
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE) as LayoutInflater
            val btDialog = Dialog(
                this,
                R.style.Theme_Black_NoTitleBar_Fullscreen
            )
            btDialog.window?.requestFeature(FEATURE_NO_TITLE)
            btDialog.setContentView(inflater.inflate(R.layout.layout_devices, null))

            btDialog.show()
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        when (requestCode) {
            REQUEST_ENABLE_BT ->
                if (resultCode == RESULT_OK) {
                    //
                } else {
                    Log.d(TAG, getString(R.string.bt_permission_error))
                    Toast.makeText(this, R.string.bt_permission_error,
                        Toast.LENGTH_SHORT).show()
                    finish()
                }
        }
    }

}
