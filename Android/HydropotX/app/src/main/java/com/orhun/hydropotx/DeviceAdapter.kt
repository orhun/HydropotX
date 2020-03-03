package com.orhun.hydropotx

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.TextView

class DeviceAdapter(context: Context,
                    private val devices: ArrayList<MainActivity.BluetoothDevice>) : BaseAdapter() {
    private val layoutInflater: LayoutInflater = LayoutInflater.from(context)

    override fun getCount(): Int {
        return devices.size
    }

    override fun getItem(position: Int): Any {
        return devices[position]
    }

    override fun getItemId(position: Int): Long {
        return position.toLong()
    }

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View? {
        val viewHolder: ViewHolder
        val rowView: View?
        if (convertView == null) {
            rowView = layoutInflater.inflate(R.layout.item_device, parent, false)
            viewHolder = ViewHolder(rowView)
            rowView.tag = viewHolder
        } else {
            rowView = convertView
            viewHolder = rowView.tag as ViewHolder
        }
        viewHolder.btDeviceName.text = devices[position].name
        return rowView
    }

    private class ViewHolder(view: View?) {
        val btDeviceName = view?.findViewById(R.id.txvDeviceName) as TextView
    }
}