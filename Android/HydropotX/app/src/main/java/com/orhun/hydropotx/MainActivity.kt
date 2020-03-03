package com.orhun.hydropotx

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import android.widget.Toast
import android.app.Dialog
import android.view.Window.FEATURE_NO_TITLE
import android.widget.ListView

class MainActivity : Activity() {

    private lateinit var btnBt: Button
    private var bluetoothAdapter: BluetoothAdapter? = null
    companion object {
        private const val TAG = "HydropotX"
        private const val REQUEST_ENABLE_BT = 1
        private const val TARGET_DEVICE_NAME = "hydroponics"
        private const val TARGET_DEVICE_ADDR = "00:13:EF:00:B2:38"
    }
    data class BluetoothDevice(val name: String, val address: String)

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
                val deviceList = arrayListOf<BluetoothDevice>()
                for (device in pairedDevices) {
                    deviceList.add(BluetoothDevice(device.name, device.address))
                }
                for (device in pairedDevices) {
                    deviceList.add(BluetoothDevice(device.name, device.address))
                }
                for (device in pairedDevices) {
                    deviceList.add(BluetoothDevice(device.name, device.address))
                }
                for (device in pairedDevices) {
                    deviceList.add(BluetoothDevice(device.name, device.address))
                }
                val btDialog = Dialog(
                    this,
                    android.R.style.Theme_Dialog
                )
                btDialog.window?.requestFeature(FEATURE_NO_TITLE)
                btDialog.setContentView(View.inflate(this,
                    R.layout.layout_devices, null))
                btDialog.findViewById<ListView>(R.id.btDeviceList).adapter =
                    DeviceAdapter(this, deviceList)
                btDialog.show()

            } else {
                TODO("not paired")
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
