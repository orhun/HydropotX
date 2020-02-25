package com.orhun.hydropotx

import android.bluetooth.BluetoothAdapter
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.bluetooth.BluetoothDevice
import android.content.pm.PackageManager
import android.util.Log
import android.widget.Toast


class MainActivity : AppCompatActivity() {

    private lateinit var bluetoothAdapter: BluetoothAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        if (!bluetoothAdapter.isEnabled) {
            val btIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(btIntent, 1)
        } else {
            val pairedDevices = bluetoothAdapter.bondedDevices
            if (pairedDevices.size > 0) {
                for (device: BluetoothDevice in pairedDevices) {
                    Log.d(device.address, device.name);
                }
            }
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        if (requestCode == 1) {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(this@MainActivity,
                    getString(R.string.bt_permission_error), Toast.LENGTH_LONG).show()
            } else {
                //
            }
        }
    }
}
