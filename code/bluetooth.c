// SPDX-License-Identifier: zlib-acknowledgement

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

// dbus formalise IPC with discrete packet sizes as oppose to byte streams with say pipes 
#include <dbus/dbus.h>

int
main(int argc, char *argv[])
{
  int dev_id = hci_get_route(NULL);
  if (dev_id < 0) 
  {
    fprintf(stderr, "Failed get default hci\n");
    return 1;
  }

  struct hci_dev_info dev_info = {0};
  if (hci_devinfo(dev_id, &dev_info) < 0)
  {
    fprintf(stderr, "Failed default hci information\n");
    return 1;
  }

  // should give something similar to $(hcitool dev)
  printf("%s\n", dev_info.name);

  DBusError dbus_error;
  dbus_error_init(&dbus_error);

  // DBUS_BUS_SESSION will make bluez unknown
  DBusConnection *connection = dbus_bus_get(DBUS_BUS_SYSTEM, &dbus_error); 
  if (dbus_error_is_set(&dbus_error))
  {
    fprintf(stderr, "error: %s\n", dbus_error.message);
    return 1;
  }

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
  int ret = dbus_bus_request_name(connection, "my.bluetooth.client",
      DBUS_NAME_FLAG_REPLACE_EXISTING, &dbus_error);
  if (dbus_error_is_set(&dbus_error))
  {
    fprintf(stderr, "error: %s\n", dbus_error.message);
    return 1;
  }

  // server, object, interface, method
  DBusMessage *request = dbus_message_new_method_call("org.bluez", "/org/bluez/hci0", 
      "org.bluez.Adapter1", "StartDiscovery");
  if (request == NULL)
  {
    fprintf(stderr, "error in new method call\n");
    return 1;
  }

  // signal (broadcast), METHOD_CALL returns METHOD_RETURN or ERROR
  
  // interface = org.bluez.Adapter1
  // object = /org/bluez/${dev_info.name}

  // THIS LISTENS TO A NEW DEVICE ADDED
  //while (1)
  //{
  //  // non blocking
  //  dbus_connection_read_write(connection, 0); 
  //  DBusMessage *msg = dbus_connection_pop_message(connection);
  //  // loop until message
  //  if (msg == NULL) continue;

  //  if (dbus_message_is_method_call(msg, "org.freedesktop.DBus.Properties", "PropertiesChanged"))
  //  {

  //  }

  //}

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
