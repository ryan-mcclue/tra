// SPDX-License-Identifier: zlib-acknowledgement

// TODO(Ryan): Investigate how to power board externally through Vin 

// IMPORTANT(Ryan): First thing to get an overview is to look at board pinout. Basically everything is user-configurable IO

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

// dbus formalise IPC with discrete packet sizes as oppose to byte streams with say pipes 

// for wifi, use esp8266?
// bluetooth, hm-10? require gatt profiles? FTDI chip could convert serial to USB, i.e. give us a COM port on the pc

// we want to know the baud and protocol of the bluetooth module so as to configure the UART pins on the STM32 to match

// #include <dbus/dbus.h>

#include <glib.h>
#include <gio/gio.h>


void on_remote_device_detection(GDBusConnection* connection,
                                const gchar* sender_name,
                                const gchar* object_path,
                                const gchar* interface_name,
                                const gchar* signal_name,
                                GVariant* parameters,
                                gpointer user_data)
{
  if (strcmp(object_path, adapter_object_path) == 0) return;

  if 
}

int
main(int argc, char *argv[])
{
  int dev_hci_id = hci_get_route(NULL);
  if (dev_hci_id < 0) 
  {
    fprintf(stderr, "Failed to get default bluetooth device hci id.\n");
    return 1;
  }

  struct hci_dev_info dev_info = {0};
  if (hci_devinfo(dev_hci_id, &dev_info) < 0)
  {
    fprintf(stderr, "Failed to get default bluetooth device information.\n");
    return 1;
  }

  char object_path[64] = {0};
  strcpy(object_path, "/org/bluez/");
  strcat(object_path, dev_info.name);

  GList *list;
  GMainLoop *loop;

  GError *err = NULL;
  GDBusConnection *gdbus_conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &err);
  if (gdbus_conn == NULL)
  {
    fprintf(stderr, "Failed to connect to DBus system bus (%s).\n", err->message);
    
    return 1;
  }

  // Listen to signal PropertiesChanged from any object indicated by NULL (this has to be the case as any device)
  int subscription_id = g_dbus_connection_signal_subscribe(gdbus_conn, "org.bluez", "org.freedesktop.DBus.Properties",
      "PropertiesChanged", NULL, NULL, G_DBUS_SIGNAL_FLAGS_NONE, on_remote_device_detection, NULL, NULL);


#define BUS_NAME "org.bluez"
#define INTERFACE_NAME "org.bluez.Adapter1"

  // DBUS_BUS_SESSION will make bluez unknown

  // IMPORTANT(Ryan): By default, we cannot register a name on the system bus
  // So, must create a policy file in /etc/dbus-1/system.d/my.bluetooth.client.conf 
  /*
   *  
      <!DOCTYPE busconfig PUBLIC "-//freedesktop//DTD D-BUS Bus Configuration 1.0//EN"
      "http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd">
      <busconfig>

      <!-- alternatively could do user="ryan" -->
      <policy context="default">
        <allow own="my.bluetooth.client"/>
      </policy>

      </busconfig>
   */

  // server, object, interface, method

  // signal (broadcast), METHOD_CALL returns METHOD_RETURN or ERROR
  
  // interface = org.bluez.Adapter1
  // object = /org/bluez/${dev_info.name}

   return 0;
}

// dbus built on-top of low-level IPCs like shared memory and sockets
// system bus for kernel, i.e for all users
// session bus for user
// if well known name of the service (org.freedesktop.ModemManager) is not started, dbus will start it for us?
// service exposes objects (/org/freedesktop/ModemManager)
// objects can have various interfaces (org.freedesktop.DBus.ObjectManager) (form of reverse domain name)
// the interface will have methods to access data (GetManagedObjects()), also have properties or signals

// our process interacts with dbus-daemon (libdbus) which interacts with systemd (sd-bus)
