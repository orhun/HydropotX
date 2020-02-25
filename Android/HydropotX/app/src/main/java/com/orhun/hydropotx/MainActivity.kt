package com.orhun.hydropotx

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.content.Intent
import android.os.Bundle
import android.bluetooth.BluetoothDevice
import android.content.pm.PackageManager
import android.os.Handler
import android.util.Log
import android.widget.Toast
import android.os.Build
import androidx.core.content.ContextCompat.getSystemService
import android.icu.lang.UCharacter.GraphemeClusterBreak.T




class MainActivity : Activity() {

    private var bluetoothAdapter: BluetoothAdapter? = null
    companion object {
        private val TAG = "HydropotX"
        private val REQUEST_ENABLE_BT = 1
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        if (bluetoothAdapter == null) {

        } else if (!bluetoothAdapter!!.isEnabled) {
            val btIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(btIntent, REQUEST_ENABLE_BT)
        } else {
            val pairedDevices = bluetoothAdapter!!.bondedDevices
            if (pairedDevices.size > 0) {
                for (device: BluetoothDevice in pairedDevices) {
                    Log.d(device.address, device.name)
                }
            }
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
