package com.orhun.hydropotx

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Toast


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
            Toast.makeText(this,
                getString(R.string.bt_device_error), Toast.LENGTH_LONG).show()
            finish()
        } else if (!bluetoothAdapter!!.isEnabled) {
            val btEnableIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(btEnableIntent, REQUEST_ENABLE_BT)
        } else {
            val pairedDevices = bluetoothAdapter!!.bondedDevices
            if (pairedDevices.size > 0) {
                for (device in pairedDevices) {
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
